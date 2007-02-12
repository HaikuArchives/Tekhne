/***************************************************************************
 *            VStorageTest.h
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

#ifndef VSTORAGETEST_H_
#define VSTORAGETEST_H_

#include "tekhne.h"
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

using namespace tekhne;

class VPathTest : public CppUnit::TestFixture {
private:
public:
	static CppUnit::Test *suite() {
		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "VPathTest" );
		suiteOfTests->addTest( new CppUnit::TestCaller<VPathTest>("testCreate",
							   &VPathTest::testCreate ) );
		suiteOfTests->addTest( new CppUnit::TestCaller<VPathTest>("testAppend",
							   &VPathTest::testAppend ) );
		suiteOfTests->addTest( new CppUnit::TestCaller<VPathTest>("testGetParent",
							   &VPathTest::testGetParent ) );
		suiteOfTests->addTest( new CppUnit::TestCaller<VPathTest>("testSetTo",
							   &VPathTest::testSetTo ) );
		suiteOfTests->addTest( new CppUnit::TestCaller<VPathTest>("testFlatten",
							   &VPathTest::testFlatten ) );
		suiteOfTests->addTest( new CppUnit::TestCaller<VPathTest>("testAssignment",
							   &VPathTest::testAssignment ) );
		suiteOfTests->addTest( new CppUnit::TestCaller<VPathTest>("testBoolean",
							   &VPathTest::testBoolean ) );
		return suiteOfTests;
	}

	void setUp();
	void tearDown();

	void testCreate();
	void testAppend();
	void testGetParent();
	void testSetTo();
	void testFlatten();
	void testAssignment();
	void testBoolean();
};

class VEntryTest : public CppUnit::TestFixture {
	private:
	public:
		static CppUnit::Test *suite() {
			CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "VEntryTest" );
			suiteOfTests->addTest( new CppUnit::TestCaller<VEntryTest>("testCreate",
								   &VEntryTest::testCreate ) );
			suiteOfTests->addTest( new CppUnit::TestCaller<VEntryTest>("testStatable",
								   &VEntryTest::testStatable ) );
			suiteOfTests->addTest( new CppUnit::TestCaller<VEntryTest>("testPathOps",
								   &VEntryTest::testPathOps ) );
			suiteOfTests->addTest( new CppUnit::TestCaller<VEntryTest>("testFileOps",
								   &VEntryTest::testFileOps ) );
			suiteOfTests->addTest( new CppUnit::TestCaller<VEntryTest>("testOperator",
								   &VEntryTest::testOperator ) );
			return suiteOfTests;
		}

		void setUp();
		void tearDown();

		void testCreate();
		void testStatable();
		void testPathOps();
		void testFileOps();
		void testOperator();
};

class VDirectoryTest : public CppUnit::TestFixture {
	private:
	public:
		static CppUnit::Test *suite() {
			CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "VDirectoryTest" );
			suiteOfTests->addTest( new CppUnit::TestCaller<VDirectoryTest>("testCreate",
								   &VDirectoryTest::testCreate ) );
			return suiteOfTests;
		}

		void setUp();
		void tearDown();

		void testCreate();
};

class VFileTest : public CppUnit::TestFixture {
	private:
	public:
		static CppUnit::Test *suite() {
			CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "VFileTest" );
			suiteOfTests->addTest( new CppUnit::TestCaller<VFileTest>("testCreate",
								   &VFileTest::testCreate ) );
			return suiteOfTests;
		}

		void setUp();
		void tearDown();

		void testCreate();
};

#endif /*VSTORAGETEST_H_*/
