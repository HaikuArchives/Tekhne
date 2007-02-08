/***************************************************************************
 *            VEntry.h
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

#ifndef _VENTRY_H
#define _VENTRY_H

#include "VStatable.h"

namespace tekhne {

class VDirectory;
class VPath;

class VEntry : public VStatable {
private:
public:
	VEntry(const VDirectory *dir, const char *path, bool traverse = false);
	VEntry(const char *path, bool traverse = false);
	VEntry(void);
	VEntry(const VEntry &entry);

	virtual ~VEntry(){}

	bool Exists(void) const;
	status_t GetName(char *buffer) const;
	status_t GetPath(VPath *path) const;

	status_t GetParent(VEntry *entry) const;
	status_t GetParent(VDirectory *dir) const;

	status_t InitCheck(void) const;

	status_t Remove(void);

	status_t Rename(const char *path, bool clobber = false);
	status_t MoveTo(VDirectory *dir, const char *path = NULL, bool clobber = false);

	status_t SetTo(const char *path, bool traverse = false);
	status_t SetTo(const VDirectory *dir, const char *path, bool traverse = false);
	void Unset(void);

	VEntry& operator=(const VEntry &entry);

	bool operator==(const VEntry &entry) const;
	bool operator!=(const VEntry &entry) const;
};

}// namespace tekhne

#endif /* _VENTRY_H */
