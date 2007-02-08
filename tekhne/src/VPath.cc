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
#include <iostream>
using namespace std;

using namespace tekhne;

bool debug = false;

bool VPath::normalize_me () {
	if (debug) cout << "1> " << _path.String() << endl;
	if ('.' == _path.ByteAt(0) && '/' == _path.ByteAt(1)) {
		_path.Remove(0, 2);
	}
	if (debug) cout << "2> " << _path.String() << endl;
	// relative pathnames are reckoned off of the current working directory
	if (('.' == _path.ByteAt(0) && '.' == _path.ByteAt(1) && '/' == _path.ByteAt(2)) || '/' != _path.ByteAt(0)) {
		char buf[V_PATH_NAME_LENGTH];
		if (getcwd(buf, V_PATH_NAME_LENGTH)) {
			_path.Prepend("/");
			_path.Prepend(buf);
		}
	}
	if (debug) cout << "3> " << _path.String() << endl;
	// "." is ignored (at the head of a path, it's taken as the cwd).
	_path.ReplaceAll("/./", "/");
	if (debug) cout << "4> " << _path.String() << endl;
	// redundant slashes are coalesced
	_path.ReplaceAll("//", "/");
	// ".." bumps up one directory level
	if (debug) cout << "5> " << _path.String() << endl;
	int32_t idx = _path.FindFirst("/..");
	if (debug) cout << "idx: " << idx << endl;
	while(idx != V_ERROR) {
		if (idx == 0) return false;
		const char *buf = _path.String();
		int32_t i = idx-1;
		while(i > 0 && buf[i] != '/') i--;
		if (debug) cout << "i: " << i << endl;
		if (i == V_ERROR) return false;
		_path.Remove(i, idx+3-i);
		idx = _path.FindFirst("/..");
		if (debug) cout << "idx: " << idx << endl;
		if (debug) cout << "6> " << _path.String() << endl;
	}
	if (debug) cout << "7> " << _path.String() << endl;
	// a trailing slash is removed.
	int32_t len = _path.Length();
	if (len > 1 && '/' == _path.ByteAt(len-1)) _path.RemoveLast("/");
	len = _path.Length();
	if (len > 1 && '.' == _path.ByteAt(len-1) && '/' == _path.ByteAt(len-2)) _path.RemoveLast("/.");
	if (debug) cout << "8> " << _path.String() << endl;
	return true;
}

VPath::VPath(const VPath &path) : _path(path._path), _leaf(path._leaf) {
	// no need to normalize since it already is
}

VPath::~VPath() {
}

status_t VPath::Append(const char *path, bool normalize) {
	if (InitCheck() != V_OK) return V_NO_INIT;
	if(!path || *path == '/') return V_BAD_VALUE;
	_leaf.Append("/");
	_leaf.Append(path);
	_path.Append("/");
	_path.Append(_leaf);
	_leaf.Clear();
	if (debug) cout << "p: " << _path.String() << endl;
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
	if (path) {
		path->Unset();
		if (InitCheck() != V_OK) return V_NO_INIT;
		if (_path == "/") return V_BAD_VALUE;
		return path->SetTo(_path.String());
	}
	return V_BAD_VALUE;
}

status_t VPath::SetTo(const char *path, const char *leaf, bool normalize) {
	Unset();
	if (!path || strlen(path) == 0) return V_BAD_VALUE;
	_path = path;
	if (leaf) {
		if (*leaf == '/') {
			_path.Clear();
			return V_BAD_VALUE;
		}
		_path.Append("/");
		_path.Append(leaf);
	}
	if (normalize || must_normalize_me()) {
		if (!normalize_me()) {
			_path.Clear();
			return V_BAD_VALUE;
		}
	}
	if (debug) cout << "1>>> " << _path.String() << endl;
	split_me();
	if (debug) cout << "2>>> " << _path.String() << " " << _leaf.String() << endl;
	return V_OK;
}

status_t VPath::SetTo(const VDirectory *dir, const char *leaf, bool normalize) {
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
	if (!AllowsTypeCode(code)) return V_BAD_VALUE;
	return SetTo((const char *)buffer);
}

VPath& VPath::operator=(const VPath &path) {
	Unset();
	if (path.InitCheck() == V_OK) {
		_path = path._path;
		_leaf = path._leaf;
	}
	return *this;
}

VPath& VPath::operator=(const char *string) {
	Unset();
	SetTo(string);
	return *this;
}

bool VPath::operator==(const VPath &path) const {
	if (InitCheck() != V_OK || path.InitCheck() != V_OK) return false;
	return _path == path._path && _leaf == path._leaf;
}

bool VPath::operator==(const char *string) const {
	VPath p(string);
	return *this == p;
}

bool VPath::operator!=(const VPath &path) const {
	if (InitCheck() != V_OK || path.InitCheck() != V_OK) return true;
	return _path != path._path || _leaf != path._leaf;
}

bool VPath::operator!=(const char *string) const {
	VPath p(string);
	return *this != p;
}
