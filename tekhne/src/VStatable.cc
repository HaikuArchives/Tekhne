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
#include "utime.h"

using namespace tekhne;

VStatable::~VStatable() {
	delete _path;
}

status_t VStatable::GetCreationTime(time_t *ctime) const {
	if (!ctime) return V_BAD_VALUE;
	struct stat st;
	status_t s = GetStat(&st);
	if (s == V_OK) {
		*ctime = st.st_ctime;
	}
	return s;
}

status_t VStatable::SetCreationTime(time_t ctime) {
	// we cant change this...
	return V_ERROR;
}

status_t VStatable::GetModificationTime(time_t *mtime) const {
	if (!mtime) return V_BAD_VALUE;
	struct stat st;
	status_t s = GetStat(&st);
	if (s == V_OK) {
		*mtime = st.st_mtime;
	}
	return s;
}

status_t VStatable::SetModificationTime(time_t mtime) {
	if (!_path || _path->InitCheck() != V_OK) return V_NO_INIT;
	struct utimbuf utb;
	struct stat st;
	VString p(_path->Path());
	p += "/";
	p += _path->Leaf();
	if (stat(p.String(), &st)) return errno;
	utb.actime = st.st_atime;
	utb.modtime = mtime;
	if (utime(p.String(), &utb)) return errno;
	return V_OK;
}

status_t VStatable::GetAccessTime(time_t *atime) const {
	if (!atime) return V_BAD_VALUE;
	struct stat st;
	status_t s = GetStat(&st);
	if (s == V_OK) {
		*atime = st.st_atime;
	}
	return s;
}

status_t VStatable::SetAccessTime(time_t atime) {
	if (!_path || _path->InitCheck() != V_OK) return V_NO_INIT;
	struct utimbuf utb;
	struct stat st;
	VString p(_path->Path());
	p += "/";
	p += _path->Leaf();
	if (stat(p.String(), &st)) return errno;
	utb.actime = atime;
	utb.modtime = st.st_mtime;
	if (utime(p.String(), &utb)) return errno;
	return V_OK;
}

status_t VStatable::GetOwner(uid_t *owner) const {
	if (!owner) return V_BAD_VALUE;
	struct stat st;
	status_t s = GetStat(&st);
	if (s == V_OK) {
		*owner = st.st_uid;
	}
	return s;
}

status_t VStatable::SetOwner(uid_t owner) {
	gid_t g;
	status_t err = GetGroup(&g);
	if (err == V_OK) {
		VString p(_path->Path());
		p += "/";
		p += _path->Leaf();
		if (chown (p.String(), owner, g)) err = errno;
	}
	return err;
}

status_t VStatable::GetGroup(gid_t *group) const {
	if (!group) return V_BAD_VALUE;
	struct stat st;
	status_t s = GetStat(&st);
	if (s == V_OK) {
		*group = st.st_gid;
	}
	return s;
}

status_t VStatable::SetGroup(gid_t group) {
	uid_t o;
	status_t err = GetOwner(&o);
	if (err == V_OK) {
		VString p(_path->Path());
		p += "/";
		p += _path->Leaf();
		if (chown (p.String(), o, group)) err = errno;
	}
	return err;
}

status_t VStatable::GetPermissions(mode_t *perms) const {
	if (!perms) return V_BAD_VALUE;
	struct stat st;
	status_t s = GetStat(&st);
	if (s == V_OK) {
		*perms = st.st_mode & (S_IRWXU|S_IRWXG|S_IRWXO);
	}
	return s;
}

status_t VStatable::SetPermissions(mode_t perms) {
	if (!_path || _path->InitCheck() != V_OK) return V_NO_INIT;
	perms = perms & (S_IRWXU|S_IRWXG|S_IRWXO);
	VString p(_path->Path());
	p += "/";
	p += _path->Leaf();
	if (chmod(p.String(), perms)) return errno;
	return V_OK;
}

status_t VStatable::GetSize(off_t *size) const {
	if (!size) return V_BAD_VALUE;
	struct stat st;
	status_t s = GetStat(&st);
	if (s == V_OK) {
		*size = st.st_size;
	}
	return s;
}

status_t VStatable::GetStat(struct stat *st) const {
	if (!_path || _path->InitCheck() != V_OK) return V_NO_INIT;
	VString p(_path->Path());
	p += "/";
	p += _path->Leaf();
	if (stat(p.String(), st)) return errno;
	return V_OK;
}

bool VStatable::IsFile(void) const {
	struct stat st;
	status_t s = GetStat(&st);
	if (s == V_OK) {
		return S_ISREG(st.st_mode);
	}
	return false;
}

bool VStatable::IsDirectory(void) const {
	struct stat st;
	status_t s = GetStat(&st);
	if (s == V_OK) {
		return S_ISDIR(st.st_mode);
	}
	return false;
}

bool VStatable::IsSymLink(void) const {
	if (!_path || _path->InitCheck() != V_OK) return V_NO_INIT;
	VString p(_path->Path());
	p += "/";
	p += _path->Leaf();
	struct stat st;
	if (!lstat(p.String(), &st)){
		return S_ISLNK(st.st_mode);
	}
	return false;
}
