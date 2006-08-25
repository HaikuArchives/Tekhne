/***************************************************************************
 *            VMessageFilter.cc
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

VMessageFilter::VMessageFilter(int32_t delivery, int32_t source, int32_t command, filter_hook filter) :
		_delivery(delivery), _source(source), _command(command), _filter(filter), _looper(0) {
}

VMessageFilter::VMessageFilter(int32_t delivery, int32_t source, filter_hook filter) :
		_delivery(delivery), _source(source), _command(V_ANY_COMMAND), _filter(filter), _looper(0) {
}

VMessageFilter::VMessageFilter(int32_t command, filter_hook filter):
		_delivery(V_ANY_DELIVERY), _source(V_ANY_SOURCE), _command(command), _filter(filter), _looper(0) {
}

VMessageFilter::VMessageFilter(const VMessageFilter &object) :
		_delivery(object._delivery), _source(object._source), _command(object._command), _filter(object._filter), _looper(0) {
}

VMessageFilter::VMessageFilter(const VMessageFilter *object) {
	if (object) {
		_delivery = object->_delivery;
		_source = object->_source;
		_command = object->_command;
		_filter = object->_filter;
	}
}

VMessageFilter::~VMessageFilter() {
}


int32_t VMessageFilter::Command(void) const {
	return _command;
}

bool VMessageFilter::FiltersAnyCommand(void) const {
	return _command == V_ANY_COMMAND;
}

int32_t VMessageFilter::Filter(VMessage *message, VHandler **target) {
	return V_DISPATCH_MESSAGE;
}

VLooper *VMessageFilter::Looper(void) const {
	return 0;
}

int32_t VMessageFilter::MessageDelivery(void) const {
	return _delivery;
}

int32_t VMessageFilter::MessageSource(void) const {
	return _source;
}

VMessageFilter &VMessageFilter::operator=(const VMessageFilter& f) {
	_delivery = f._delivery;
	_source = f._source;
	_command = f._command;
	_filter = f._filter;
	_looper = f._looper;
	return *this;
}
