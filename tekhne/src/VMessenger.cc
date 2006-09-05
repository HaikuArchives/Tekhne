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
#include "IMessenger.h"
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>

using namespace std;

using namespace tekhne;

VMessenger::VMessenger(const VHandler *handler, const VLooper *looper, status_t *error) :
	_handler(const_cast<VHandler *>(handler)), _looper(const_cast<VLooper *>(looper)),
	_localTarget(true), _isValid(true), _signature(0), _id(getMextMessengerId()) {
	if (error) *error = V_OK;
	setup_reply_mutex();
	if (!_handler || !_looper) {
		_isValid = false;
		if (error) *error = V_BAD_VALUE;
	}
	if (_isValid) {
		registerMessenger(this, _id);
	}
}

VMessenger::VMessenger(const char *signature, team_t team, status_t *error) :
	_handler(0), _looper(0), _localTarget(false), _isValid(true),
	_signature(signature), _id(getMextMessengerId()) {
	setup_reply_mutex();
	if (error) *error = V_OK;
	if (_signature.Length() == 0) {
		if (error) *error = V_BAD_VALUE;
		_isValid = false;
	} else {
		// if we are making a remote messenger to ourself just turn it into a local messenger
		if (_signature == v_app->Signature()) {
			_looper = v_app;
			_handler = v_app;
			_signature.Clear();
			_localTarget = true;
		}
	}
	if (_isValid) registerMessenger(this, _id);
}

VMessenger::VMessenger(const VMessenger &messenger) :
	_handler(0), _looper(0), _localTarget(true), _isValid(true),
	_signature(0), _id(getMextMessengerId()) {
	setup_reply_mutex();
	if (messenger._isValid) {
		_handler = messenger._handler;
		_looper = messenger._looper;
		_localTarget = messenger._localTarget;
		_signature = messenger._signature;
	} else {
		_isValid = false;
	}
	if (_isValid) registerMessenger(this, _id);
}

VMessenger::VMessenger(void) : _handler(0), _looper(0), _localTarget(true),
	_isValid(false), _signature(0), _id(getMextMessengerId())  {
	setup_reply_mutex();
	if (_isValid) registerMessenger(this, _id);
}

VMessenger::~VMessenger() {
	pthread_mutex_destroy(&_reply_mutex);
	pthread_cond_destroy(&_reply_cond);
  	unregisterMessenger(_id);
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
		if (reply) message->_isSourceWaiting = true;
		else  message->_isSourceWaiting = false;
		if (_localTarget) {
			err = _looper->LockWithTimeout(deliveryTimeout);
			if (err == V_OK) {
				message->_replyMessage = reply;
				err = _looper->PostMessage(message);
				_looper->Unlock();
			}
		} else {
			if (reply) const_cast<VMessenger*>(this)->_isWaiting = true;
			else  const_cast<VMessenger*>(this)->_isWaiting = false;
			message->AddString("_replySignature", v_app->Signature());
			message->AddInt32("_originatingMessenger", _id);
			err = SendToRemoteHost(_signature.String(), message);
			if (err == V_OK && reply) {
				struct timeval now;
				struct timespec timeout;
				pthread_mutex_lock(&const_cast<VMessenger*>(this)->_reply_mutex);
				gettimeofday(&now, 0);
				timeout.tv_sec = now.tv_sec + 5;
				timeout.tv_nsec = now.tv_usec * 1000;
				while (!_replyMessage && !err) {
					err = pthread_cond_timedwait(&const_cast<VMessenger*>(this)->_reply_cond, &const_cast<VMessenger*>(this)->_reply_mutex, &timeout);
				}
				if (err != ETIMEDOUT && _replyMessage) {
					*reply = *_replyMessage;
					delete _replyMessage;
					const_cast<VMessenger*>(this)->_replyMessage = 0;
				}
				/* operate on x and y */
				pthread_mutex_unlock(&const_cast<VMessenger*>(this)->_reply_mutex);
			}
			const_cast<VMessenger*>(this)->_isWaiting = false;
		}
	}
	return err;
}

