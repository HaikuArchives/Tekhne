/***************************************************************************
 *            VSymLink.cc
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

VSymLink::VSymLink(const VSymLink &link) {
	if (link.InitCheck() == V_OK) {
		SetTo(link._path->FullPath());
		if (!IsSymLink()) Unset();
	}
}

VSymLink::VSymLink(const VPath *path) {
	if (path && path->InitCheck() == V_OK) {
		SetTo(path->FullPath());
		if (!IsSymLink()) Unset();
	}
}

VSymLink::VSymLink(const VEntry *entry) {
	if (entry && entry->InitCheck() == V_OK) {
		SetTo(entry->FullPath());
		if (!IsSymLink()) Unset();
	}
}


status_t VSymLink::MakeLinkedPath(VPath *path) const {
	return GetPath(path);
}

size_t VSymLink::ReadLink(char *buf, size_t length) {
	if (!buf || InitCheck() != V_OK) return -1;
	strcpy(buf, _path->FullPath());
	return strlen(buf);
}

status_t VSymLink::GetStat(struct stat *st) const {
	if (!_path || _path->InitCheck() != V_OK) return V_NO_INIT;
	if (lstat(_path->FullPath(), st)) return errno;
	return V_OK;
}

