/***************************************************************************
 *            VRosterTest.cc
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

#include "VRosterTest.h"
#include <iostream>

using namespace std;

void VRosterTest::setUp() {
}

void VRosterTest::tearDown() {
}
void VRosterTest::testIsRunning() {
	// give the registration process a chance to finish
	usleep(1000);
	CPPUNIT_ASSERT(v_roster->IsRunning("app/x-test-app"));
	CPPUNIT_ASSERT(v_roster->IsRunning("app/x-baldmountain-roster"));
	CPPUNIT_ASSERT(v_roster->IsRunning("app/x-test-app"));
	CPPUNIT_ASSERT(v_roster->IsRunning("app/x-baldmountain-roster"));
	CPPUNIT_ASSERT(v_roster->IsRunning("app/x-test-app"));
}

void VRosterTest::testGetAppInfo() {
	app_info ai;

	CPPUNIT_ASSERT(v_roster->GetAppInfo("app/x-test-app", &ai) == V_OK);
	CPPUNIT_ASSERT(ai.team > 0);
	CPPUNIT_ASSERT(ai.thread > 0);
	CPPUNIT_ASSERT(ai.flags == 0);
	CPPUNIT_ASSERT(strcmp(ai.signature,"app/x-test-app") == 0);
	CPPUNIT_ASSERT(v_roster->GetAppInfo("app/x-baldmountain-roster", &ai) == V_OK);
	CPPUNIT_ASSERT(ai.team > 0);
	CPPUNIT_ASSERT(ai.thread > 0);
	CPPUNIT_ASSERT(ai.flags == 0);
	CPPUNIT_ASSERT(strcmp(ai.signature,"app/x-baldmountain-roster") == 0);
}

void VRosterTest::testTeamFor() {
	CPPUNIT_ASSERT(v_roster->TeamFor("app/x-test-app") > 0);
	CPPUNIT_ASSERT(v_roster->TeamFor("app/x-baldmountain-roster") > 0);
}
