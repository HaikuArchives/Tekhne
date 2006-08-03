/***************************************************************************
 *            VAutoLock.h
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

#ifndef _VAUTOLOCK_H
#define _VAUTOLOCK_H

#include "VLocker.h"
#include "VLooper.h"

namespace tekhne {

class VAutoLock {
private:
	VLocker *_locker;
	VLooper *_looper;
public:
	inline VAutoLock(VLooper *looper) : _locker(0), _looper(looper) { if (_looper) { _looper->Lock(); } }
	inline VAutoLock(VLocker *locker) : _locker(locker), _looper(0) { if (_locker) { _locker->Lock(); } }
	inline VAutoLock(VLocker &locker) : _locker(&locker), _looper(0) { if (_locker) { _locker->Lock(); } }
	inline ~VAutoLock()  { if (_looper) { _looper->Unlock(); } else if (_locker) { _locker->Unlock(); } }

	inline bool IsLocked(void) { if (_looper) { return _looper->IsLocked(); } else if (_locker) { return _locker->IsLocked(); } return false; }
};

} // namespace tekhne

#endif /* _VAUTOLOCK_H */
