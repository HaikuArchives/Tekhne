/***************************************************************************
 *            VStatable.cc
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
using namespace std;

VEntry::VEntry(const VDirectory *dir, const char *path, bool traverse) {
	_path = new VPath(dir, path, traverse);
}

VEntry::VEntry(const char *path, bool traverse) {
	SetTo(path, traverse);
}

VEntry::VEntry(void) {
}

VEntry::VEntry(const VEntry &entry) {
	_path = new VPath(&entry);
}

bool VEntry::Exists(void) const {
	if (InitCheck() != V_OK) return V_NO_INIT;
	struct stat st;
	return GetStat(&st) == V_OK;
}

status_t VEntry::GetName(char *buffer) const {
	return V_ERROR;
}

status_t VEntry::GetPath(VPath *path) const {
	return V_ERROR;
}

status_t VEntry::GetParent(VEntry *entry) const {
	return V_ERROR;
}

status_t VEntry::GetParent(VDirectory *dir) const {
	return V_ERROR;
}

status_t VEntry::InitCheck(void) const {
	return (_path) ? _path->InitCheck() : V_NO_INIT;
}

status_t VEntry::Remove(void) {
	return V_ERROR;
}

status_t VEntry::Rename(const char *path, bool clobber) {
	return V_ERROR;
}

status_t VEntry::MoveTo(VDirectory *dir, const char *path, bool clobber) {
	return V_ERROR;
}

status_t VEntry::SetTo(const char *path, bool traverse) {
	_path = new VPath(path, 0, traverse);
	return _path->InitCheck();
}

status_t VEntry::SetTo(const VDirectory *dir, const char *path, bool traverse) {
	return V_ERROR;
}

void VEntry::Unset(void) {
}

VEntry& VEntry::operator=(const VEntry &entry) {
	return *this;
}

bool VEntry::operator==(const VEntry &entry) const {
	return false;
}

bool VEntry::operator!=(const VEntry &entry) const {
	return false;
}

