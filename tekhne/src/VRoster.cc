/***************************************************************************
 *            VArchivable.cc
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

#include "tekhne.h"
#include "IRoster.h"

#include <iostream>
using namespace std;

using namespace tekhne;

namespace tekhne {
const char *V_ROSTER_THREAD = "_thread";
const char *V_ROSTER_TEAM = "_team";
const char *V_ROSTER_FLAGS = "_flags";
const char *V_ROSTER_SIGNATURE = "_sig";
const char *V_ROSTER_REPLY = "_reply";
}

VRoster::VRoster(void) {
	_rosterMgr = new VMessenger("app/x-baldmountain-roster");
	VString s = v_app->Signature();
	if (s.Length()) {
		app_info ai;
		VMessage msg(V_ROSTER_REGISTER);
		msg.AddString(V_ROSTER_SIGNATURE, v_app->Signature());
		v_app->GetAppInfo(&ai);
		msg.AddUInt32(V_ROSTER_TEAM, ai.team);
		_rosterMgr->SendMessage(&msg, static_cast<VHandler*>(0));
	}
}

VRoster::~VRoster() {
	VString s = v_app->Signature();
	if (s.Length()) {
		VMessage msg(V_ROSTER_UNREGISTER);
		msg.AddString(V_ROSTER_SIGNATURE, v_app->Signature());
		_rosterMgr->SendMessage(&msg, static_cast<VHandler*>(0));
	}
	delete _rosterMgr;
}

status_t VRoster::Broadcast(VMessage *message) const {
	return V_ERROR;
}

status_t VRoster::Broadcast(VMessage *message, VMessenger reply_to) const {
	return V_ERROR;
}

status_t VRoster::GetAppInfo(const char *signature, app_info *appInfo) const {
	VMessage msg(V_ROSTER_APP_INFO);
	VMessage reply;
	msg.AddString(V_ROSTER_SIGNATURE, signature);
	_rosterMgr->SendMessage(&msg, &reply);
	int32_t err = V_NAME_NOT_FOUND;
	reply.FindInt32(V_ROSTER_REPLY, &err);
	if (err == V_OK) {
		if (appInfo) {
			uint32_t ui;
			VString s;
			reply.FindUInt32(V_ROSTER_TEAM, &ui);
			appInfo->team = ui;
			reply.FindUInt32(V_ROSTER_THREAD, &ui);
			appInfo->thread = ui;
			reply.FindUInt32(V_ROSTER_FLAGS, &ui);
			appInfo->flags = ui;
			reply.FindString(V_ROSTER_SIGNATURE, &s);
			s.CopyInto(appInfo->signature, 0, s.Length());
		}
	}
	return err;
}

status_t VRoster::GetRunningAppInfo(team_t team, app_info *appInfo) const {
	return V_ERROR;
}

status_t VRoster::GetActiveAppInfo(app_info *appInfo) const {
	return V_ERROR;
}

void VRoster::GetAppList(VList *teams) const {
}

void VRoster::GetAppList(const char *signature, VList *teams) const {
}

status_t VRoster::StartWatching(VMessenger target, uint32_t events) const {
	return V_ERROR;
}

status_t VRoster::StopWatching(VMessenger target) const {
	return V_ERROR;
}

team_t VRoster::TeamFor(const char *signature) const {
	VMessage msg(V_ROSTER_TEAM_FOR);
	VMessage reply;
	msg.AddString(V_ROSTER_SIGNATURE, signature);
	_rosterMgr->SendMessage(&msg, &reply);
	uint32_t ans = 0;
	reply.FindUInt32(V_ROSTER_REPLY, &ans);
	return ans;
}

bool VRoster::IsRunning(const char *signature) const {
	VMessage msg(V_ROSTER_IS_RUNNING);
	VMessage reply;
	msg.AddString(V_ROSTER_SIGNATURE, signature);
	_rosterMgr->SendMessage(&msg, &reply);
	bool ans = false;
	reply.FindBool(V_ROSTER_REPLY, &ans);
	return ans;
}
