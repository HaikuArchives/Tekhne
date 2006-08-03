/***************************************************************************
 *            VHandler.h
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

#ifndef _VHANDLER_H
#define _VHANDLER_H

#include "StandardDefs.h"
#include "VArchivable.h"
#include "VErrors.h"
#include "VString.h"
#include "VList.h"

namespace tekhne {

class VMessage;
class VLooper;
class VMessageFilter;
class VMessenger;

class VHandler : public VArchivable {
protected:
	VString _name;
	VLooper *_looper;
private:
	// Archived field
	VHandler *_nextHandler;
	VList *_filterList;
public:
	VHandler(const char *name = 0);
	VHandler(VMessage *archive);

	virtual ~VHandler();

	virtual status_t GetSupportedSuites(VMessage *message);

	bool LockLooper(void);
	status_t LockLooperWithTimeout(bigtime_t timeout);
	void UnlockLooper(void);

	VLooper *Looper(void) const;

	virtual void MessageReceived(VMessage *message);

	virtual VHandler *ResolveSpecifier(VMessage *message, int32_t index, VMessage *specifier, int32_t what, const char *property);

	virtual void SetFilterList(VList *list);
	VList *FilterList(void) const;
	virtual void AddFilter(VMessageFilter *filter);
	virtual bool RemoveFilter(VMessageFilter *filter);
	
	void SetName(const char *string);
	const char *Name(void) const;
	
	void SetNextHandler(VHandler *handler);
	VHandler *NextHandler(void) const;
	
	status_t StartWatching(VMessenger *watcher, uint32_t what);
	status_t StartWatching(VHandler *watcher, uint32_t what);
	status_t StartWatchingAll(VMessenger *watcher);
	status_t StartWatchingAll(VHandler *watcher);
	status_t StopWatching(VMessenger *watcher, uint32_t what);
	status_t StopWatching(VHandler *watcher, uint32_t what);
	status_t StopWatchingAll(VMessenger *watcher);
	status_t StopWatchingAll(VHandler *watcher);
	// VArchivable methods
	static VArchivable *Instantiate(VMessage *archive);
	virtual status_t Archive(VMessage *archive, bool deep = true) const;
};

}
#endif /* _VHANDLER_H */
