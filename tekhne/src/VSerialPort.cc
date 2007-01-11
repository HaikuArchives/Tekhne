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

using namespace tekhne;

VSerialPort::VSerialPort(void) : _is_open(false) {
	memset(_name, 0, V_NAME_LENGTH);
}

VSerialPort::~VSerialPort() {
}

void VSerialPort::ClearInput(void) {
}

void VSerialPort::ClearOutput(void) {
}

bool VSerialPort::IsCTS(void) {
	return false;
}

bool VSerialPort::IsDCD(void) {
	return false;
}

bool VSerialPort::IsDSR(void) {
	return false;
}

bool VSerialPort::IsRI(void) {
	return false;
}

status_t VSerialPort::Open(const char *name) {
	return V_ERROR;
}

void VSerialPort::Close(void) {
}

ssize_t VSerialPort::Read(void *buffer, size_t maxBytes) {
	return 0;
}

void VSerialPort::SetBlocking(bool shouldBlock) {
}

status_t VSerialPort::SetTimeout(bigtime_t timeout) {
	return V_ERROR;
}

void VSerialPort::SetDataBits(data_bits count) {
}

void VSerialPort::SetStopBits(stop_bits count) {
}

void VSerialPort::SetParityMode(parity_mode mode) {
}

data_bits VSerialPort::DataBits(void) {
	return V_DATA_BITS_8;
}

stop_bits VSerialPort::StopBits(void) {
}

parity_mode VSerialPort::ParityMode(void) {
}

status_t VSerialPort::SetDataRate(data_rate bitsPerSecond) {
	return V_ERROR;
}

data_rate VSerialPort::DataRate(void) {
	return V_0_BPS;
}

status_t VSerialPort::SetDTR(bool pinAsserted) {
	return V_ERROR;
}

void VSerialPort::SetFlowControl(uint32_t mask) {
}

uint32_t VSerialPort::FlowControl(void) {
	return 0;
}

status_t VSerialPort::SetRTS(bool pinAsserted) {
	return V_ERROR;
}

ssize_t VSerialPort::WaitForInput(void) {
	return 0;
}

ssize_t VSerialPort::Write(const void *data, size_t numBytes) {
	return 0;
}
