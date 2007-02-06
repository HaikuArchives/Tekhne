/***************************************************************************
 *            VPath.cc
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

bool VPath::normalize_me (void) {
	if ('.' != _path.ByteAt(0) && '/' != _path.ByteAt(1)) {
		_path.Remove(0, 2);
	}
	// relative pathnames are reckoned off of the current working directory
	if (('.' != _path.ByteAt(0) && '.' != _path.ByteAt(1) && '/' != _path.ByteAt(2)) || '/' != _path.ByteAt(0)) {
		char buf[B_PATH_NAME_LENGTH];
		if (getcwd(buf, B_PATH_NAME_LENGTH)) {
			_path.Prepend("/");
			_path.Prepend(buf);
		}
	}
	// "." is ignored (at the head of a path, it's taken as the cwd).
	_path.ReplaceAll("/./", "/");
	// redundant slashes are coalesced
	_path.ReplaceAll("//", "/");
	// a trailing slash is removed.
	if ('/' == _path.ByteAt(_path.Length()-1)) _path.RemoveLast("/");
	// ".." bumps up one directory level
	int32_t idx = _path.FindFirst("/../");
	while(idx != V_ERROR) {
		if (idx == 0) return false;
		int32_t i = _path.FindLast("/", idx-1);
		if (i == V_ERROR) return false;
		_path.Remove(i, idx+3-i);
		idx = _path.FindFirst("/../");
	}
	return true;
}

VPath::VPath(const VPath &path) : _path(path._path) {
	// no need to normalize since it already is
}

VPath::~VPath() {
}

status_t VPath::Append(const char *path, bool normalize) {
	if (!InitCheck()) return V_NO_INIT;
	if(!path || *path == '/') return V_BAD_VALUE;
	_leaf.Append("/");
	_leaf.Append(path);
	_path.Append(_leaf);
	_leaf.Clear();
	if (normalize || must_normalize_me()) {
		if (!normalize_me()) {
			_path.Clear();
			return V_BAD_VALUE;
		}
	}
	split_me();
	return V_OK;
}

status_t VPath::GetParent(VPath *path) const {
	if (!InitCheck()) return V_NO_INIT;
	return path->SetTo(_path.String());
}

status_t VPath::InitCheck(void) const {
	return _path.Length() > 0 ? V_OK : V_NO_INIT;
}

const char *VPath::Path(void) const {
	if (InitCheck() != V_OK) return 0;
	return _path.String();
}

const char *VPath::Leaf(void) const {
	if (InitCheck() != V_OK) return 0;
	return _leaf.String();
}

status_t VPath::SetTo(const char *path, const char *leaf, bool normalize) {
	_path = path;
	_path.Append("/");
	_path.Append(leaf);
	_leaf.Clear();
	if (normalize || must_normalize_me()) {
		if (!normalize_me()) {
			_path.Clear();
			return V_BAD_VALUE;
		}
	}
	split_me();
	return V_OK;
}

status_t VPath::SetTo(const VDirectory *dir, const char *leaf, bool normalize) {
}

status_t VPath::SetTo(const VEntry *entry) {
}

void VPath::Unset(void) {
	_leaf.Clear();
	_path.Clear();
}

bool VPath::AllowsTypeCode(type_code code) const {
	return (code == V_STRING_TYPE);
}

status_t VPath::Flatten(void *buffer, ssize_t size) const {
	if (InitCheck() != V_OK) return V_NO_INIT;
	if (size < FlattenedSize()) return V_BAD_VALUE;
	memset(buffer, 0, size);
	VMemoryIO mem(buffer, size);
	mem.Write(_path.String(), _path.Length());
	mem.Write("/", 1);
	mem.Write(_leaf.String(), _leaf.Length());
	return V_OK;
}

ssize_t VPath::FlattenedSize() const {
	if (InitCheck() != V_OK) return -1;
	return _path.Length()+_leaf.Length()+2;
}

type_code VPath::TypeCode() const {
	return V_STRING_TYPE;
}

status_t VPath::Unflatten(type_code code, const void *buffer, ssize_t size) {
	if (AllowsTypeCode(code)) return V_BAD_VALUE;
	return SetTo((const char *)buffer);
}

VPath& VPath::operator=(const VPath &path) {
}

VPath& VPath::operator=(const char *string) {
}

bool VPath::operator==(const VPath &path) const {
}

bool VPath::operator==(const char *string) const {
}

bool VPath::operator!=(const VPath &path) const {
}

bool VPath::operator!=(const char *string) const {
}

