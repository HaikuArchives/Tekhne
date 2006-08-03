/***************************************************************************
 *            VLooper.h
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

#ifndef _VLOOPER_H
#define _VLOOPER_H

#include "VHandler.h"
#include "VMessage.h"

namespace tekhne {
	
class VMessageQueue;

void *looper_thread_func(void *l);

class VLooper : public VHandler  {
protected:
	bool _quitting;
	char *_name;
private:
	VMessage *_currentMessage;
	VList _handlers;
	VMessageQueue *_mq;
	pthread_t _thread;
	pthread_attr_t _attr;
public:
	VLooper(const char *name = 0, int32_t priority = V_NORMAL_PRIORITY, int32_t portCapacity = V_LOOPER_PORT_DEFAULT_CAPACITY);
	VLooper(VMessage *archive);
	virtual ~VLooper();

	static VLooper *LooperForThread(thread_t thread);
	
	virtual void AddCommonFilter(VMessageFilter *filter);
	virtual bool RemoveCommonFilter(VMessageFilter *filter);
	virtual void SetCommonFilterList(VList *filters);
	VList *CommonFilterList(void) const;

	void AddHandler(VHandler *handler);
	bool RemoveHandler(VHandler *handler);
	VHandler *HandlerAt(int32_t index) const;
	int32_t CountHandlers(void) const;
	int32_t IndexOf(VHandler *handler) const;

	VMessage *CurrentMessage(void) const;
	VMessage *DetachCurrentMessage(void);

	virtual void DispatchMessage(VMessage *message, VHandler *target);
	
	bool Lock(void);
	status_t LockWithTimeout(bigtime_t timeout);
	void Unlock(void);
	
	thread_t LockingThread(void) const;
	bool IsLocked(void) const;
	int32_t CountLocks(void) const;
	
	virtual void MessageReceived(VMessage *message);
	
	VMessageQueue *MessageQueue(void) const;
	
	status_t PostMessage(VMessage *message);
	status_t PostMessage(uint32_t command);
	status_t PostMessage(VMessage *message, VHandler *handler, VHandler *replyHandler = 0);
	status_t PostMessage(uint32_t command, VHandler *handler, VHandler *replyHandler = 0);

	virtual void Quit(void);
	
	virtual bool QuitRequested(void);
	
	virtual thread_t Run(void);
	
	void SetPreferredHandler(VHandler *handler) const;
	VHandler *PreferredHandler(void);
	
	thread_t Thread(void) const;
	team_t Team(void) const;
	
	friend void *looper_thread_func(void *l);

	// VArchivable methods
	static VArchivable *Instantiate(VMessage *archive);
	virtual status_t Archive(VMessage *archive, bool deep = true) const;
};

void *looper_thread_func(void *l);


}
#endif /* _VLOOPER_H */
