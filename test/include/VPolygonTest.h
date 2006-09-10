/***************************************************************************
 *            VPolygonTest.h
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

#ifndef _VPOLOYGONTEST_H
#define _VPOLOYGONTEST_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "tekhne.h"

using namespace tekhne;

class VPolygonTest : public CppUnit::TestFixture {
	private:
	public:
		static CppUnit::Test *VPolygonTest::suite() {
			CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "VPolygonTest" );
			suiteOfTests->addTest( new CppUnit::TestCaller<VPolygonTest>("testCreate",
								   &VPolygonTest::testCreate ) );
			suiteOfTests->addTest( new CppUnit::TestCaller<VPolygonTest>("testFrame",
								   &VPolygonTest::testFrame ) );
			suiteOfTests->addTest( new CppUnit::TestCaller<VPolygonTest>("testMapTo",
								   &VPolygonTest::testMapTo ) );
			return suiteOfTests;
		}

		void setUp();
		void tearDown();

		void testCreate();
		void testFrame();
		void testMapTo();
};

class VRegionTest : public CppUnit::TestFixture {
	private:
	public:
		static CppUnit::Test *VRegionTest::suite() {
			CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "VRegionTest" );
			suiteOfTests->addTest( new CppUnit::TestCaller<VRegionTest>("testCreate",
								   &VRegionTest::testCreate ) );
			return suiteOfTests;
		}

		void setUp();
		void tearDown();

		void testCreate();
};

class VShapeTest : public CppUnit::TestFixture {
	private:
	public:
		static CppUnit::Test *VShapeTest::suite() {
			CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "VShapeTest" );
			suiteOfTests->addTest( new CppUnit::TestCaller<VShapeTest>("testCreate",
								   &VShapeTest::testCreate ) );
			suiteOfTests->addTest( new CppUnit::TestCaller<VShapeTest>("testIterator",
								   &VShapeTest::testIterator ) );
			return suiteOfTests;
		}

		void setUp();
		void tearDown();

		void testCreate();
		void testIterator();
};

#endif /* _VPOLOYGONTEST_H */
