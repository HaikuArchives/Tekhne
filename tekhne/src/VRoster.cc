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

VRoster::VRoster(void) {
	_rosterMgr = new VMessenger("app/x-baldmountain-roster");
	VString s = v_app->Signature();
	if (s.Length()) {
		VMessage *msg = new VMessage(V_ROSTER_REGISTER);
		msg->AddString("_signature", v_app->Signature());
		_rosterMgr->SendMessage(msg, static_cast<VHandler*>(0));
		delete msg;
	}
}

VRoster::~VRoster() {
	VString s = v_app->Signature();
	if (s.Length()) {
		VMessage *msg = new VMessage(V_ROSTER_UNREGISTER);
		msg->AddString("_signature", v_app->Signature());
		_rosterMgr->SendMessage(msg, static_cast<VHandler*>(0));
		delete msg;
	}
	delete _rosterMgr;
}

void VRoster::GetRecentDocuments(VMessage *refList, int32_t maxCount, const char *ofType,
								 const char *openedByAppSig) const {
}
void VRoster::GetRecentDocuments(VMessage *refList, int32_t maxCount, const char *ofTypeList[],
							int32_t ofTypeListCount, const char *openedByAppSig) const {
}

void VRoster::GetRecentFolders(VMessage *refList, int32_t maxCount, const char *openedByAppSig) const {
}

status_t VRoster::Broadcast(VMessage *message) const {
	return V_ERROR;
}

status_t VRoster::Broadcast(VMessage *message, VMessenger reply_to) const {
	return V_ERROR;
}

status_t VRoster::GetAppInfo(const char *signature, app_info *appInfo) const {
	return V_ERROR;
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

void VRoster::GetRecentApps(VMessage *refList, int32_t maxCount) const {
}

status_t VRoster::Launch(const char *type, VMessage *message, team_t *team) const {
	return V_ERROR;
}

status_t VRoster::Launch(const char *type, VList *messages, team_t *team) const {
	return V_ERROR;
}

status_t VRoster::Launch(const char *type, int argc, char **argv, team_t *) const {
	return V_ERROR;
}

status_t VRoster::StartWatching(VMessenger target, uint32_t events) const {
	return V_ERROR;
}

status_t VRoster::StopWatching(VMessenger target) const {
	return V_ERROR;
}

team_t VRoster::TeamFor(const char *signature) const {
	return -1;
}

bool VRoster::IsRunning(const char *signature) const {
	return false;
}