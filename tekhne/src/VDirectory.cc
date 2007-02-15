/***************************************************************************
 *            VDirectory.cc
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

VDirectory::VDirectory(const VEntry *entry) {
	SetTo(entry);
}

VDirectory::VDirectory(const char *path) : VEntry(path) {
	if (!IsDirectory()) {
		Unset();
	}
}

VDirectory::VDirectory(const VDirectory *dir, const char *path) : VEntry (dir, path) {
	if (!IsDirectory()) {
		Unset();
	}
}

VDirectory::VDirectory(void) {
}

VDirectory::VDirectory(const VDirectory &directory) : VEntry(directory) {
	if (!IsDirectory()) {
		Unset();
	}
}

bool VDirectory::Contains(const char *path, int32_t nodeFlags) const {
	if (!path) return false;
	VEntry e(path);
	return Contains(&e);
}

bool VDirectory::Contains(const VEntry *entry, int32_t nodeFlags) const {
	if (InitCheck() != V_OK) return V_NO_INIT;
	if (!entry) return V_BAD_VALUE;
	if (entry->InitCheck() != V_OK) return V_NO_INIT;
	if (!entry->Exists()) return false;
	// seach this and subdirectories for this path
	VEntry e1(*entry);
	VEntry e2;
	while(e1.GetParent(&e2) == V_OK) {
		if (*this == e2) {
			if (nodeFlags == V_ANY_NODE) return true;
			if (nodeFlags == V_SYMLINK_NODE && entry->IsSymLink()) return true;
			if (nodeFlags == V_DIRECTORY_NODE && entry->IsDirectory()) return true;
			if (nodeFlags == V_FILE_NODE && entry->IsFile()) return true;
			return false;
		}
		e1 = e2;
	}
	return false;
}

status_t VDirectory::CreateFile(const char *path, VFile *file, bool failIfExists) {
	if (!path) return V_BAD_VALUE;
	status_t err;
	VEntry e;
	if (*path == '/') e.SetTo(path);
	else e.SetTo(this, path);
	err = e.InitCheck();
	if (err == V_OK) {
		if (file) {
			if (failIfExists) {
				err = file->SetTo(&e, O_RDWR|O_CREAT|O_TRUNC|O_EXCL);
			} else {
				err = file->SetTo(&e, O_RDWR|O_CREAT|O_TRUNC);
			}
		} else {
			VFile f;
			if (failIfExists) {
				err = f.SetTo(&e, O_RDWR|O_CREAT|O_TRUNC|O_EXCL);
			} else {
				err = f.SetTo(&e, O_RDWR|O_CREAT|O_TRUNC);
			}
		}
	}
	return err;
}

status_t VDirectory::CreateDirectory(const char *path, VDirectory *dir) {
	return V_ERROR;
}

status_t VDirectory::CreateSymLink(const char *path, const char *linkToPath, VSymLink *link) {
	return V_ERROR;
}

status_t VDirectory::FindEntry(const char *path, VEntry *entry, bool traverse) const {
	return V_ERROR;
}

status_t VDirectory::GetEntry(VEntry *entry) const {
	return V_ERROR;
}

status_t VDirectory::GetNextEntry(VEntry *entry, bool traverse) {
	return V_ERROR;
}

int32_t VDirectory::GetNextDirents(dirent *buf, size_t bufsize, int32_t count) {
	return V_ERROR;
}

int32_t VDirectory::CountEntries(void) {
	return V_ERROR;
}

status_t VDirectory::Rewind(void) {
	return V_ERROR;
}

status_t VDirectory::GetStatFor(const char *path, struct stat *st) const {
	VEntry e(this, path);
	return e.GetStat(st);
}

bool VDirectory::IsRootDirectory(void) const {
	if (InitCheck() != V_OK) return false;
	return (strcmp(_path->FullPath(), "/") == 0);
}

status_t VDirectory::SetTo(const VEntry *entry) {
	if (!entry) return V_BAD_VALUE;
	if (entry->InitCheck() != V_OK) return V_NO_INIT;
	VPath p(entry);
	status_t err = p.InitCheck();
	if (err == V_OK) {
		err = VEntry::SetTo(p.FullPath());
		if (err == V_OK) {
			if (!IsDirectory()) {
				Unset();
				err = V_ENTRY_NOT_FOUND;
			}
		}
	}
	return err;
}

status_t VDirectory::SetTo(const char *path) {
	status_t err = VEntry::SetTo(path);
	if (err == V_OK) {
		if (!IsDirectory()) {
			Unset();
			err = V_ENTRY_NOT_FOUND;
		}
	}
	return err;
}

status_t VDirectory::SetTo(const VDirectory *dir, const char *path) {
	VEntry e(dir, path);
	status_t err = e.InitCheck();
	if (err == V_OK) {
		err = SetTo(&e);
	}
	return err;
}

void VDirectory::Unset(void) {
	VEntry::Unset();
}

VDirectory& VDirectory::operator=(const VDirectory &directory) {
	if (this == &directory) return *this;
	Unset();
	SetTo(&directory);
	return *this;
}

status_t create_directory(const char *path, mode_t mode) {
	if(mkdir (path, mode)) return errno;
	return V_OK;
}

