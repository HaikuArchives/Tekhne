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
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

using namespace tekhne;
using namespace std;

VClipboard *tekhne::v_clipboard = 0;

VClipboard::VClipboard(const char *name, bool discard) : _data(new VMessage()), _shared_mem(0), _shmid(0), _page_size(0), _lock(new VLocker()), _watchers(0) {
	memset(_name, 0, V_NAME_LENGTH);
	strncpy(_name, name, V_NAME_LENGTH-1);
	if (v_clipboard == 0 && strcmp(name, "system") == 0) {
		v_clipboard = this;
	}
	_page_size = (size_t) sysconf (_SC_PAGESIZE);
	struct stat buf;
	int32_t id = stat ("/tmp/v_clipboard", &buf );
	if ( id != 0 ) {
		close(open("/tmp/v_clipboard", O_CREAT|O_TRUNC|O_RDWR, 0600));
		//chmod("/tmp/v_clipboard", 0600);
	}
	key_t mykey=ftok("/tmp/v_clipboard", 1);
	_shmid = shmget(mykey, 16*_page_size, (IPC_CREAT|0640));
//	shmctl(_shmid, IPC_RMID, 0);
	_shared_mem = shmat(_shmid, 0, 0);
}

VClipboard::~VClipboard() {
	shmdt(_shared_mem);
	_shared_mem = 0;
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
		ssize_t bytes;
		VMemoryIO mem(_shared_mem, 16*_page_size);
		_data->Flatten(&mem, &bytes);
		return V_OK;
	}
	return V_ERROR;
}

status_t VClipboard::Revert(void) {
	if (_lock->IsLocked()) {
		VMemoryIO mem(_shared_mem, 16*_page_size);
		_data->Unflatten(&mem);
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
	return _lock->Lock();
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
