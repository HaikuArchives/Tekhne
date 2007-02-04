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

#ifndef _VDIRECTORY_H
#define _VDIRECTORY_H

#include <sys/stat.h>

namespace tekhne {

class VDictionary : VEntry, VEntryList {
private:

public:
	VDirectory(const BEntry *entry);
	VDirectory(const char *path);
	VDirectory(const VDirectory *dir, const char *path);
	VDirectory(void);
	VDirectory(const VDirectory &directory);

	virtual ~VDirectory(){}

	bool Contains(const char *path, int32 nodeFlags = B_ANY_NODE) const;
	bool Contains(const BEntry *entry, int32 nodeFlags = B_ANY_NODE) const;

	status_t CreateFile(const char *path, BFile *file, bool failIfExists = false);
	status_t CreateDirectory(const char *path, VDirectory *dir);
	status_t CreateSymLink(const char *path, const char *linkToPath, BSymLink *link);

	status_t FindEntry(const char *path, BEntry *entry, bool traverse = false) const;

	status_t GetEntry(BEntry *entry) const;

	virtual status_t GetNextEntry(BEntry *entry, bool traverse = false);
	virtual status_t GetNextRef(entry_ref *ref);
	virtual int32 GetNextDirents(dirent *buf, size_t bufsize, int32 count = INT_MAX);
	virtual int32 CountEntries(void);
	virtual status_t Rewind(void);

	status_t GetStatFor(const char *path, stat *st) const;

	bool IsRootDirectory(void) const;

	status_t SetTo(const BEntry *entry);
	status_t SetTo(const char *path);
	status_t SetTo(const VDirectory *dir, const char *path);
	void Unset(void);

	VDirectory& operator=(const VDirectory &directory);

};

status_t create_directory(const char *path, mode_t mode);

status_t find_directory(directory_which which, dev_t volume, bool create_it, char *path_string, int32 length);
status_t find_directory(directory_which which, BPath *path_obj, bool create_it = false, BVolume *volume = NULL);
}// namespace tekhne

#endif /* _VDIRECTORY_H */
