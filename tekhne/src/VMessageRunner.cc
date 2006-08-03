/***************************************************************************
 *            VmessageRunner.cc
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

void *VMessageRunner::loop_function(void *m) {
	VMessageRunner *mr = static_cast<VMessageRunner*>(m);
	while(mr->_count > 0) {
		usleep(mr->_interval);
		mr->_target.SendMessage(const_cast<VMessage*>(mr->_message));
	}
	return 0;
}

void *VMessageRunner::loop_function_with_reply(void *m) {
	VMessageRunner *mr = static_cast<VMessageRunner*>(m);
	while(mr->_count > 0) {
		usleep(mr->_interval);
		mr->_target.SendMessage(const_cast<VMessage*>(mr->_message), &mr->_replyTo);
	}
	return 0;
}

VMessageRunner::VMessageRunner(VMessenger target, const VMessage *message, bigtime_t interval, int32_t count) :
	_target(target), _message(message), _interval(interval), _count(count) {
	pthread_attr_init(&_attr);
	pthread_create(&_thread, &_attr, VMessageRunner::loop_function, this);
	pthread_detach(_thread);
}

VMessageRunner::VMessageRunner(VMessenger target, const VMessage *message, bigtime_t interval, int32_t count, VMessenger replyTo) :
	_target(target), _message(message), _interval(interval), _count(count) {
	pthread_attr_init(&_attr);
	pthread_create(&_thread, &_attr, VMessageRunner::loop_function_with_reply, this);
	pthread_detach(_thread);
}

VMessageRunner::~VMessageRunner() {
}

status_t VMessageRunner::GetInfo(bigtime_t *interval, int32_t *count) const {
	*interval = _interval;
	*count = _count;
	return V_OK;
}

status_t VMessageRunner::InitCheck(void) const {
	return V_OK;
}

status_t VMessageRunner::SetCount(int32_t count) {
	_count = count;
	return V_OK;
}

status_t VMessageRunner::SetInterval(bigtime_t interval) {
	_interval = interval;
	return V_OK;
}