status_t VMessenger::SendMessage(VMessage *message, VHandler *replyHandler, bigtime_t deliveryTimeout) const {
	status_t err = V_ERROR;
	if (_isValid) {
		if (replyHandler) message->_isSourceWaiting = true;
		else  message->_isSourceWaiting = false;
		if (_localTarget) {
			err = _looper->LockWithTimeout(deliveryTimeout);
			if (err == V_OK) {
				message->_replyHandler = replyHandler;
				err = _looper->PostMessage(message, _handler, replyHandler);
				_looper->Unlock();
			}
		} else {
			if (replyHandler) const_cast<VMessenger*>(this)->_isWaiting = true;
			else const_cast<VMessenger*>(this)->_isWaiting = false;
			message->AddString("_replySignature", v_app->Signature());
			message->AddInt32("_originatingMessenger", _id);
			err = SendToRemoteHost(_signature.String(), message);
			if (err == V_OK && replyHandler) {
				struct timeval now;
				struct timespec timeout;
				pthread_mutex_lock(&const_cast<VMessenger*>(this)->_reply_mutex);
				gettimeofday(&now, 0);
				timeout.tv_sec = now.tv_sec + 5;
				timeout.tv_nsec = now.tv_usec * 1000;
				while (!_replyMessage) {
					err = pthread_cond_timedwait(&const_cast<VMessenger*>(this)->_reply_cond, &const_cast<VMessenger*>(this)->_reply_mutex, &timeout);
				}
				if (err != ETIMEDOUT) {
					replyHandler->Looper()->PostMessage(_replyMessage);
					delete _replyMessage;
					const_cast<VMessenger*>(this)->_replyMessage = 0;
				}
				/* operate on x and y */
				pthread_mutex_unlock(&const_cast<VMessenger*>(this)->_reply_mutex);
			}
			const_cast<VMessenger*>(this)->_isWaiting = false;
		}
	}
	return err;
}

status_t VMessenger::SendMessage(VMessage *message, VMessenger *replyMessenger, bigtime_t deliveryTimeout) const {
	status_t err = V_ERROR;
	if (_isValid) {
		if (replyMessenger) message->_isSourceWaiting = true;
		else  message->_isSourceWaiting = false;
		if (_localTarget) {
			err = _looper->LockWithTimeout(deliveryTimeout);
			if (err == V_OK) {
				if (replyMessenger) message->_replyHandler = replyMessenger->Target(0);
				err = _looper->PostMessage(message, _handler, replyMessenger->_handler);
				_looper->Unlock();
			}
		} else {
			if (replyMessenger) const_cast<VMessenger*>(this)->_isWaiting = true;
			else const_cast<VMessenger*>(this)->_isWaiting = false;
			message->AddString("_replySignature", v_app->Signature());
			message->AddInt32("_originatingMessenger", _id);
			err = SendToRemoteHost(_signature.String(), message);
			if (err == V_OK && replyMessenger) {
				struct timeval now;
				struct timespec timeout;
				pthread_mutex_lock(&const_cast<VMessenger*>(this)->_reply_mutex);
				gettimeofday(&now, 0);
				timeout.tv_sec = now.tv_sec + 5;
				timeout.tv_nsec = now.tv_usec * 1000;
				while (!_replyMessage) {
					err = pthread_cond_timedwait(&const_cast<VMessenger*>(this)->_reply_cond, &const_cast<VMessenger*>(this)->_reply_mutex, &timeout);
				}
				if (err != ETIMEDOUT) {
					replyMessenger->SendMessage(_replyMessage, static_cast<VHandler*>(0));
					delete _replyMessage;
					const_cast<VMessenger*>(this)->_replyMessage = 0;
				}
				/* operate on x and y */
				pthread_mutex_unlock(&const_cast<VMessenger*>(this)->_reply_mutex);
			}
			const_cast<VMessenger*>(this)->_isWaiting = false;
		}
	}
	return err;
}

