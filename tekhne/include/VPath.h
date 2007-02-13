/***************************************************************************
 *            VDirectory.h
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

#ifndef _VPATH_H
#define _VPATH_H

#include "VFlattenable.h"
#include "VEntry.h"

namespace tekhne {

class VPath : VFlattenable {
private:
	VString _path;
	VString _leaf;
	VString _fullPath;
	inline bool must_normalize_me () {
		if ('/' != _path.ByteAt(0) || _path.FindFirst("/./")!=V_ERROR || _path.FindFirst("/..")!=V_ERROR ||
				  _path.FindFirst("//")!=V_ERROR || _path.FindLast("/.") == _path.Length()-2 ||
				  '/' == _path.ByteAt(_path.Length()-1)) return true;
		return false;
	}
	inline void split_me() {
		_fullPath = _path;
		if (_path != "/") {
			int32_t idx = _path.FindLast("/");
			_leaf = _path.Substring(idx+1);
			if (idx == 0) idx = 1;
			_path.Truncate(idx);
		}
	}
	bool normalize_me();
public:
	VPath(const char *path, const char *leaf = 0, bool normalize = false){
		SetTo(path, leaf, normalize);
	}

	VPath(const VDirectory *dir, const char *leaf = 0, bool normalize = false) {
		SetTo(dir, leaf, normalize);
	}

	VPath(const VEntry *entry) {
		SetTo(entry);
	}
	VPath(void) {}
	VPath(const VPath &path);

	virtual ~VPath();

	status_t Append(const char *path, bool normalize = false);

	status_t GetParent(VPath *path) const;

	inline status_t InitCheck(void) const {
		return _path.Length() > 0 ? V_OK : V_NO_INIT;
	}

	inline const char *Path(void) const {
		if (InitCheck() != V_OK) return 0;
		return _path.String();
	}

	inline const char *Leaf(void) const {
		if (InitCheck() != V_OK) return 0;
		return _leaf.String();
	}

	inline const char *FullPath(void) const {
		if (InitCheck() != V_OK) return 0;
		return _fullPath.String();
	}


	status_t SetTo(const VPath &path, bool normalize = false);
	status_t SetTo(const char *path, const char *leaf = 0, bool normalize = false);
	status_t SetTo(const VDirectory *dir, const char *leaf = 0, bool normalize = false);
	inline status_t SetTo(const VEntry *entry) {
		if (entry && entry->_path) return SetTo(*entry->_path);
		return V_BAD_VALUE;
	}
	inline void Unset(void) {
		_leaf.Clear();
		_path.Clear();
		_fullPath.Clear();
	}

	virtual bool AllowsTypeCode(type_code code) const;

	virtual status_t Flatten(void *buffer, ssize_t size) const;

	virtual ssize_t FlattenedSize() const;

	virtual type_code TypeCode() const;

	virtual status_t Unflatten(type_code code, const void *buffer, ssize_t size);
	virtual bool IsFixedSize(void) const { return false; }

	VPath& operator=(const VPath &path);
	VPath& operator=(const char *string);

	bool operator==(const VPath &path) const;
	bool operator==(const char *string) const;
	bool operator!=(const VPath &path) const;
	bool operator!=(const char *string) const;
};

}// namespace tekhne

#endif /* _VPATH_H */
