/***************************************************************************
 *            VMessenger.h
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

#ifndef _VMESSENGER_H
#define _VMESSENGER_H

#include "StandardDefs.h"
#include "VErrors.h"
#include "VString.h"

namespace tekhne {

class VHandler;
class VLooper;
class VMessage;
class VMallocIO;

class VMessenger {
private:
	VHandler *_handler;
	VLooper *_looper;
	bool _localTarget;
	bool _isValid;
	VString _signature;

public:
	VMessenger(const VHandler *handler, const VLooper *looper = 0, status_t *error = 0);
	VMessenger(const char *signature, team_t team = 0, status_t *error = 0);
	VMessenger(const VMessenger &messenger);
	VMessenger(void);

	~VMessenger();

	bool IsValid(void) const;

	bool LockTarget(void) const;
	status_t LockTargetWithTimeout(bigtime_t timeout) const;

	status_t SendMessage(VMessage *message, VMessage *reply, bigtime_t deliveryTimeout = V_INFINITE_TIMEOUT, bigtime_t replyTimeout = V_INFINITE_TIMEOUT) const;
	status_t SendMessage(VMessage *message, VHandler *replyHandler = 0, bigtime_t deliveryTimeout = V_INFINITE_TIMEOUT) const;
	status_t SendMessage(VMessage *message, VMessenger *replyMessenger, bigtime_t deliveryTimeout = V_INFINITE_TIMEOUT) const;
	status_t SendMessage(int32_t command, VMessage *reply) const;
	status_t SendMessage(int32_t command, VHandler *replyHandler = 0) const;

	VHandler *Target(VLooper **looper) const;
	bool IsTargetLocal(void) const;
	inline team_t Team(void) const;

	VMessenger &operator =(const VMessenger& v);
	bool operator ==(const VMessenger& v) const;
};

}
#endif /* _VMESSENGER_H */
