/***************************************************************************
 *            main.cc
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

#include "VListTest.h"
#include "VTListTest.h"
#include "VStringTest.h"
#include "VBlockCacheTest.h"
#include "VMallocIOTest.h"
#include "VPointTest.h"
#include "ThreadTest.h"
#include "VMessageTest.h"
#include "VMessengerTest.h"

#include "VApplication.h"

#include <cppunit/ui/text/TestRunner.h>

#include <string>
#include <iostream>

using namespace tekhne;

class VTestApplication : public VApplication {
private:
public:
	VTestApplication() : VApplication("app/v-test-app") {
		// send pulse every half a second, we quit on the first one
		SetPulseRate(500000);
	}

	virtual void ReadyToRun(void) {
		VStopWatch sw("me");
		std::cout << "ReadyToRun" << std::endl;
		std::cout << "IsLaunching " << (IsLaunching()?"true":"false") << std::endl;

		CppUnit::TextUi::TestRunner runner;
		runner.addTest( VListTest::suite() );
		runner.addTest( VTListTest::suite() );
		runner.addTest( VStringTest::suite() );
		runner.addTest( VBlockCacheTest::suite() );
		runner.addTest( VMallocIOTest::suite() );
		runner.addTest( VPointTest::suite() );
		runner.addTest( VRectTest::suite() );
		runner.addTest( ThreadTest::suite() );
		runner.addTest( VMessageTest::suite() );
		runner.addTest( VMessengerTest::suite() );
		runner.run();
	}

	virtual void Pulse(void) {
		std::cout << "Pulse" << std::endl;
		std::cout << "IsLaunching " << (IsLaunching()?"true":"false") << std::endl;
		Quit();
	}

};

int main (int argc, char *argv[]) {

	VTestApplication *ta = new VTestApplication();
	ta->Run();

	delete ta;
	return 0;
}
