/***************************************************************************
 *            VStringTest.h
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

#ifndef VSTRINGTEST_H_
#define VSTRINGTEST_H_

#include "tekhne.h"
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

using namespace tekhne;

class VStringTest : public CppUnit::TestFixture {
private:
	VString *s;
public:
static CppUnit::Test *VStringTest::suite() {
	CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "VStringTest" );
	suiteOfTests->addTest( new CppUnit::TestCaller<VStringTest>("testCreate",
								&VStringTest::testCreate ) );
	suiteOfTests->addTest( new CppUnit::TestCaller<VStringTest>("testAppend",
								&VStringTest::testAppend ) );
	suiteOfTests->addTest( new CppUnit::TestCaller<VStringTest>("testCountChars",
								&VStringTest::testCountChars ) );
	suiteOfTests->addTest( new CppUnit::TestCaller<VStringTest>("testPrepend",
								&VStringTest::testPrepend ) );
	suiteOfTests->addTest( new CppUnit::TestCaller<VStringTest>("testInsert",
								&VStringTest::testInsert ) );
	suiteOfTests->addTest( new CppUnit::TestCaller<VStringTest>("testCharacterEscape",
								&VStringTest::testCharacterEscape ) );
	suiteOfTests->addTest( new CppUnit::TestCaller<VStringTest>("testCompare",
								&VStringTest::testCompare ) );
	suiteOfTests->addTest( new CppUnit::TestCaller<VStringTest>("testCopyInto",
								&VStringTest::testCopyInto ) );
	suiteOfTests->addTest( new CppUnit::TestCaller<VStringTest>("testFind",
								&VStringTest::testFind ) );
	suiteOfTests->addTest( new CppUnit::TestCaller<VStringTest>("testRemove",
								&VStringTest::testRemove ) );
	suiteOfTests->addTest( new CppUnit::TestCaller<VStringTest>("testReplace",
								&VStringTest::testReplace ) );
	suiteOfTests->addTest( new CppUnit::TestCaller<VStringTest>("testSetTo",
								&VStringTest::testSetTo ) );
	suiteOfTests->addTest( new CppUnit::TestCaller<VStringTest>("testAdopt",
								&VStringTest::testAdopt ) );
	suiteOfTests->addTest( new CppUnit::TestCaller<VStringTest>("testSubstring",
								&VStringTest::testSubstring ) );
	suiteOfTests->addTest( new CppUnit::TestCaller<VStringTest>("testOperators",
								&VStringTest::testOperators ) );
	return suiteOfTests;
}

	void setUp();
	void tearDown();

	void testCreate();
	void testAppend();
	void testPrepend();
	void testInsert();
	void testCharacterEscape();
	void testCompare();
	void testCopyInto();
	void testFind();
	void testRemove();
	void testReplace();
	void testSetTo();
	void testAdopt();
	void testCountChars();
	void testSubstring();
	void testOperators();
};


#endif /*VSTRINGTEST_H_*/
