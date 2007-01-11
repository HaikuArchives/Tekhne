/***************************************************************************
 *            VSerialPort.h
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

#ifndef _VSERIALPORT_H
#define _VSERIALPORT_H

#include <stdint.h>

namespace tekhne {

typedef enum { V_DATA_BITS_7, V_DATA_BITS_8 } data_bits;
typedef enum { V_STOP_BITS_1, V_STOP_BITS_2 } stop_bits;
typedef enum { V_EVEN_PARITY, V_ODD_PARITY, V_NO_PARITY } parity_mode;
typedef enum { V_0_BPS, V_50_BPS, V_75_BPS, V_110_BPS, V_134_BPS, V_150_BPS,
	V_200_BPS, V_300_BPS, V_600_BPS, V_1200_BPS, V_1800_BPS, V_2400_BPS,
	V_4800_BPS, V_9600_BPS, V_19200_BPS, V_31250_BPS, V_38400_BPS, V_57600_BPS,
	V_115200_BPS, V_230400_BPS } data_rate;

const int32_t V_SOFTWARE_CONTROL = 1;
const int32_t V_HARDWARE_CONTROL = 2;

class VSerialPort {
private:
	char _name[V_NAME_LENGTH];
	bool _is_open;
public:
	VSerialPort(void);
	virtual ~VSerialPort();

	void ClearInput(void);
	void ClearOutput(void);

	bool IsCTS(void);
	bool IsDCD(void);
	bool IsDSR(void);
	bool IsRI(void);
	status_t Open(const char *name);
	void Close(void);
	ssize_t Read(void *buffer, size_t maxBytes);
	void SetBlocking(bool shouldBlock);
	status_t SetTimeout(bigtime_t timeout);

	void SetDataBits(data_bits count);
	void SetStopBits(stop_bits count);
	void SetParityMode(parity_mode mode);
	data_bits DataBits(void);
	stop_bits StopBits(void);
	parity_mode ParityMode(void);

	status_t SetDataRate(data_rate bitsPerSecond);
	data_rate DataRate(void);

	status_t SetDTR(bool pinAsserted);
	
	void SetFlowControl(uint32_t mask);
	uint32_t FlowControl(void);

	status_t SetRTS(bool pinAsserted);

	ssize_t WaitForInput(void);

	ssize_t Write(const void *data, size_t numBytes);
};


} // namespace tekhne

#endif /* _VSERIALPORT_H */
