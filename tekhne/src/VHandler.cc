/***************************************************************************
 *            VHandler.cc
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

VHandler::VHandler(const char *name) : _name(name), _looper(0), _nextHandler(0), _filterList(0) {
}

VHandler::VHandler(VMessage *archive) : VArchivable(archive), _looper(0), _nextHandler(0), _filterList(0) {
	archive->FindString("_name", &_name);
}

VHandler::~VHandler() {
}

status_t VHandler::GetSupportedSuites(VMessage *message) {
	return V_ERROR;
}

bool VHandler::LockLooper(void) {
	if (_looper) {
		return _looper->Lock();
	}
	return false;
}

status_t VHandler::LockLooperWithTimeout(bigtime_t timeout) {
	if (_looper) {
		return _looper->LockWithTimeout(timeout);
	}
	return V_BAD_VALUE;
}

void VHandler::UnlockLooper(void) {
	if (_looper) {
		_looper->Unlock();
	}
}

VLooper *VHandler::Looper(void) const {
	return _looper;
}

void VHandler::MessageReceived(VMessage *message) {
}

VHandler *VHandler::ResolveSpecifier(VMessage *message, int32_t index, VMessage *specifier, int32_t what, const char *property) {
	return 0;
}

void VHandler::SetFilterList(VList *list) {
	delete _filterList;
	_filterList = list;
}

VList *VHandler::FilterList(void) const {
	return _filterList;
}

void VHandler::AddFilter(VMessageFilter *filter) {
	if (_filterList) {
		_filterList->AddItem(filter);
	}
}

bool VHandler::RemoveFilter(VMessageFilter *filter) {
	if (_filterList) {
		return _filterList->RemoveItem(filter);
	}
	return false;
}

void VHandler::SetName(const char *string) {
	_name = string;
}

const char *VHandler::Name(void) const {
	return _name.String();
}

void VHandler::SetNextHandler(VHandler *handler) {
	_nextHandler = handler;
}

VHandler *VHandler::NextHandler(void) const {
	return _nextHandler;
}

status_t VHandler::StartWatching(VMessenger *watcher, uint32_t what) {
	return V_ERROR;
}

status_t VHandler::StartWatching(VHandler *watcher, uint32_t what) {
	return V_ERROR;
}

status_t VHandler::StartWatchingAll(VMessenger *watcher) {
	return V_ERROR;
}

status_t VHandler::StartWatchingAll(VHandler *watcher) {
	return V_ERROR;
}

status_t VHandler::StopWatching(VMessenger *watcher, uint32_t what) {
	return V_ERROR;
}

status_t VHandler::StopWatching(VHandler *watcher, uint32_t what) {
	return V_ERROR;
}

status_t VHandler::StopWatchingAll(VMessenger *watcher) {
	return V_ERROR;
}

status_t VHandler::StopWatchingAll(VHandler *watcher) {
	return V_ERROR;
}

VArchivable *VHandler::Instantiate(VMessage *archive) {
	return new VHandler(archive);
}
status_t VHandler::Archive(VMessage *archive, bool deep) const {
	// Every Archive method should look like this...
	if (archive) {
		// add class specific stuff here
		archive->AddString("_name", _name);
		
		if (deep) {
			// call return super::Archive()
			return VArchivable::Archive(archive);
		}
		return V_OK;
	}
	return V_BAD_VALUE;
}
