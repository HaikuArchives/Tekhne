/***************************************************************************
 *            VLocker.h
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

#ifndef _VLOCKER_H
#define _VLOCKER_H

#include "VString.h"
#include <pthread.h>

namespace tekhne {
class VLocker {
private:
	VString _name;
	thread_t _locking_thread;
	pthread_mutexattr_t _attr; // recursive mutex attribute
	pthread_mutex_t _mutex; // recursive mutex
public:

	VLocker(void);
	VLocker(const char *name);

	virtual ~VLocker();

	bool Lock(void);
	status_t LockWithTimeout(bigtime_t timeout);
	void Unlock(void);

	thread_t LockingThread(void) const;
	bool IsLocked(void) const;
	int32_t CountLocks(void) const;
};
}

#endif /* _VLOCKER_H */
