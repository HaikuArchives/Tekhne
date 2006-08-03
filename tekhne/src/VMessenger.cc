/***************************************************************************
 *            VMessenger.cc
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

VMessenger::VMessenger(const VHandler *handler, const VLooper *looper, status_t *error) {
	_handler = (VHandler *)handler;
	_looper = (VLooper *)looper;
}

VMessenger::VMessenger(const char *signature, team_t team, status_t *error) {
	_handler = 0;
	_looper = 0;
}

VMessenger::VMessenger(const VMessenger &messenger) {
	_handler = 0;
	_looper = 0;
}

VMessenger::VMessenger(void) {
	_handler = 0;
	_looper = 0;
}

VMessenger::~VMessenger() {
}

bool VMessenger::IsValid(void) const {
	return false;
}

bool VMessenger::LockTarget(void) const {
	if (_looper) {
		return _looper->Lock();
	}
	return false;
}

status_t VMessenger::LockTargetWithTimeout(bigtime_t timeout) const {
	if (_looper) {
		return _looper->LockWithTimeout(timeout);
	}
	return V_ERROR;
}

status_t VMessenger::SendMessage(VMessage *message, VMessage *reply, bigtime_t deliveryTimeout, bigtime_t replyTimeout) const {
	status_t err = _looper->LockWithTimeout(deliveryTimeout);
	if (err == V_OK) {
		// not sure what to do with reply, need to fill it in somehow
		err = _looper->PostMessage(message, _handler);
		_looper->Unlock();
	}
	return err;
}

status_t VMessenger::SendMessage(VMessage *message, VHandler *replyHandler, bigtime_t deliveryTimeout) const {
	status_t err = _looper->LockWithTimeout(deliveryTimeout);
	if (err == V_OK) {
		err = _looper->PostMessage(message, _handler, replyHandler);
		_looper->Unlock();
	}
	return err;
}

status_t VMessenger::SendMessage(VMessage *message, VMessenger *replyMessenger, bigtime_t deliveryTimeout) const {
	status_t err = _looper->LockWithTimeout(deliveryTimeout);
	if (err == V_OK) {
		err = _looper->PostMessage(message, _handler, replyMessenger->_handler);
		_looper->Unlock();
	}
	return err;
}

status_t VMessenger::SendMessage(uint32_t command, VMessage *reply) const {
	_looper->Lock();
		// not sure what to do with reply, need to fill it in somehow
	status_t err = _looper->PostMessage(command);
	_looper->Unlock();
	return err;
}

status_t VMessenger::SendMessage(uint32_t command, VHandler *replyHandler) const {
	_looper->Lock();
	status_t err = _looper->PostMessage(command, _handler, replyHandler);
	_looper->Unlock();
	return err;
}

VHandler *VMessenger::Target(VLooper **looper) const {
	*looper = _looper;
	return _handler;
}

bool VMessenger::IsTargetLocal(void) const {
	return true;
}

inline team_t VMessenger::Team(void) const {
	return 0;
}

VMessenger &VMessenger::operator =(const VMessenger& v) {
	if (this != &v) {
		_looper = v._looper;
		_handler = v._handler;
	}
	return *this;
}

bool VMessenger::operator ==(const VMessenger& v) const {
	return _looper == v._looper && _handler == v._handler;
}
