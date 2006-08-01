/***************************************************************************
 *            VMessageTest.cc
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

#include "VMessageTest.h"

#include <iostream>
using namespace std;

void VMessageTest::setUp() {
}

void VMessageTest::tearDown() {
}

void VMessageTest::testCreate() {
	VMessage *msg = new VMessage(0);
	delete msg;
}

void VMessageTest::testAdd() {
	VMessage *msg = new VMessage(0);
	msg->AddString("bar", "fann");
	VString s;
	CPPUNIT_ASSERT(msg->FindString("bar", &s) == V_OK);
	const char *str = s.String();
	CPPUNIT_ASSERT( strlen(str) == 4 );
	CPPUNIT_ASSERT( strcmp(str, "fann") == 0 );

	msg->AddString("foo", VString("desk"));
	CPPUNIT_ASSERT(msg->FindString("foo", &s) == V_OK);
	str = s.String();
	CPPUNIT_ASSERT( strlen(str) == 4 );
	CPPUNIT_ASSERT( strcmp(str, "desk") == 0 );

	msg->AddBool("foo", true);
	bool x = false;
	CPPUNIT_ASSERT(msg->FindBool("foo", &x) == V_OK);
	CPPUNIT_ASSERT( x == true );
	CPPUNIT_ASSERT(msg->ReplaceBool("foo", false) == V_OK);
	CPPUNIT_ASSERT(msg->FindBool("foo", &x) == V_OK);
	CPPUNIT_ASSERT( x == false );

	msg->AddInt32("clip", 456);
	int32_t i = 0;
	CPPUNIT_ASSERT(msg->FindInt32("clip", &i) == V_OK);
	CPPUNIT_ASSERT( i == 456 );
	CPPUNIT_ASSERT(msg->ReplaceInt32("clip", 18) == V_OK);
	CPPUNIT_ASSERT(msg->FindInt32("clip", &i) == V_OK);
	CPPUNIT_ASSERT( i == 18 );

	CPPUNIT_ASSERT(msg->ReplaceInt32("clip", 4, 44) == V_NAME_NOT_FOUND);
	CPPUNIT_ASSERT(msg->FindInt32("clip", &i) == V_OK);
	CPPUNIT_ASSERT( i == 18 );
	delete msg;
}
