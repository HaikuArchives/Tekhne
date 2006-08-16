/***************************************************************************
 *            VListTest.h
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

#ifndef VLISTTESTCASE_H_
#define VLISTTESTCASE_H_

#include "tekhne.h"
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

using namespace tekhne;

class VListTest : public CppUnit::TestFixture {
private:
	VList *l;
public:
static CppUnit::Test *VListTest::suite() {
	CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "VListTest" );
	suiteOfTests->addTest( new CppUnit::TestCaller<VListTest>("testAddItem",
								&VListTest::testAddItem ) );
	suiteOfTests->addTest(	new CppUnit::TestCaller<VListTest>("testSortItems",
								&VListTest::testSortItems ) );
	suiteOfTests->addTest(	new CppUnit::TestCaller<VListTest>("testInsertItem",
								&VListTest::testInsertItem ) );
	suiteOfTests->addTest(	new CppUnit::TestCaller<VListTest>("testOperator",
								&VListTest::testOperator ) );
	suiteOfTests->addTest(	new CppUnit::TestCaller<VListTest>("testBigList",
							&VListTest::testBigList ) );
	suiteOfTests->addTest(	new CppUnit::TestCaller<VListTest>("testIterator",
							&VListTest::testIterator ) );
	return suiteOfTests;
}

	void setUp();
	void tearDown();

	void testAddItem();
	void testSortItems();
	void testInsertItem();
	void testOperator();
	void testBigList();
	void testIterator();
};


#endif /*VLISTTESTCASE_H_*/