status_t VMessenger::SendMessage(int32_t command, VMessage *reply) const {
	status_t err = V_ERROR;
	if (_isValid) {
		if (_localTarget) {
			_looper->Lock();
			VMessage msg(command);
			msg._replyMessage = reply;
			msg._isSourceWaiting = true;
			// this isn't right, we need to call DispatchMessage here
			err = _looper->PostMessage(&msg);
			_looper->Unlock();
		} else {
			VMessage msg(command);
			if (reply) const_cast<VMessenger*>(this)->_isWaiting = msg._isSourceWaiting = true;
			else const_cast<VMessenger*>(this)->_isWaiting = msg._isSourceWaiting = false;
			msg.AddString("_replySignature", v_app->Signature());
			msg.AddInt32("_originatingMessenger", _id);
			err = SendToRemoteHost(_signature.String(), &msg);
			if (err == V_OK && reply) {
				struct timeval now;
				struct timespec timeout;
				pthread_mutex_lock(&const_cast<VMessenger*>(this)->_reply_mutex);
				gettimeofday(&now, 0);
				timeout.tv_sec = now.tv_sec + 5;
				timeout.tv_nsec = now.tv_usec * 1000;
				while (!_replyMessage) {
					err = pthread_cond_timedwait(&const_cast<VMessenger*>(this)->_reply_cond, &const_cast<VMessenger*>(this)->_reply_mutex, &timeout);
				}
				if (err != ETIMEDOUT) {
					*reply = *_replyMessage;
					delete _replyMessage;
					const_cast<VMessenger*>(this)->_replyMessage = 0;
				}
				/* operate on x and y */
				pthread_mutex_unlock(&const_cast<VMessenger*>(this)->_reply_mutex);
			}
			const_cast<VMessenger*>(this)->_isWaiting = false;
		}
	}
	return err;
}

status_t VMessenger::SendMessage(int32_t command, VHandler *replyHandler) const {
	status_t err = V_ERROR;
	if (_isValid) {
		if (_localTarget) {
			_looper->Lock();
			err = _looper->PostMessage(command, _handler, replyHandler);
			_looper->Unlock();
		} else {
			VMessage msg(command);
			if (replyHandler) const_cast<VMessenger*>(this)->_isWaiting = msg._isSourceWaiting = true;
			else const_cast<VMessenger*>(this)->_isWaiting = msg._isSourceWaiting = false;
			msg.AddString("_replySignature", v_app->Signature());
			msg.AddInt32("_originatingMessenger", _id);
			err = SendToRemoteHost(_signature.String(), &msg);
			if (err == V_OK && replyHandler) {
				struct timeval now;
				struct timespec timeout;
				pthread_mutex_lock(&const_cast<VMessenger*>(this)->_reply_mutex);
				gettimeofday(&now, 0);
				timeout.tv_sec = now.tv_sec + 5;
				timeout.tv_nsec = now.tv_usec * 1000;
				while (!_replyMessage) {
					err = pthread_cond_timedwait(&const_cast<VMessenger*>(this)->_reply_cond, &const_cast<VMessenger*>(this)->_reply_mutex, &timeout);
				}
				if (err != ETIMEDOUT) {
					replyHandler->Looper()->PostMessage(_replyMessage);
					delete _replyMessage;
					const_cast<VMessenger*>(this)->_replyMessage = 0;
				}
				/* operate on x and y */
				pthread_mutex_unlock(&const_cast<VMessenger*>(this)->_reply_mutex);
			}
			const_cast<VMessenger*>(this)->_isWaiting = false;
		}
	}
	return err;
}

