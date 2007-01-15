/***************************************************************************
 *            VUSB.cc
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

#include <usb.h>

using namespace tekhne;

bool VUSB::_init_called = false;

VUSB::VUSB(void) {
}

VUSB::~VUSB() {
	Close();
}

status_t VUSB::Open(const char *name) {
	return V_ERROR;
}

void VUSB::Close(void) {
}

ssize_t VUSB::Read(void *buffer, size_t maxBytes) {
	return V_ERROR;
}

ssize_t VUSB::Write(const void *data, size_t numBytes) {
	return V_ERROR;
}


int32_t VUSB::CountDevices(void) {
	return 0;
}

status_t VUSB::GetDeviceName(int32_t index, char *outName, size_t bufSize) {
}
