/***************************************************************************
		*            VStatable.h
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

#ifndef _VSTATABLE_H
#define _VSTATABLE_H

#include "StorageDefs.h"
#include <sys/stat.h>
#include <iostream>
namespace tekhne {

class VPath;

class VStatable {
protected:
	VPath *_path;
public:
	VStatable() : _path(0) {}
	virtual ~VStatable();
	status_t GetCreationTime(time_t *ctime) const;
	status_t SetCreationTime(time_t ctime);
	status_t GetModificationTime(time_t *mtime) const;
	status_t SetModificationTime(time_t mtime);
	status_t GetAccessTime(time_t *atime) const;
	status_t SetAccessTime(time_t atime);

	status_t GetOwner(uid_t *owner) const;
	status_t SetOwner(uid_t owner);
	status_t GetGroup(gid_t *group) const;
	status_t SetGroup(gid_t group);
	status_t GetPermissions(mode_t *perms) const;
	status_t SetPermissions(mode_t perms);

	status_t GetSize(off_t *size) const;

	virtual status_t GetStat(struct stat *st) const;

	const char *FullPath(void) const;

	bool IsFile(void) const;
	bool IsDirectory(void) const;
	bool IsSymLink(void) const;

	friend class VPath;
};

}// namespace tekhne

#endif /* _VSTATABLE_H */
