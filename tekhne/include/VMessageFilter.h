/***************************************************************************
 *            VMessageFilter.h
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

#ifndef _VMESSAGEFILTER_H
#define _VMESSAGEFILTER_H

#include "VErrors.h"

namespace tekhne {
	
class VMessageFilter;
class VHandler;
class VMessage;
class VLooper;

typedef int32_t filter_result;
typedef int32_t message_delivery;
typedef int32_t message_source;

typedef filter_result (*filter_hook)(VMessage *message, VHandler **target, VMessageFilter *messageFilter);

class VMessageFilter {
private:
public:
	VMessageFilter(message_delivery delivery, message_source source, uint32_t command, filter_hook filter = 0);
	VMessageFilter(message_delivery delivery, message_source source, filter_hook filter = 0);
	VMessageFilter(uint32_t command, filter_hook filter = 0);
	VMessageFilter(const VMessageFilter &object);
	VMessageFilter(const VMessageFilter *object);

	virtual ~VMessageFilter();

	uint32_t Command(void) const;
	bool FiltersAnyCommand(void) const;

	virtual filter_result Filter(VMessage *message, VHandler **target);

	VLooper *Looper(void) const;

	message_delivery MessageDelivery(void) const;
	message_source MessageSource(void) const;

	VMessageFilter &operator=(const VMessageFilter&);
};

}

#endif /* _VMESSAGEFILTER_H */
