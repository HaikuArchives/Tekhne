/***************************************************************************
 *            Roster.cc
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
 ****************************************************************************/

#include "Roster.h"

using namespace tekhne;

Roster::Roster() : VApplication("app/x-baldmountain-roster") {
}

Roster::~Roster() {
}

void Roster::ReadyToRun(void) {
}

void Roster::MessageReceived(VMessage *message) {
	switch(message->what) {
		case V_ROSTER_REGISTER:
			break;
		case V_ROSTER_ACTIVATE:
			break;
		case V_ROSTER_TEAM_FOR:
			break;
		case V_ROSTER_IS_RUNNING:
			break;
		case V_ROSTER_START_WATCHING:
			break;
		case V_ROSTER_STOP_WATCHING:
			break;
		case V_ROSTER_LAUNCH:
			break;
		case V_ROSTER_APP_LIST:
			break;
		case V_ROSTER_APP_INFO:
			break;
		case V_ROSTER_BROADCAST:
			break;
	}
}
