/***************************************************************************
 *            VLooper.cc
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
#include <iostream>

using namespace tekhne;

VLooper::VLooper(const char *name, int32_t priority, int32_t portCapacity) :
	VHandler(name), _quitting(false), _currentMessage(0), _preferredHandler(0) {
	_looper = this;
	_handlers.AddItem(this);
	_mq = new VMessageQueue();
}

VLooper::VLooper(VMessage *archive) :
	VHandler(archive), _quitting(false), _currentMessage(0), _preferredHandler(0) {
	_looper = this;
	_handlers.AddItem(this);
	_mq = new VMessageQueue();
}

VLooper::~VLooper() {
}

VLooper *VLooper::LooperForThread(thread_t thread) {
	return 0;
}

void VLooper::AddCommonFilter(VMessageFilter *filter) {
}

bool VLooper::RemoveCommonFilter(VMessageFilter *filter) {
	return false;
}

void VLooper::SetCommonFilterList(VList *filters) {
}

VList *VLooper::CommonFilterList(void) const {
	return 0;
}

void VLooper::AddHandler(VHandler *handler) {
	_handlers.AddItem(handler);
}

bool VLooper::RemoveHandler(VHandler *handler) {
	return 	_handlers.RemoveItem(handler);
}

VHandler *VLooper::HandlerAt(int32_t index) const {
	return (VHandler *)_handlers.GetItem(index);
}

int32_t VLooper::CountHandlers(void) const {
	return _handlers.CountItems();
}

int32_t VLooper::IndexOf(VHandler *handler) const {
	return _handlers.IndexOf(handler);
}

VMessage *VLooper::CurrentMessage(void) const {
	return _currentMessage;
}

VMessage *VLooper::DetachCurrentMessage(void) {
	return 0;
}

void VLooper::DispatchMessage(VMessage *message, VHandler *target) {
	_currentMessage = message;
	if (target) {
		// 1) Specific handler
		target->MessageReceived(message);
	} else {
		VHandler *ph = PreferredHandler();
		if (ph) {
			// 2) preferred handler
			ph->MessageReceived(message);
		} else {
			// 3) send to ourself
			this->MessageReceived(message);
		}
	}
	_currentMessage = 0;
}

bool VLooper::Lock(void) {
	return _mq->Lock();
}

status_t VLooper::LockWithTimeout(bigtime_t timeout) {
	return _mq->LockWithTimeout(timeout);
}

void VLooper::Unlock(void) {
	_mq->Unlock();
}

thread_t VLooper::LockingThread(void) const {
	return _mq->_lock.LockingThread();
}

bool VLooper::IsLocked(void) const {
	return _mq->_lock.IsLocked();
}

int32_t VLooper::CountLocks(void) const {
	return _mq->_lock.CountLocks();
}

void VLooper::MessageReceived(VMessage *message) {
}

VMessageQueue *VLooper::MessageQueue(void) const {
	return _mq;
}

status_t VLooper::PostMessage(VMessage *message) {
	status_t err = V_OK;
	if (Lock()) {
		_mq->AddMessage(new VMessage(*message));
		Unlock();
	} else {
		err = V_ERROR;
	}
	return err;
}

status_t VLooper::PostMessage(uint32_t command) {
	status_t err = V_OK;
	if (Lock()) {
		_mq->AddMessage(new VMessage(command));
		Unlock();
	} else {
		err = V_ERROR;
	}
	return err;
}

status_t VLooper::PostMessage(VMessage *message, VHandler *handler, VHandler *replyHandler) {
	status_t err = V_OK;
	if (Lock()) {
		VMessage *nmsg = new VMessage(*message);
		nmsg->_handler = handler;
		nmsg->_replyHandler = replyHandler;
		_mq->AddMessage(nmsg);
		Unlock();
	} else {
		err = V_ERROR;
	}
	return err;
}

status_t VLooper::PostMessage(uint32_t command, VHandler *handler, VHandler *replyHandler) {
	status_t err = V_OK;
	if (Lock()) {
		VMessage *nmsg = new VMessage(command);
		nmsg->_handler = handler;
		nmsg->_replyHandler = replyHandler;
		_mq->AddMessage(nmsg);
		Unlock();
	} else {
		err = V_ERROR;
	}
	return err;
}

void VLooper::Quit(void) {
	PostMessage(V_QUIT_REQUESTED);
}

bool VLooper::QuitRequested(void) {
	return true;
}

void *tekhne::looper_thread_func(void *l) {
	VLooper *looper = (VLooper *)l;
	while (!looper->_quitting) {
		VMessage *msg = looper->MessageQueue()->NextMessage();
		looper->Lock();
		switch(msg->what) {
			case V_QUIT_REQUESTED:
				if (looper->QuitRequested()) {
					looper->_quitting = true;
				}
				break;
			default:
				// goes to the preferred handler if there is one and then ourselves
				looper->DispatchMessage(msg, msg->_handler);
		}
		looper->Unlock();
		delete msg;
	}
	delete looper;
	return 0;
}

thread_t VLooper::Run(void) {

	if (print_debug_messages) std::cout << "Run" << std::endl;

	pthread_attr_init(&_attr);
	pthread_create(&_thread, &_attr, looper_thread_func, this);
	pthread_detach(_thread);

	return _thread;
}

void VLooper::SetPreferredHandler(VHandler *handler) const {
	const_cast<VLooper*>(this)->_preferredHandler = handler;
}

VHandler *VLooper::PreferredHandler(void) {
	return _preferredHandler;
}

VArchivable *VLooper::Instantiate(VMessage *archive) {
	return new VLooper(archive);
}
status_t VLooper::Archive(VMessage *archive, bool deep) const {
	// Every Archive method should look like this...
	if (archive) {
		if (deep) {
			// call return super::Archive()
			return VHandler::Archive(archive);
		}
		return V_OK;
	}
	return V_BAD_VALUE;
}
