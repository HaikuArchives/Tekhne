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
}

VMessenger::VMessenger(const VMessenger &messenger) :
	_handler(0), _looper(0), _localTarget(true), _isValid(true), _signature(0) {
	if (messenger._isValid) {
		_handler = messenger._handler;
		_looper = messenger._looper;
		_localTarget = messenger._localTarget;
		_signature = messenger._signature;
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
				message->_replyMessage = reply;
				message->_isSourceWaiting = true;
				err = _looper->PostMessage(message);
				_looper->Unlock();
			}
		} else {
			message->AddString("_replySignature", v_app->Signature());
			err = SendToRemoteHost(_signature.String(), message, reply);
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
			message->AddString("_replySignature", v_app->Signature());
			err = SendToRemoteHost(_signature.String(), message, 0, replyHandler);
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
			message->AddString("_replySignature", v_app->Signature());
			err = SendToRemoteHost(_signature.String(), message, 0, replyMessenger->_handler);
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
			msg.AddString("_replySignature", v_app->Signature());
			err = SendToRemoteHost(_signature.String(), &msg, reply, 0);
			// need to read reply
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
			msg.AddString("_replySignature", v_app->Signature());
			err = SendToRemoteHost(_signature.String(), &msg, 0, replyHandler);
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

namespace tekhne {

VDictionary socketDictionary;

extern fd_set active_fd_set;

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

status_t SendToRemoteHost(const char *signature, VMessage *message, VMessage *reply, VHandler *replyHandler) {
	status_t err = V_ERROR;
	if (!message) return err;

	message->_isSourceRemote = true;
	VMallocIO data;
	// here we finaly determine if we need to wait
	if (reply || replyHandler) message->_isSourceWaiting = true;
	else message->_isSourceWaiting = false;
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
		if (err == V_OK && message->_isSourceWaiting) {
			void *buf = malloc(4096);
			/* Data arriving on an already-connected socket. */
			int32_t len = read (_socket, buf, 4096);
			if (len > 0) {
				VMemoryIO mio(buf, len);
				VMessage msg;
				msg.Unflatten(&mio);
				if (print_debug_messages) { cout << __FILE__ << " " << __LINE__ << ": "; msg.PrintToStream(); }
				// short circuit here to Process message directly if source is waiting
				if (reply) {
					*reply = msg;
				} else if (replyHandler) {
					replyHandler->MessageReceived(&msg);
				} else {
					v_app->PostMessage(&msg);
				}
			}
			free(buf);
		}
	}
	return err;
}

} // namespace tekhne
