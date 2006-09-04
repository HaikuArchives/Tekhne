/***************************************************************************
 *            VStatable.cc
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
#include <utime.h>

using namespace tekhne;
using namespace std;

status_t VStatable::GetCreationTime(time_t *ctime) const {
	return V_ERROR;
}

status_t VStatable::SetCreationTime(time_t ctime) {
	return V_ERROR;
}

status_t VStatable::GetModificationTime(time_t *mtime) const {
	if (_path.Length() > 0) {
		struct stat _stat;

		if (stat(_path.String(), &_stat) == 0) {
			if (mtime) *mtime = _stat.st_mtime;
			return V_OK;
		}
		return V_ERROR;
	}
	return V_BAD_VALUE;
}

status_t VStatable::SetModificationTime(time_t mtime) {
	if (_path.Length() > 0) {
		struct utimbuf times;
		if (GetAccessTime(&times.actime) == 0) {
			times.modtime = mtime;
			return utime(_path.String(), &times);
		}
		return V_ERROR;
	}
	return V_BAD_VALUE;
}

status_t VStatable::GetAccessTime(time_t *atime) const {
	if (_path.Length() > 0) {
		struct stat _stat;

		if (stat(_path.String(), &_stat) == 0) {
			if (atime) *atime = _stat.st_atime;
			return V_OK;
		}
		return V_ERROR;
	}
	return V_BAD_VALUE;
}

status_t VStatable::SetAccessTime(time_t atime) {
	if (_path.Length() > 0) {
		struct utimbuf times;
		if (GetModificationTime(&times.modtime) == 0) {
			times.actime = atime;
			return utime(_path.String(), &times);
		}
		return V_ERROR;
	}
	return V_BAD_VALUE;
}

status_t VStatable::GetOwner(uid_t *owner) const {
	if (_path.Length() > 0) {
		struct stat _stat;

		if (stat(_path.String(), &_stat) == 0) {
			if (owner) *owner = _stat.st_uid;
			return V_OK;
		}
		return V_ERROR;
	}
	return V_BAD_VALUE;
}

status_t VStatable::SetOwner(uid_t owner) {
	if (_path.Length() > 0) {
		gid_t group;
		if (GetGroup(&group) == 0) {
			return chown(_path.String(), owner, group);
		}
		return V_ERROR;
	}
	return V_BAD_VALUE;
}

status_t VStatable::GetGroup(gid_t *group) const {
	if (_path.Length() > 0) {
		struct stat _stat;

		if (stat(_path.String(), &_stat) == 0) {
			if (group) *group = _stat.st_gid;
			return V_OK;
		}
		return V_ERROR;
	}
	return V_BAD_VALUE;
}

status_t VStatable::SetGroup(gid_t group) {
	if (_path.Length() > 0) {
		uid_t owner;
		if (GetOwner(&owner) == 0) {
			return chown(_path.String(), owner, group);
		}
		return V_ERROR;
	}
	return V_BAD_VALUE;
}

status_t VStatable::GetPermissions(mode_t *perms) const {
	if (_path.Length() > 0) {
		struct stat _stat;

		if (stat(_path.String(), &_stat) == 0) {
			if (perms) *perms = _stat.st_mode;
			return V_OK;
		}
		return V_ERROR;
	}
	return V_BAD_VALUE;
}

status_t VStatable::SetPermissions(mode_t perms) {
	if (_path.Length() > 0) {

		return chmod(_path.String(), perms);
	}
	return V_BAD_VALUE;
}

status_t VStatable::GetSize(off_t *size) const {
	if (_path.Length() > 0) {
		struct stat _stat;

		if (lstat(_path.String(), &_stat) == 0) {
			if (size) *size = _stat.st_size;
			return V_OK;
		}
		return V_ERROR;
	}
	return V_BAD_VALUE;
}

status_t VStatable::GetStat(struct stat *st) const {
	if (_path.Length() > 0) {
		if (st) {
			return lstat(_path.String(), st);
		}
	}
	return V_BAD_VALUE;
}

bool VStatable::IsFile(void) const {
	if (_path.Length() > 0) {
		struct stat _stat;

		if (stat(_path.String(), &_stat) == 0) {
			return S_ISREG(_stat.st_mode);
		}
		return false;
	}
	return false;
}

bool VStatable::IsDirectory(void) const {
	if (_path.Length() > 0) {
		struct stat _stat;

		if (stat(_path.String(), &_stat) == 0) {
			return S_ISDIR(_stat.st_mode);
		}
		return false;
	}
	return false;
}

bool VStatable::IsSymLink(void) const {
	if (_path.Length() > 0) {
		struct stat _stat;

		if (lstat(_path.String(), &_stat) == 0) {
			return S_ISLNK(_stat.st_mode);
		}
		return false;
	}
	return false;
}
