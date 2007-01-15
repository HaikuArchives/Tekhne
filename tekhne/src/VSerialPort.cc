/***************************************************************************
 *            VSerialPort.cc
 *
 * Copyright (c) 2007 Geoffrey Clements
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 ****************************************************************************/

#include "tekhne.h"

#include <stdio.h>
#include <fcntl.h>
#include <termio.h>

using namespace tekhne;

VSerialPort::VSerialPort(void) : _device_count(-1), _fd(-1) {
	memset(_name, 0, V_NAME_LENGTH);
	SetFlowControl(V_HARDWARE_CONTROL);
	SetDataRate(V_19200_BPS);
	SetDataBits(V_DATA_BITS_8);
	SetStopBits(V_STOP_BITS_1);
	SetParityMode(V_NO_PARITY);
	SetBlocking(true);
	SetTimeout(V_INFINITE_TIMEOUT);
}

VSerialPort::~VSerialPort() {
	Close();
}

void VSerialPort::ClearInput(void) {
}

void VSerialPort::ClearOutput(void) {
}

bool VSerialPort::IsCTS(void) {
	if (_fd >= 0) {
		int32_t status;
		ioctl(_fd, TIOCMGET, &status);
		return status & TIOCM_CTS;
	}
	return false;
}

bool VSerialPort::IsDCD(void) {
	if (_fd >= 0) {
		int32_t status;
		ioctl(_fd, TIOCMGET, &status);
		return status & TIOCM_CAR;
	}
	return false;
}

bool VSerialPort::IsDSR(void) {
	if (_fd >= 0) {
		int32_t status;
		ioctl(_fd, TIOCMGET, &status);
		return status & TIOCM_DSR;
	}
	return false;
}

bool VSerialPort::IsRI(void) {
	if (_fd >= 0) {
		int32_t status;
		ioctl(_fd, TIOCMGET, &status);
		return status & TIOCM_RI;
	}
	return false;
}

static void update_data_rate(int32_t fd, int32_t br) {
	struct termios options;
	tcgetattr(fd, &options);
	cfsetispeed(&options, br);
	cfsetospeed(&options, br);
	options.c_cflag |= (CLOCAL | CREAD);
	tcsetattr(fd, TCSANOW, &options);
}

static void update_data_bits(int32_t fd, int32_t bits) {
	struct termios options;
	tcgetattr(fd, &options);
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= bits;
	tcsetattr(fd, TCSANOW, &options);
}

static void update_parity(int32_t fd, int32_t parity) {
	struct termios options;
	tcgetattr(fd, &options);
	switch(parity) {
		case V_EVEN_PARITY:
			options.c_cflag |= PARENB;
			options.c_cflag &= ~PARODD;
			break;
		case V_ODD_PARITY:
			options.c_cflag |= PARENB;
			options.c_cflag |= PARODD;
			break;
		case V_NO_PARITY:
			options.c_cflag &= ~PARENB;
			break;
	}
	tcsetattr(fd, TCSANOW, &options);
}

static void update_stop_bits(int32_t fd, int32_t sb) {
	struct termios options;
	tcgetattr(fd, &options);
	switch(sb) {
		case V_STOP_BITS_1:
			options.c_cflag &= ~CSTOPB;
			break;
		case V_STOP_BITS_2:
			options.c_cflag |= CSTOPB;
			break;
	}
	tcsetattr(fd, TCSANOW, &options);
}

static void update_blocking(int32_t fd, bool shouldBlock) {
	if (shouldBlock) fcntl(fd, F_SETFL, 0);
	else fcntl(fd, F_SETFL, FNDELAY);
}

static void update_timeout(int32_t fd, bigtime_t timeout) {
	struct termios options;
	tcgetattr(fd, &options);
	options.c_cc[VTIME] = int(timeout/100000LL);
	tcsetattr(fd, TCSANOW, &options);
}

status_t VSerialPort::Open(const char *name) {
	_fd = open(name, O_RDWR | O_NOCTTY | O_NDELAY);
	if (_fd < 0) {
		return V_FILE_ERROR;
	}
	// update setting on this open port
	// flow control
	fcntl(_fd, F_SETFL, _flow_control);
	// data rate
	update_data_rate(_fd, _data_rate);
	// data bits
	update_data_bits(_fd, _data_bits);
	// stop bits
	update_stop_bits(_fd, _stop_bits);
	// parity
	update_parity(_fd, _parity_mode);
	// blocking
	update_blocking(_fd, _blocking);
	// timeout
	update_timeout(_fd, _timeout);

	return V_OK;
}

void VSerialPort::Close(void) {
	if (_fd >= 0) {
		close(_fd);
		_fd = -1;
	}
}

ssize_t VSerialPort::Read(void *buffer, size_t maxBytes) {
	if (_fd >= 0) {
		return read(_fd, buffer, maxBytes);
	}
	return V_FILE_ERROR;
}

void VSerialPort::SetBlocking(bool shouldBlock) {
	_blocking = shouldBlock;
	if (_fd >= 0) {
		update_blocking(_fd, _blocking);
	}
}

status_t VSerialPort::SetTimeout(bigtime_t timeout) {
	_timeout = timeout;
	if (_fd >= 0) {
		update_timeout(_fd, _timeout);
	}
	return V_OK;
}

