/***************************************************************************
 *            VPointTest.h
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

#ifndef _VPOINTTEST_H
#define _VPOINTTEST_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "tekhne.h"

using namespace tekhne;

class VPointTest : public CppUnit::TestFixture {
private:
	VPoint *p;
public:
	static CppUnit::Test *suite() {
		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "VPointTest" );
		suiteOfTests->addTest( new CppUnit::TestCaller<VPointTest>("testCreate",
									&VPointTest::testCreate ) );
		suiteOfTests->addTest( new CppUnit::TestCaller<VPointTest>("testConstrainTo",
									&VPointTest::testConstrainTo ) );
		return suiteOfTests;
	}

	void setUp();
	void tearDown();

	void testCreate();
	void testConstrainTo();
};

class VRectTest : public CppUnit::TestFixture {
private:
	VRect *r;
public:
	static CppUnit::Test *suite() {
		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "VRectTest" );
		suiteOfTests->addTest( new CppUnit::TestCaller<VRectTest>("testCreate",
									&VRectTest::testCreate ) );
		suiteOfTests->addTest( new CppUnit::TestCaller<VRectTest>("testContains",
									&VRectTest::testContains ) );
		suiteOfTests->addTest( new CppUnit::TestCaller<VRectTest>("testSetPoint",
									&VRectTest::testSetPoint ) );
		suiteOfTests->addTest( new CppUnit::TestCaller<VRectTest>("testOperators",
									&VRectTest::testOperators ) );
		return suiteOfTests;
	}

	void setUp();
	void tearDown();

	void testCreate();
	void testContains();
	void testSetPoint();
	void testOperators();
};

#endif /* _VPOINTTEST_H */
