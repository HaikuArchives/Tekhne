/***************************************************************************
 *            VSymLink.h
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

#ifndef _VSYMLINK_H
#define _VSYMLINK_H

#include "VErrors.h"

namespace tekhne {

class VSymLink : public VEntry {
private:
public:
	VSymLink(void) {}
	VSymLink(const VSymLink &link);
	VSymLink(const VPath *path);
	VSymLink(const VEntry *entry);
	VSymLink(const char *path) { SetTo(path); if (!IsSymLink()) Unset(); }
	VSymLink(const VDirectory *dir, const char *path) { SetTo(dir, path); if (!IsSymLink()) Unset(); }

	virtual ~VSymLink() {}

	// Unlike on the BeOS VSymLinks are alwas absolute.
	bool IsAbsolute(void) { return true; }

	// Since VSymLinks are always absolute we don't have to tell the VSymLink
	// what directory it is in.
	status_t MakeLinkedPath(VPath *path) const;

	size_t ReadLink(char *buf, size_t length);
	// over ride from statable so we don't follow the link
	virtual status_t GetStat(struct stat *st) const;
};

}

#endif // _VSYMLINK_H
