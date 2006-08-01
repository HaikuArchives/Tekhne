/***************************************************************************
 *            VMallocIOTest.cc
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

#include "VMallocIOTest.h"

#include <iostream>

void VMallocIOTest::setUp() {
	mio = new VMallocIO();
	buf = malloc(38);
	memio = new VMemoryIO(buf, 38);
}

void VMallocIOTest::tearDown() {
	delete mio;
	delete memio;
	free(buf);
}

void VMallocIOTest::testMallocIO() {
	CPPUNIT_ASSERT(mio->Buffer() != NULL);
	CPPUNIT_ASSERT(mio->BufferLength() == 256);

	char *str = "another one";
	int len = strlen(str)+1;
	char buf2[len];
	bzero(buf2, len);

	VString s("A longer string");
	char buf[s.Length()];
	bzero(buf, s.Length());

	mio->Write(str, len);
	mio->Write(s.String(), s.Length());

	CPPUNIT_ASSERT(mio->Position() == len+s.Length());

	mio->Seek(0, SEEK_SET);

	CPPUNIT_ASSERT(mio->Read(buf2, len) == len);
	CPPUNIT_ASSERT(mio->Read(buf, s.Length()) == s.Length());

	CPPUNIT_ASSERT(memcmp(buf, s.String(), s.Length()) == 0);
	CPPUNIT_ASSERT(memcmp(buf2, str, len) == 0);
}

void VMallocIOTest::testMemoryIO() {
	CPPUNIT_ASSERT(memio->Buffer() != NULL);
	CPPUNIT_ASSERT(memio->BufferLength() == 38);

	char *str = "another one";
	int len = strlen(str)+1;
	char buf2[len];
	bzero(buf2, len);

	VString s("A longer string");
	char buf[s.Length()];
	bzero(buf, s.Length());

	CPPUNIT_ASSERT(memio->Write(str, len) == len);
	CPPUNIT_ASSERT(memio->Write(s.String(), s.Length()) == s.Length());

	CPPUNIT_ASSERT(memio->Position() == len+s.Length());

	memio->Seek(0, SEEK_SET);

	CPPUNIT_ASSERT(memio->Read(buf2, len) == len);
	CPPUNIT_ASSERT(memio->Read(buf, s.Length()) == s.Length());

	CPPUNIT_ASSERT(memcmp(buf, s.String(), s.Length()) == 0);
	CPPUNIT_ASSERT(memcmp(buf2, str, len) == 0);
	
	const char*constBuf = "A buffer";
	VMemoryIO vmem(constBuf, strlen(constBuf));
	CPPUNIT_ASSERT(vmem.Write("aaa", 3) == V_NOT_ALLOWED);

	void *variableBuf = malloc(8);
	VMemoryIO vmem2(variableBuf, 8);
	CPPUNIT_ASSERT(vmem2.BufferLength() == 8);
	CPPUNIT_ASSERT(vmem2.Position() == 0);
	CPPUNIT_ASSERT(vmem2.Write("A buffer ", 9) == 8);
	CPPUNIT_ASSERT(vmem2.Position() == 8);
	free(variableBuf);
}
