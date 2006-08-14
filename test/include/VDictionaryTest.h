/***************************************************************************
 *            VDictionaryTest.h
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

#ifndef VDICTIONARYTEST_H_
#define VDICTIONARYTEST_H_

#include "tekhne.h"
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

using namespace tekhne;

class VDictionaryTest : public CppUnit::TestFixture {
	private:
		VDictionary *bc;
	public:
		static CppUnit::Test *VDictionaryTest::suite() {
			CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "VDictionaryTest" );
			suiteOfTests->addTest( new CppUnit::TestCaller<VDictionaryTest>("testCreate",
								   &VDictionaryTest::testCreate ) );
			return suiteOfTests;
		}

		void setUp();
		void tearDown();

		void testCreate();
};


#endif /*VDICTIONARYTEST_H_*/
