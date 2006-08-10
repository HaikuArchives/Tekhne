/***************************************************************************
 *            VTListTest.cc
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

#include "VTListTest.h"
#include <string.h>
#include <iostream>

using namespace std;


void VTListTest::setUp() {
	l = new VTList<char *>();
}

void VTListTest::tearDown() {
	delete l;
}

void VTListTest::testAddItem() {
	l->AddItem("foo");
	l->AddItem("bar");
	l->AddItem("baz");
	l->AddItem("ball");
	l->AddItem("far", 67);
	CPPUNIT_ASSERT( l->CountItems() == 5 );
	char **items = l->Items();
	CPPUNIT_ASSERT( strcmp(items[0], "foo") == 0 );
	CPPUNIT_ASSERT( strcmp(items[1], "bar") == 0 );
	CPPUNIT_ASSERT( strcmp(items[2], "baz") == 0 );
	CPPUNIT_ASSERT( strcmp(items[3], "ball") == 0 );
	CPPUNIT_ASSERT( strcmp(items[67], "far") == 0 );
}

static int my_strcmp(const void*a, const void*b) {
	return strcmp(*(const char**)a, *(const char**)b);
}

void VTListTest::testSortItems() {
	l->AddItem("foo");
	l->AddItem("baz");
	l->AddItem("tingle");
	l->AddItem("bar");
	l->AddItem("ball");
	CPPUNIT_ASSERT( l->CountItems() == 5 );
	l->SortItems(my_strcmp);

	char **items = l->Items();
	CPPUNIT_ASSERT( strcmp(items[0], "ball") == 0 );
	CPPUNIT_ASSERT( strcmp(items[1], "bar") == 0 );
	CPPUNIT_ASSERT( strcmp(items[2], "baz") == 0 );
	CPPUNIT_ASSERT( strcmp(items[3], "foo") == 0 );
	CPPUNIT_ASSERT( strcmp(items[4], "tingle") == 0 );
}

void VTListTest::testInsertItem() {
	l->AddItem("ball");
	l->AddItem("foo");
	l->AddItem("bar");
	l->AddItem("baz");
	l->AddItem("tic", 2);
	char **items = l->Items();
	CPPUNIT_ASSERT( strcmp(items[0], "ball") == 0 );
	CPPUNIT_ASSERT( strcmp(items[1], "foo") == 0 );
	CPPUNIT_ASSERT( strcmp(items[2], "tic") == 0 );
	CPPUNIT_ASSERT( strcmp(items[3], "bar") == 0 );
	CPPUNIT_ASSERT( strcmp(items[4], "baz") == 0 );
}

void VTListTest::testOperator() {
	l->AddItem("ball");
	l->AddItem("foo");
	l->AddItem("bar");
	l->AddItem("baz");
	l->AddItem("tic", 4);

	VTList<char*> &nl = *this->l;

	char **items = nl.Items();
	CPPUNIT_ASSERT( strcmp(items[0], "ball") == 0 );
	CPPUNIT_ASSERT( strcmp(items[1], "foo") == 0 );
	CPPUNIT_ASSERT( strcmp(items[2], "bar") == 0 );
	CPPUNIT_ASSERT( strcmp(items[3], "baz") == 0 );
	CPPUNIT_ASSERT( strcmp(items[4], "tic") == 0 );
}
