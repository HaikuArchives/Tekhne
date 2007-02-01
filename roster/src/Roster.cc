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
#include "IRoster.h"
#include <iostream>

using namespace tekhne;
using namespace std;

const char *V_ROSTER_THREAD = "_thread";
const char *V_ROSTER_TEAM = "_team";
const char *V_ROSTER_FLAGS = "_flags";
const char *V_ROSTER_SIGNATURE = "_sig";
const char *V_ROSTER_REPLY = "_reply";

Roster::Roster() : VApplication("app/x-baldmountain-roster") {
}

Roster::~Roster() {
}

void Roster::ReadyToRun(void) {
	if (v_clipboard->Lock()) {
		v_clipboard->Clear();
		VMessage *clip = v_clipboard->Data();
		if (clip) {
			char *asciiText = "app/x-baldmountain-roster";
			clip->AddString("text/plain", asciiText);
			v_clipboard->Commit();
		}
		v_clipboard->Unlock();
	}
}

ApplicationInfo *Roster::FindAppBySignature(VString& sig) {
	VListIterator iter(_runningApps);
	while(iter.HasNext()) {
		ApplicationInfo *ai = static_cast<ApplicationInfo*>(iter.Next());
		if (ai->IsMe(sig)) return ai;
	}
	return 0;
}


void Roster::MessageReceived(VMessage *message) {
	VString sig;
	ApplicationInfo *ai;
	VMessage reply;
	uint32_t ui;
	switch(message->what) {
		case V_ROSTER_REGISTER:
			message->FindString(V_ROSTER_SIGNATURE, &sig);
			message->FindUInt32(V_ROSTER_TEAM, &ui);
			cout << "register " << sig.String() << " " << ui << endl;
			ai = FindAppBySignature(sig);
			if (!ai) {
				_runningApps.AddItem(new ApplicationInfo(sig, ui));
			}
			break;
		case V_ROSTER_UNREGISTER:
			message->FindString(V_ROSTER_SIGNATURE, &sig);
			cout << "unregister " << sig.String() << endl;
			ai = FindAppBySignature(sig);
			if (ai) {
				_runningApps.RemoveItem(ai);
				delete ai;
			}
			break;
		case V_ROSTER_ACTIVATE:
			break;
		case V_ROSTER_TEAM_FOR:
			message->FindString(V_ROSTER_SIGNATURE, &sig);
			cout << "TeamFor: " << sig.String() << endl;
			reply.what = V_ROSTER_TEAM_FOR;
			ai = FindAppBySignature(sig);
			if (ai) {
				reply.AddUInt32(V_ROSTER_REPLY, ai->Team());
			} else {
				reply.AddUInt32(V_ROSTER_REPLY, 0);
			}
			message->SendReply(&reply);
			break;
		case V_ROSTER_IS_RUNNING:
			message->FindString(V_ROSTER_SIGNATURE, &sig);
			cout << "IsRunning: " << sig.String() << endl;
			ai = FindAppBySignature(sig);
			reply.what = V_ROSTER_IS_RUNNING;
			reply.AddBool(V_ROSTER_REPLY, ai != 0);
			message->SendReply(&reply);
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
			message->FindString(V_ROSTER_SIGNATURE, &sig);
			cout << "GetAppInfo: " << sig.String() << endl;
			reply.what = V_ROSTER_TEAM_FOR;
			ai = FindAppBySignature(sig);
			if (ai) {
				reply.AddUInt32(V_ROSTER_THREAD, ai->Thread());
				reply.AddUInt32(V_ROSTER_TEAM, ai->Team());
				reply.AddUInt32(V_ROSTER_FLAGS, ai->Flags());
				reply.AddString(V_ROSTER_SIGNATURE, ai->Signature());
				reply.AddInt32(V_ROSTER_REPLY, V_OK);
			} else {
				reply.AddInt32(V_ROSTER_REPLY, V_NAME_NOT_FOUND);
			}
			message->SendReply(&reply);
			break;
		case V_ROSTER_BROADCAST:
			break;
	}
}
