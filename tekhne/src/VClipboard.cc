/***************************************************************************
 *            VClipboard.cc
 *
 * Copyright (c) 2006-2007 Geoffrey Clements
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
using namespace std;

VClipboard *tekhne::v_clipboard = 0;

VClipboard::VClipboard(const char *name, bool discard) {
	memset(_name, 0, V_NAME_LENGTH);
	strncpy(_name, name, V_NAME_LENGTH-1);
	if (v_clipboard == 0 && strcmp(name, "system") == 0) {
		v_clipboard = this;
	}
}

VClipboard::~VClipboard() {
}

status_t VClipboard::Clear(void) {
	return V_OK;
}
status_t VClipboard::Commit(void) {
	return V_OK;
}

status_t VClipboard::Revert(void) {
	return V_OK;
}

VMessage *VClipboard::Data(void) const {
	return _data;
}

VMessenger *VClipboard::DataSource(void) const {
	return NULL;
}


uint32_t VClipboard::LocalCount(void) const {
	return 0;
}

uint32_t VClipboard::SystemCount(void) const {
	return 0;
}

bool VClipboard::Lock(void) {
	return false;
}

void VClipboard::Unlock(void) {
}

bool VClipboard::IsLocked(void) {
}

status_t VClipboard::StartWatching(VMessenger *target) {
	return V_OK;
}

status_t VClipboard::StopWatching(VMessenger *target) {
	return V_OK;
}
