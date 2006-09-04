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
#include <unistd.h>

using namespace tekhne;
using namespace std;

VPath::VPath(const char *path, const char *leaf, bool normalize) : _err(V_NO_INIT) {
	SetTo(path, leaf, normalize);
}

VPath::VPath(const VDirectory *dir, const char *leaf, bool normalize) : _err(V_NO_INIT) {
	SetTo(dir, leaf, normalize);
}

VPath::VPath(const VEntry *entry) : _err(V_NO_INIT) {
}

VPath::VPath(void) : _err(V_NO_INIT) {
}

VPath::VPath(const VPath &path) : _err(V_NO_INIT) {
	if (path.InitCheck() == V_OK) {
		_path = path._path;
		_leaf = path._leaf;
	}
}

VPath::~VPath() {
}

status_t VPath::Append(const char *path, bool normalize) {
	return V_ERROR;
}

status_t VPath::GetParent(VPath *path) const {
	return V_ERROR;
}

status_t VPath::InitCheck(void) const {
	return _err;
}

const char *VPath::Path(void) const {
	return _path.String();
}

const char *VPath::Leaf(void) const {
	return _leaf.String();
}

status_t VPath::SetTo(const char *path, const char *leaf, bool normalize) {
	Unset();
	// does this make sense at all?
	if (!path || (*path != '/' && *path != '.')) { _err = V_BAD_VALUE; return V_BAD_VALUE; }
	char buf[1024];
	VString testPath;
	// are we setting an absolute path or a relative one to where we are?
	if (*path == '/') {
		testPath.Clear();
	} else {
		buf[0] = 0;
		getcwd(buf, 1024);
		testPath = buf;
	}

	// tokenize the path and make sure it exists
	char *token;
	char *cp = strdupa (path);
	token = strtok (cp, "/");
	const char *last_token = 0;
	while (token) {
		if (strcmp(".", token) == 0) {
			// current directory
			token = strtok (NULL, "/");
			continue;
		}
		else if (strcmp("..", token) == 0) {
			// move back a directory
			if (testPath == "/") {
				_err = V_BAD_VALUE;
				return V_BAD_VALUE;
			}
			int idx = testPath.FindLast('/');
			if (idx > 0) {
				testPath.Remove(idx, INT32_MAX);
				// unset last_token below...
				token = 0;
			} else {
				_err = V_BAD_VALUE;
				return V_BAD_VALUE;
			}
		} else {
			testPath.Append('/', 1);
			testPath.Append(token);
		}
		last_token = token;
		token = strtok (NULL, "/");
		if (access(testPath.String(), F_OK)) {
			if (!token && !leaf) {
				// we are done set path and leaf
				if (testPath.Length() > 1) {
					int idx = testPath.FindLast('/');
					_leaf = testPath.Substring(idx+1);
					_path = testPath;
				} else {
					_path = "/";
				}
				_err = V_OK;
				return V_OK;
			} else {
				_err = V_BAD_VALUE;
				return V_BAD_VALUE;
			}
		} else {
			if (!token) { // done
				if (leaf) {
					// test with leaf and then we are done
					_leaf = leaf;
					if (testPath.Length() > 1) {
						_path = testPath;
						_path += leaf;
					} else {
						_path = "/";
					}
					_err = V_OK;
					return V_OK;
				} else {
					// we are done set path and leaf
					if (testPath.Length() > 1) {
						int idx = testPath.FindLast('/');
						_leaf = testPath.Substring(idx+1);
						_path = testPath;
					} else {
						_path = "/";
					}
					_err = V_OK;
					return V_OK;
				}
			}
			// otherwise keep going...
		}
	}
	_err = V_ERROR;
	return V_ERROR;
}

status_t VPath::SetTo(const VDirectory *dir, const char *leaf, bool normalize) {
	return V_ERROR;
}

status_t VPath::SetTo(const VEntry *entry) {
	return V_ERROR;
}

void VPath::Unset(void) {
	_path.Clear();
	_err = V_NO_INIT;
}

status_t VPath::Flatten(void *buffer, ssize_t size) const {
	return V_ERROR;
}

ssize_t VPath::FlattenedSize() const {
	return V_ERROR;
}

type_code VPath::TypeCode() const {
	return 2112;
}

status_t VPath::Unflatten(type_code code, const void *buffer, ssize_t size) {
	return V_ERROR;
}

VPath& VPath::operator=(const VPath &path) {
	if (&path != this && path.InitCheck() == V_OK) {
		_path = path._path;
		_leaf = path._leaf;
	}
	return *this;
}

VPath& VPath::operator=(const char *string) {
	SetTo(string);
	return *this;
}


bool VPath::operator==(const VPath &path) const {
	return false;
}

bool VPath::operator==(const char *string) const {
	return false;
}

bool VPath::operator!=(const VPath &path) const {
	return false;
}

bool VPath::operator!=(const char *string) const {
	return false;
}
