/***************************************************************************
 *            VMessageQueue.h
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

#include "VLooper.h"
#include "VLocker.h"

#ifndef _VMESSAGEQUEUE_H
#define _VMESSAGEQUEUE_H

#include <semaphore.h>

namespace tekhne {

class VMessage;

class VMessageQueue {
private:
	VList _list;
	VLocker _lock;
	sem_t _sem;

public:
	VMessageQueue(void);

	virtual ~VMessageQueue();

	void AddMessage(VMessage *message);
	void RemoveMessage(VMessage *message);

	int32_t CountMessages(void) const;
	bool IsEmpty(void) const;

	VMessage *FindMessage(int32_t index) const;
	VMessage *FindMessage(int32_t what, int32_t index = 0) const;

	bool Lock(void);
	status_t LockWithTimeout(bigtime_t timeout);
	void Unlock(void);

	VMessage *NextMessage(void);

	friend thread_t VLooper::LockingThread(void) const;
	friend bool VLooper::IsLocked(void) const;
	friend int32_t VLooper::CountLocks(void) const;
};

}

#endif /* _VMESSAGEQUEUE_H */
