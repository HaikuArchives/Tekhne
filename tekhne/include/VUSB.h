/***************************************************************************
 *            VUSB.h
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

#ifndef _VUSB_H
#define _VUSB_H

#include <StandardDefs.h>
#include <usb.h>

namespace tekhne {

class VUSB {
	static bool _init_called;
private:
public:
	VUSB(void);
	virtual ~VUSB();

	status_t Open(const char *name);
	void Close(void);
	ssize_t Read(void *buffer, size_t maxBytes);
	ssize_t Write(const void *data, size_t numBytes);

	int32_t CountDevices(void);
	status_t GetDeviceName(int32_t index, char *outName, size_t bufSize = V_NAME_LENGTH);

	static void Init() { if (!_init_called) { usb_init(); _init_called = true; } }
};


} // namespace tekhne

#endif /* _VUSB_H */