void VSerialPort::SetDataBits(data_bits count) {
	switch(count) {
		case V_DATA_BITS_7:
		case V_DATA_BITS_8:
			break;
		default:
			return;
	}
	_data_bits = count;
	if (_fd >= 0) {
		// update open port
		update_data_bits(_fd, _data_bits);
	}
}

void VSerialPort::SetStopBits(stop_bits count) {
	if (count != V_STOP_BITS_1 && count != V_STOP_BITS_2) {
		return;
	}
	_stop_bits = count;
	if (_fd >= 0) {
		// update open port
		update_stop_bits(_fd, _stop_bits);
	}
}

void VSerialPort::SetParityMode(parity_mode mode) {
	switch(mode) {
		case V_EVEN_PARITY:
		case V_ODD_PARITY:
		case V_NO_PARITY:
			break;
		default:
			return;
	}
	_parity_mode = mode;
	if (_fd >= 0) {
		// update open port
		update_parity(_fd, _parity_mode);
	}
}

data_bits VSerialPort::DataBits(void) {
	return _data_bits;
}

stop_bits VSerialPort::StopBits(void) {
	return _stop_bits;
}

parity_mode VSerialPort::ParityMode(void) {
	return _parity_mode;
}

status_t VSerialPort::SetDataRate(data_rate bitsPerSecond) {
	switch(bitsPerSecond) {
		case V_0_BPS:
		case V_50_BPS:
		case V_75_BPS:
		case V_110_BPS:
		case V_134_BPS:
		case V_150_BPS:
		case V_200_BPS:
		case V_300_BPS:
		case V_600_BPS:
		case V_1200_BPS:
		case V_1800_BPS:
		case V_2400_BPS:
		case V_4800_BPS:
		case V_9600_BPS:
		case V_19200_BPS:
		case V_38400_BPS:
		case V_57600_BPS:
		case V_115200_BPS:
		case V_230400_BPS:
			break;
		default:
			return V_BAD_VALUE;
	}
	_data_rate = bitsPerSecond;
	if (_fd >= 0) {
		// update open port
		update_data_rate(_fd, _data_rate);
	}
	return V_OK;
}

data_rate VSerialPort::DataRate(void) {
	return _data_rate;
}

status_t VSerialPort::SetDTR(bool pinAsserted) {
	if (_fd >= 0) {
		int32_t status;
		ioctl(_fd, TIOCMGET, &status);
		if (pinAsserted) status |= TIOCM_DTR;
		else status &= ~TIOCM_DTR;
		ioctl(_fd, TIOCMSET, &status);
		return V_OK;
	}
	return V_FILE_ERROR;
}

void VSerialPort::SetFlowControl(uint32_t mask) {
	if (_fd < 0) {
		// validity check
		switch(mask) {
			case V_SOFTWARE_CONTROL:
			case V_HARDWARE_CONTROL:
				break;
			default:
				return;
		}
		_flow_control = mask;
	}
}

uint32_t VSerialPort::FlowControl(void) {
	return _flow_control;
}

status_t VSerialPort::SetRTS(bool pinAsserted) {
	if (_fd >= 0) {
		int32_t status;
		ioctl(_fd, TIOCMGET, &status);
		if (pinAsserted) status |= TIOCM_RTS;
		else status &= ~TIOCM_RTS;
		ioctl(_fd, TIOCMSET, &status);
		return V_OK;
	}
	return V_FILE_ERROR;
}

ssize_t VSerialPort::WaitForInput(void) {
	if (_fd >= 0) {
		fd_set input;

		FD_ZERO(&input);
		FD_SET(_fd, &input);

		int32_t max_fd = _fd + 1;

		if (_timeout) {
			struct timeval timeout;
			timeout.tv_sec = int(_timeout/1000000LL);
			timeout.tv_usec = int(_timeout%1000000LL);
			return select(max_fd, &input, NULL, NULL, &timeout);
		} else {
			return select(max_fd, &input, NULL, NULL, NULL);
		}
	}
	return V_ERROR;
}

ssize_t VSerialPort::Write(const void *data, size_t numBytes) {
	if (_fd >= 0) {
		return write(_fd, data, numBytes);
	}
	return V_FILE_ERROR;
}

int32_t VSerialPort::CountDevices(void) {
	if (_device_count < 0) {
		char buf[V_NAME_LENGTH];
		_device_count = 0;
		for (int i=0;i<256;i++) {
			sprintf(buf, "/dev/ttyS%d",i);
			int32_t fd = open(buf, O_RDWR | O_NOCTTY | O_NDELAY);
			if (fd >= 0) {
				_device_count++;
			}
		}
	}
	return _device_count;
}

status_t VSerialPort::GetDeviceName(int32_t index, char *outName, size_t bufSize) {
	if (index < 0 || index > CountDevices()) {
		return V_BAD_INDEX;
	}
	char buf[V_NAME_LENGTH];
	sprintf(buf, "/dev/ttyS%d", index);
	if (strlen(buf) > bufSize-1) {
		return V_NAME_TOO_LONG;
	}
	strcpy(outName, buf);
	return V_OK;
}
