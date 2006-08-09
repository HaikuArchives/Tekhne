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
 * IN THE SOFTWARE.#include <sys/socket.h>

 * 
 ****************************************************************************/

#include "tekhne.h"
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>

using namespace std;

using namespace tekhne;

VMessenger::VMessenger(const VHandler *handler, const VLooper *looper, status_t *error) :
	_handler(const_cast<VHandler *>(handler)), _looper(const_cast<VLooper *>(looper)),
	_localTarget(true), _isValid(true), _signature(0) {
	if (error) *error = V_OK;
	if (!_handler || !_looper) {
		_isValid = false;
		if (error) *error = V_BAD_VALUE;
	}
}

VMessenger::VMessenger(const char *signature, team_t team, status_t *error) :
	_handler(0), _looper(0), _localTarget(false), _isValid(true), _signature(signature) {
	if (error) *error = V_OK;
	if (_signature.Length() == 0) {
		if (error) *error = V_OK;
		_isValid = false;
	}
}

VMessenger::VMessenger(const VMessenger &messenger) :
	_handler(0), _looper(0), _localTarget(true), _isValid(true), _signature(0) {
	if (messenger._isValid) {
		if (messenger._localTarget) {
			_handler = messenger._handler;
			_looper = messenger._looper;
		} else {
			// initialize connection to remote target
		}
	} else {
		_isValid = false;
	}
}

VMessenger::VMessenger(void) {
	_handler = 0;
	_looper = 0;
}

VMessenger::~VMessenger() {
}

bool VMessenger::IsValid(void) const {
	return _isValid;
}

bool VMessenger::LockTarget(void) const {
	if (_localTarget && _isValid && _looper) {
		return _looper->Lock();
	}
	return false;
}

status_t VMessenger::LockTargetWithTimeout(bigtime_t timeout) const {
	if (_localTarget && _isValid && _looper) {
		return _looper->LockWithTimeout(timeout);
	}
	return V_ERROR;
}

status_t VMessenger::SendMessage(VMessage *message, VMessage *reply, bigtime_t deliveryTimeout, bigtime_t replyTimeout) const {
	status_t err = V_ERROR;
	if (_isValid) {
		if (_localTarget) {
			err = _looper->LockWithTimeout(deliveryTimeout);
			if (err == V_OK) {
				// not sure what to do with reply, need to fill it in somehow
				err = _looper->PostMessage(message, _handler);
				_looper->Unlock();
			}
		} else {
		}
	}
	return err;
}

status_t VMessenger::SendMessage(VMessage *message, VHandler *replyHandler, bigtime_t deliveryTimeout) const {
	status_t err = V_ERROR;
	if (_isValid) {
		if (_localTarget) {
			err = _looper->LockWithTimeout(deliveryTimeout);
			if (err == V_OK) {
				err = _looper->PostMessage(message, _handler, replyHandler);
				_looper->Unlock();
			}
		} else {
		}
	}
	return err;
}

status_t VMessenger::SendMessage(VMessage *message, VMessenger *replyMessenger, bigtime_t deliveryTimeout) const {
	status_t err = V_ERROR;
	if (_isValid) {
		if (_localTarget) {
			err = _looper->LockWithTimeout(deliveryTimeout);
			if (err == V_OK) {
				err = _looper->PostMessage(message, _handler, replyMessenger->_handler);
				_looper->Unlock();
			}
		} else {
		}
	}
	return err;
}

status_t VMessenger::SendMessage(uint32_t command, VMessage *reply) const {
	status_t err = V_ERROR;
	if (_isValid) {
		if (_localTarget) {
			_looper->Lock();
			// not sure what to do with reply, need to fill it in somehow
			err = _looper->PostMessage(command);
			_looper->Unlock();
		} else {
			VMessage msg(command);
			VMallocIO mio;
			msg.Flatten(&mio);
			err = SendToRemoteHost(mio);
			// need to read reply
		}
	}
	return err;
}

status_t VMessenger::SendMessage(uint32_t command, VHandler *replyHandler) const {
	status_t err = V_ERROR;
	if (_isValid) {
		if (_localTarget) {
			_looper->Lock();
			err = _looper->PostMessage(command, _handler, replyHandler);
			_looper->Unlock();
		} else {
			VMessage msg(command);
			VMallocIO mio;
			msg.Flatten(&mio);
			err = SendToRemoteHost(mio);
		}
	}
	return err;
}

VHandler *VMessenger::Target(VLooper **looper) const {
	if (_isValid && _localTarget) {
		*looper = _looper;
		return _handler;
	}
	*looper = 0;
	return 0;
}

bool VMessenger::IsTargetLocal(void) const {
	return _localTarget;
}

inline team_t VMessenger::Team(void) const {
	if (_isValid && _localTarget) {
		return 0; //return our team
	}
	// return remote team
	return 0;
}

VMessenger &VMessenger::operator =(const VMessenger& v) {
	if (this != &v) {
		_localTarget = v._localTarget;
		_isValid = v._isValid;
		_signature = v._signature;
		_looper = v._looper;
		_handler = v._handler;
	}
	return *this;
}

bool VMessenger::operator ==(const VMessenger& v) const {
	if (_localTarget && v._localTarget) {
		return _looper == v._looper && _handler == v._handler;
	} else if (!_localTarget && !v._localTarget) {
		return _signature == v._signature;
	}
	return false;
}

// private method
status_t VMessenger::SendToRemoteHost(VMallocIO &data) const {
	status_t err = V_ERROR;
	int32_t len = data.BufferLength();
	const void *buf = data.Buffer();
	int32_t _socket = socket(PF_LOCAL, SOCK_STREAM, 0);
	if (_socket > 0) {
		struct sockaddr_un name;
		name.sun_family = AF_LOCAL;
		VString socket_name("/tmp/");
		socket_name.Append(_signature);
		socket_name.ReplaceAll('/', '-', 5);
		strncpy(name.sun_path, socket_name.String(), sizeof (name.sun_path));
		int32_t e = connect(_socket, (struct sockaddr*)&name, SUN_LEN(&name));
		if (!e) {
			e = send (_socket, buf, len, 0);
			if (e < 0) {
				err = e;
			} else {
				if (e == len) {
					err = V_OK;
				} else {
					int32_t sent = e;
					len -= e;
					while (len > 0 && e > 0) {
						e = send (_socket, (char *)buf+sent, len, 0);
						if (e > 0) {
							sent += e;
						}
					}
				}
			}
		} else {
			err = e;
		}
		close(_socket);
	} else {
		err = _socket;
	}
	return err;
}
