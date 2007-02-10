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

using namespace tekhne;

VFile::VFile(void) {
}

VFile::VFile(const VFile &file) {
	//SetTo(&file);
}

VFile::VFile(const VEntry *entry, uint32_t openMode) {
	SetTo(entry, openMode);
}

VFile::VFile(const char *path, uint32_t openMode) {
	SetTo(path, openMode);
}

VFile::VFile(VDirectory *dir, const char *path, uint32_t openMode) {
	SetTo(dir, path, openMode);
}

VFile::~VFile() {
	Unset();
}

status_t VFile::GetSize(off_t *size) const {
	return VEntry::GetSize(size);
}

status_t VFile::SetSize(off_t size) {
	if (InitCheck() == V_OK) return V_NO_INIT;
	VString s(_path->Path());
	s += "/";
	s += _path->Leaf();
	if (truncate(s.String(), size)) return errno;
	return V_OK;
}

status_t VFile::InitCheck(void) const {
	if (_fd < 0 || openMode == 0 || VEntry::InitCheck() != V_OK) return V_NO_INIT;
	return V_OK;
}

bool VFile::IsReadable(void) const {
	return !(openMod & O_WRONLY);
}

bool VFile::IsWritable(void) const {
	return openMode & O_WRONLY || openMode & O_RDWR;
}


ssize_t VFile::Read(void *buffer, size_t size) {
	return 0;
}

ssize_t VFile::ReadAt(off_t location, void *buffer, size_t size) {
	return 0;
}

ssize_t VFile::Write(const void *buffer, size_t size) {
	return 0;
}

ssize_t VFile::WriteAt(off_t location, const void *buffer, size_t size) {
	return 0;
}

off_t VFile::Seek(off_t offset, int32_t seekMode) {
	if (InitCheck() == V_OK) {
		return lseek(_fd, offset, seekMode);
	}
	return -1;
}

off_t VFile::Position(void) const {
	return 0;
}

status_t VFile::SetTo(const VEntry *entry, uint32_t openMode) {
	return V_ERROR;
}

status_t VFile::SetTo(const char *path, uint32_t openMode) {
	VEntry::SetTo(path);
	if (VEntry::InitCheck()) {
		_openMode = openMode;
		_fd = open(path, openMode);
		if (_fd < 0) {
			Unset();
			return errno;
		}
	} else {
		Unset();
		return V_NO_INIT;
	}
	return V_OK;
}

status_t VFile::SetTo(const VDirectory *dir, const char *path, uint32_t openMode) {
	return V_ERROR;
}

void VFile::Unset(void) {
	if (_fd > 0) {
		close(_fd);
		_fd = -1;
	}
	_openMode = 0;
	VEntry::Unset();
}

VFile& VFile::operator=(const VFile &File) {
	return *this;
}
