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
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

using namespace tekhne;
using namespace std;

VClipboard *tekhne::v_clipboard = 0;

VClipboard::VClipboard(const char *name, bool discard) : _data(new VMessage()), _shared_mem(0), _page_size(0), _lock(new VLocker()) {
	memset(_name, 0, V_NAME_LENGTH);
	strncpy(_name, name, V_NAME_LENGTH-1);
	if (v_clipboard == 0 && strcmp(name, "system") == 0) {
		v_clipboard = this;
	}
	_page_size = (size_t) sysconf (_SC_PAGESIZE);
	_shared_mem = mmap(0, 16*_page_size, PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, 0, 0);
	cout << "shared mem: " << _shared_mem << endl;
}

VClipboard::~VClipboard() {
	cout << "unmapping..." << endl;
	msync (_shared_mem, 16*_page_size, MS_SYNC);
	munmap(_shared_mem, 16*_page_size);
	delete _data;
	delete _lock;
	if (_watchers) {
		delete _watchers;
	}
}

status_t VClipboard::Clear(void) {
	if (_lock->IsLocked()) {
		return _data->MakeEmpty();
	}
	return V_ERROR;
}
status_t VClipboard::Commit(void) {
	if (_lock->IsLocked()) {
		return V_OK;
	}
	return V_ERROR;
}

status_t VClipboard::Revert(void) {
	if (_lock->IsLocked()) {
		return V_OK;
	}
	return V_ERROR;
}

VMessage *VClipboard::Data(void) const {
	return _data;
}

VMessenger *VClipboard::DataSource(void) const {
	return v_app_messenger;
}


uint32_t VClipboard::LocalCount(void) const {
	return 0;
}

uint32_t VClipboard::SystemCount(void) const {
	return 0;
}

bool VClipboard::Lock(void) {
	return _lock->Lock() ? V_OK : V_ERROR;
}

void VClipboard::Unlock(void) {
	_lock->Unlock();
}

bool VClipboard::IsLocked(void) {
	return _lock->IsLocked();
}

status_t VClipboard::StartWatching(VMessenger *target) {
	if (!_watchers) {
		_watchers = new VList();
	}
	if (!_watchers->HasItem(target)) {
		_watchers->AddItem(target);
	}
	return V_OK;
}

status_t VClipboard::StopWatching(VMessenger *target) {
	return V_OK;
}
