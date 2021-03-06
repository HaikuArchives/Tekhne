/***************************************************************************
 *            VSerialPortTest.cc
 *
 * Copyright (c) 2006 Geoffrey Clements
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

#include "VSerialPortTest.h"
#include <iostream>

using namespace std;

void VSerialPortTest::setUp() {
	sp = new VSerialPort();
}

void VSerialPortTest::tearDown() {
	delete sp;
}

void VSerialPortTest::testCountAndName() {
	int32_t num = sp->CountDevices();
	cout << "devices: " << num << endl;
	for (int i=0; i<=num; i++) {
		char name[V_NAME_LENGTH];
		char buf[256];
		CPPUNIT_ASSERT(sp->GetDeviceName(i, name, V_NAME_LENGTH) == V_OK);
		sprintf(buf, "/dev/ttyS%d",i);
		CPPUNIT_ASSERT(strcmp(name, buf) == 0);
	}

}

void VSerialPortTest::testOpenAndClose() {
	int32_t num = sp->CountDevices();
	for (int i=0; i<num; i++) {
		char name[V_NAME_LENGTH];
		sp->GetDeviceName(i, name, V_NAME_LENGTH);
		CPPUNIT_ASSERT(sp->Open(name) == V_OK);
		sp->Close();
	}
}

void VUSBTest::setUp() {
	VUSB::Init();
	usb = new VUSB();
}

void VUSBTest::tearDown() {
	delete usb;
}

void VUSBTest::testCreate() {
}
