/***************************************************************************
 *            VFile.cc
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
#include <stdio_ext.h>

using namespace tekhne;

VFile::VFile(void) : _fd(0), _f(0) {
}

VFile::VFile(const VFile &file) : _fd(0), _f(0) {
	SetTo(&file, file._openMode);
}

VFile::VFile(const VEntry *entry, uint32_t openMode) : _fd(0), _f(0) {
	SetTo(entry, openMode);
}

VFile::VFile(const char *path, uint32_t openMode) : _fd(0), _f(0) {
	SetTo(path, openMode);
}

VFile::VFile(VDirectory *dir, const char *path, uint32_t openMode) : _fd(0), _f(0) {
	SetTo(dir, path, openMode);
}

VFile::~VFile() {
	Unset();
}

status_t VFile::GetSize(off_t *size) const {
	return VEntry::GetSize(size);
}

status_t VFile::SetSize(off_t size) {
	if (InitCheck() != V_OK) return V_NO_INIT;
	if (truncate(_path->FullPath(), size)) return errno;
	return V_OK;
}

status_t VFile::InitCheck(void) const {
	if (_fd < 0 || VEntry::InitCheck() != V_OK) return V_NO_INIT;
	return V_OK;
}

bool VFile::IsReadable(void) const {
	if (InitCheck() != V_OK) return false;
	return __freadable(_f);
}

bool VFile::IsWritable(void) const {
	if (InitCheck() != V_OK) return false;
	return __fwritable(_f);
}


ssize_t VFile::Read(void *buffer, size_t size) {
	if (!buffer || InitCheck() != V_OK) return -1;
	return read(_fd, buffer, size);
}

ssize_t VFile::ReadAt(off_t location, void *buffer, size_t size) {
	if (!buffer || InitCheck() != V_OK) return -1;
	lseek(_fd, location, SEEK_SET);
	return read(_fd, buffer, size);
}

ssize_t VFile::Write(const void *buffer, size_t size) {
	if (!buffer || size < 0) return V_BAD_VALUE;
	return write(_fd, buffer, size);
}

ssize_t VFile::WriteAt(off_t location, const void *buffer, size_t size) {
	if (!buffer || size < 0) return V_BAD_VALUE;
	lseek(_fd, location, SEEK_SET);
	return write(_fd, buffer, size);
}

off_t VFile::Seek(off_t offset, int32_t seekMode) {
	if (InitCheck() == V_OK) {
		return lseek(_fd, offset, seekMode);
	}
	return -1;
}

off_t VFile::Position(void) const {
	if (InitCheck() != V_OK) return V_NO_INIT;
	return lseek(_fd, 0, SEEK_CUR);
}

status_t VFile::SetTo(const VEntry *entry, uint32_t openMode) {
	if (!entry) return V_BAD_VALUE;
	VPath p;
	status_t err = entry->GetPath(&p);
	if (err == V_OK) {
		err = SetTo(p.FullPath(), openMode);
	}
	return err;
}

status_t VFile::SetTo(const VPath &path, uint32_t openMode) {
	return SetTo(path.FullPath(), openMode);
}

status_t VFile::SetTo(const char *path, uint32_t openMode) {
	VEntry::SetTo(path);
	status_t err = VEntry::InitCheck();
	if (err == V_OK) {
		_openMode = openMode;
		if (Exists() && !IsFile() && !IsSymLink()) {
			err = V_NO_INIT;
		} else {
			if (openMode & O_CREAT) {
				_fd = open(path, openMode, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
			} else {
				_fd = open(path, openMode);
			}
			if (_fd < 0) {
				err = errno;
			} else {
				_open_stream();
				if (!_f) {
					err = errno;
				}
			}
		}
	}
	if (err != V_OK) Unset();
	return err;
}

status_t VFile::SetTo(const VDirectory *dir, const char *path, uint32_t openMode) {
	if (!dir || !path) return V_BAD_VALUE;
	VPath p(dir, path);
	int32_t err = p.InitCheck();
	if (err == V_OK) {
		err = SetTo(p.FullPath(), openMode);
	}
	return err;
}

void VFile::Unset(void) {
	if (_f) {
		fclose(_f);
		_fd = -1;
		_f = 0;
	} else if (_fd > 0) {
		close(_fd);
		_fd = -1;
	}
	_openMode = 0;
	VEntry::Unset();
}

VFile& VFile::operator=(const VFile &file) {
	if (this == &file) return *this;
	SetTo(&file, file._openMode);
	return *this;
}
