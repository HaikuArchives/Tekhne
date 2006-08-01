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

VMessageFilter::VMessageFilter(message_delivery delivery, message_source source, uint32_t command, filter_hook filter) {
}

VMessageFilter::VMessageFilter(message_delivery delivery, message_source source, filter_hook filter) {
}

VMessageFilter::VMessageFilter(uint32_t command, filter_hook filter) {
}

VMessageFilter::VMessageFilter(const VMessageFilter &object) {
}

VMessageFilter::VMessageFilter(const VMessageFilter *object) {
}

VMessageFilter::~VMessageFilter() {
}


uint32_t VMessageFilter::Command(void) const {
	return V_ERROR;
}

bool VMessageFilter::FiltersAnyCommand(void) const {
	return false;
}

filter_result VMessageFilter::Filter(VMessage *message, VHandler **target) {
	return V_ERROR;
}

VLooper *VMessageFilter::Looper(void) const {
	return NULL;
}

message_delivery VMessageFilter::MessageDelivery(void) const {
	return V_ERROR;
}

message_source VMessageFilter::MessageSource(void) const {
	return V_ERROR;
}

VMessageFilter &VMessageFilter::operator=(const VMessageFilter& f) {
	VMessageFilter *mf = new VMessageFilter(f);
	return *mf;
}