VHandler *VMessenger::Target(VLooper **looper) const {
	if (_isValid && _localTarget) {
		if (looper) *looper = _looper;
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

namespace tekhne {

VDictionary messengerDictionary;

void registerMessenger(VMessenger *msgr, int32_t id) {
	VInteger i(id);
	messengerDictionary.AddItem(i, msgr);
}

VMessenger *unregisterMessenger(int32_t id) {
	VInteger i(id);
	return static_cast<VMessenger*>(messengerDictionary.RemoveItem(i));
}

VMessenger *findMessenger(int32_t id) {
	VInteger i(id);
	return static_cast<VMessenger*>(messengerDictionary.FindItem(i));
}

VDictionary socketDictionary;

extern fd_set active_fd_set;
static int32_t messenger_id = 0;
VLocker messenger_id_lock;

int32_t getMextMessengerId(void) {
	VAutoLock l(messenger_id_lock);
	int32_t id = messenger_id;
	messenger_id++;
	return id;
}

void deleteSocketForSignature(const char *signature) {
	VString sig(signature);
	int32_t *sock = static_cast<int32_t*>(socketDictionary.RemoveItem(sig));
	if (sock) {
		free(sock);
	}
}

void deleteSocket(int32_t sock) {
	VList items;
	socketDictionary.Items(items);
	VListIterator iter(items);
	while (iter.HasNext()) {
		int32_t *s = static_cast<int32_t*>(iter.Next());
		if (*s == sock) {
			socketDictionary.RemoveItem(s);
			free(s);
			break;
		}
	}
}

void addSocketForSignature(const char *signature, int32_t sock) {
	VString sig(signature);
	if (!socketDictionary.FindItem(sig)) {
		int32_t *sock_item = static_cast<int32_t*>(malloc(sizeof(int32_t)));
		*sock_item = sock;
		socketDictionary.AddItem(sig, sock_item);
	}
}
int32_t getSocketForSignature(const char *signature) {
	VString sig(signature);
	int32_t *sock = static_cast<int32_t*>(socketDictionary.FindItem(sig));
	if (sock) {
		return *sock;
	} else {
		int32_t _socket = socket(PF_LOCAL, SOCK_STREAM, 0);
		if (_socket > 0) {
			struct sockaddr_un name;
			name.sun_family = AF_LOCAL;
			VString socket_name("/tmp/");
			socket_name.Append(signature);
			socket_name.ReplaceAll('/', '-', 5);
			strncpy(name.sun_path, socket_name.String(), sizeof (name.sun_path));
			int32_t e = connect(_socket, (struct sockaddr*)&name, SUN_LEN(&name));
			if (!e) {
				FD_SET (_socket, &active_fd_set);
				addSocketForSignature(signature, _socket);
				return _socket;
			}
		}
	}
	return -1;
}

status_t SendToRemoteHost(const char *signature, VMessage *message) {
	status_t err = V_ERROR;
	if (!message) return err;

	message->_isSourceRemote = true;
	VMallocIO data;
	// here we finaly determine if we need to wait
	message->Flatten(&data);
	int32_t len = data.Length();
	const void *buf = data.Buffer();

	int32_t _socket = getSocketForSignature(signature);
	if (_socket < 0) {
		return err;
	} else {
		int32_t e = write (_socket, buf, len);
		if (e < 0) {
			close(_socket);
			FD_CLR(_socket, &active_fd_set);
			deleteSocketForSignature(signature);
			// try one more time
			_socket = getSocketForSignature(signature);
			if (_socket < 0) {
				return err;
			} else {
				e = write (_socket, buf, len);
				if (e < 0) {
					close(_socket);
					FD_CLR(_socket, &active_fd_set);
					deleteSocketForSignature(signature);
					err = e;
				}else if (e == len) {
					err = V_OK;
				} else { // this probably should never happen
					int32_t sent = e;
					len -= e;
					while (len > 0 && e > 0) {
						e = write (_socket, (char *)buf+sent, len);
						if (e > 0) {
							sent += e;
						}
					}
				}
			}
		} else if (e == len) {
			err = V_OK;
		} else { // this probably should never happen
			int32_t sent = e;
			len -= e;
			while (len > 0 && e > 0) {
				e = write (_socket, (char *)buf+sent, len);
				if (e > 0) {
					sent += e;
				}
			}
		}
	}
	return err;
}

} // namespace tekhne
