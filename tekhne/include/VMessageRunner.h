/***************************************************************************
 *            VmessageRunner.h
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

#ifndef _VMESSAGERUNNER_H
#define _VMESSAGERUNNER_H

#include "VErrors.h"
#include "VMessage.h"
#include "VMessenger.h"

namespace tekhne {

class VMessageRunner {
private:
	VMessenger _target;
	const VMessage *_message;
	bigtime_t _interval;
	int32_t _count;
	VMessenger _replyTo;

	pthread_t _thread;
	pthread_attr_t _attr;

	static void *loop_function(void *m);
	static void *loop_function_with_reply(void *m);
public:
	VMessageRunner(VMessenger target, const VMessage *message, bigtime_t interval, int32_t count = -1);
	VMessageRunner(VMessenger target, const VMessage *message, bigtime_t interval, int32_t count, VMessenger replyTo);
	
	virtual ~VMessageRunner();
	
	status_t GetInfo(bigtime_t *interval, int32_t *count) const;

	status_t InitCheck(void) const;

	status_t SetCount(int32_t count);
	status_t SetInterval(bigtime_t interval);
};

}

#endif /* _VMESSAGERUNNER_H */
