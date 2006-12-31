/***************************************************************************
 *            VLocker.cc
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

using namespace tekhne;

VLocker::VLocker(void) : _name(0), _locking_thread(0) {
	
	pthread_mutexattr_init(&_attr);
	pthread_mutexattr_settype(&_attr, PTHREAD_MUTEX_RECURSIVE_NP);
	pthread_mutex_init(&_mutex, &_attr);
}

VLocker::VLocker(const char *name) : _name(name), _locking_thread(0) {
	_name = name;
	pthread_mutexattr_init(&_attr);
	pthread_mutexattr_settype(&_attr, PTHREAD_MUTEX_RECURSIVE_NP);
	pthread_mutex_init(&_mutex, &_attr);
}

VLocker::~VLocker() {
	pthread_mutex_destroy(&_mutex);
	pthread_mutexattr_destroy(&_attr);
}

bool VLocker::Lock(void) {
	pthread_mutex_lock(&_mutex);
	_locking_thread = pthread_self();
	return true;
}

status_t VLocker::LockWithTimeout(bigtime_t timeout) {
	_locking_thread = 0;
	pthread_mutex_lock(&_mutex);
	return V_OK;
}

void VLocker::Unlock(void) {
	pthread_mutex_unlock(&_mutex);
}


thread_t VLocker::LockingThread(void) const {
	return _locking_thread;
}

bool VLocker::IsLocked(void) const {
	return _mutex.__data.__count > 0;
}

int32_t VLocker::CountLocks(void) const {
	return _mutex.__data.__count;
}
