/***************************************************************************
 *            VRoster.h
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

#ifndef _VROSTER_H
#define _VROSTER_H

#include "VMessenger.h"

namespace tekhne {

class VMessage;
class VList;

class VRoster {
private:
	VMessenger *_rosterMgr;
	VRoster(void);
	~VRoster();
public:

//	void AddToRecentDocuments(const entry_ref *document, const char *appSig = 0) const;
	void GetRecentDocuments(VMessage *refList, int32_t maxCount, const char *ofType = 0,
							const char *openedByAppSig = 0) const;
	void GetRecentDocuments(VMessage *refList, int32_t maxCount, const char *ofTypeList[] = 0,
							int32_t ofTypeListCount = 0, const char *openedByAppSig = 0) const;

//	void AddToRecentFolders(const entry_ref *folder, const char *appSig = 0) const;
	void GetRecentFolders(VMessage *refList, int32_t maxCount, const char *openedByAppSig = 0) const;

	status_t Broadcast(VMessage *message) const;
	status_t Broadcast(VMessage *message, VMessenger reply_to) const;

//	status_t FindApp(const char *type, entry_ref *app) const;
// 	status_t FindApp(entry_ref *file, entry_ref *app) const;

	status_t GetAppInfo(const char *signature, app_info *appInfo) const;
//	status_t GetAppInfo(entry_ref *executable, app_info *appInfo) const;
	status_t GetRunningAppInfo(team_t team, app_info *appInfo) const;
	status_t GetActiveAppInfo(app_info *appInfo) const;

	void GetAppList(VList *teams) const;
	void GetAppList(const char *signature, VList *teams) const;

	void GetRecentApps(VMessage *refList, int32_t maxCount) const;

	status_t Launch(const char *type, VMessage *message = 0, team_t *team = 0) const;
	status_t Launch(const char *type, VList *messages, team_t *team = 0) const;
	status_t Launch(const char *type, int argc, char **argv, team_t *team = 0) const;
//	status_t Launch(const entry_ref *file, const BMessage *message = 0, team_t *team = 0) const;
//	status_t Launch(const entry_ref *file, const BList *messages, team_t *team = 0) const;
//	status_t Launch(const entry_ref *file, int argc, const char * const char *argv,
//					team_t *team = 0) const;

	status_t StartWatching(VMessenger target, uint32_t events = V_REQUEST_LAUNCHED | V_REQUEST_QUIT) const;
	status_t StopWatching(VMessenger target) const;
	team_t TeamFor(const char *signature) const;
	//team_t TeamFor(entry_ref *executable) const;
	bool IsRunning(const char *signature) const;
	//bool IsRunning(entry_ref *executable) const;

	friend class VApplication;
};


}

#endif /* _VROSTER_H */
