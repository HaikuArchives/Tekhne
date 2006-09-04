/***************************************************************************
 *            VBlockCache.h
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

#include "VString.h"
#include "VFlattenable.h"

namespace tekhne {

class VDirectory;
class VEntry;

class VPath : public VFlattenable {
private:
	status_t _err;
	VString _path;
	VString _leaf;
public:
	VPath(const char *path, const char *leaf = 0, bool normalize = false);
	VPath(const VDirectory *dir, const char *leaf = 0, bool normalize = false);
	VPath(const VEntry *entry);
	VPath(void);
	VPath(const VPath &path);

	virtual ~VPath();

	status_t Append(const char *path, bool normalize = false);

	status_t GetParent(VPath *path) const;

	status_t InitCheck(void) const;

	const char *Path(void) const;
	const char *Leaf(void) const;

	status_t SetTo(const char *path, const char *leaf = 0, bool normalize = false);
	status_t SetTo(const VDirectory *dir, const char *leaf = 0, bool normalize = false);
	status_t SetTo(const VEntry *entry);
	void Unset(void);

	virtual status_t Flatten(void *buffer, ssize_t size) const;
	virtual ssize_t FlattenedSize() const;
	virtual type_code TypeCode() const;
	virtual status_t Unflatten(type_code code, const void *buffer, ssize_t size);
	virtual bool IsFixedSize(void) const { return false; }
	virtual bool AllowsTypeCode(type_code code) const { return code == TypeCode(); }

	VPath& operator=(const VPath &path);
	VPath& operator=(const char *string);

	bool operator==(const VPath &path) const;
	bool operator==(const char *string) const;
	bool operator!=(const VPath &path) const;
	bool operator!=(const char *string) const;
};

} // namespace tekhne

#endif /* _VPATH_H */
