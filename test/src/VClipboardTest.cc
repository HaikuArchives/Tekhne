/***************************************************************************
 *            VClipboardTest.cc
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

#include "VClipboardTest.h"
#include <iostream>

using namespace std;

void VClipboardTest::setUp() {
}

void VClipboardTest::tearDown() {
}

void VClipboardTest::testLockUnlock() {
	CPPUNIT_ASSERT(v_clipboard->Clear() == V_ERROR);
	CPPUNIT_ASSERT(v_clipboard->Revert() == V_ERROR);
	CPPUNIT_ASSERT(v_clipboard->Commit() == V_ERROR);
	CPPUNIT_ASSERT(!v_clipboard->IsLocked());
	CPPUNIT_ASSERT(v_clipboard->Lock() == V_OK);
	CPPUNIT_ASSERT(v_clipboard->IsLocked());
	CPPUNIT_ASSERT(v_clipboard->Clear() == V_OK);
	CPPUNIT_ASSERT(v_clipboard->Revert() == V_OK);
	CPPUNIT_ASSERT(v_clipboard->Commit() == V_OK);
	v_clipboard->Unlock();
}

void VClipboardTest::testData() {
	CPPUNIT_ASSERT(v_clipboard->Lock() == V_OK);
	VMessage *data = v_clipboard->Data();
	CPPUNIT_ASSERT(data);
	CPPUNIT_ASSERT(v_clipboard->Commit() == V_OK);
	v_clipboard->Unlock();
}
