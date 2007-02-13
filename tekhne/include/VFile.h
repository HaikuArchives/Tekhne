/***************************************************************************
 *            VFile.h
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

#ifndef _VFILE_H
#define _VFILE_H

#include "VErrors.h"
#include <iostream>

namespace tekhne {

class VPath;
class VDirectory;

class VFile : public VEntry, VPositionIO {
private:
	int32_t _openMode;
	int32_t _fd;
	FILE *_f;

	inline void _open_stream() {
		VString mode("");
		if ((_openMode & O_ACCMODE) == O_RDWR) mode.SetTo("rw");
		else if ((_openMode & O_ACCMODE) == O_RDONLY) mode.SetTo("r");
		else if ((_openMode & O_ACCMODE) == O_WRONLY) mode.Append("w");
		// std::cout << "mode: " << mode.String() << std::endl;
		_f = fdopen(_fd, mode.String());
	}
public:
	VFile(void);
	VFile(const VFile &file);
	VFile(const VEntry *entry, uint32_t openMode);
	VFile(const char *path, uint32_t openMode);
	VFile(VDirectory *dir, const char *path, uint32_t openMode);

	virtual ~VFile();

	virtual status_t GetSize(off_t *size) const;
	virtual status_t SetSize(off_t size);

	status_t InitCheck(void) const;

	bool IsReadable(void) const;
	bool IsWritable(void) const;

	virtual ssize_t Read(void *buffer, size_t size);
	virtual ssize_t ReadAt(off_t location, void *buffer, size_t size);
	virtual ssize_t Write(const void *buffer, size_t size);
	virtual ssize_t WriteAt(off_t location, const void *buffer, size_t size);
	// wrapper for GNU getline
	inline ssize_t ReadLine(char **buffer, size_t *size) {
		if (!buffer || InitCheck() != V_OK) return -1;
		return getline(buffer, size, _f);
	}


	virtual off_t Seek(off_t offset, int32_t seekMode);
	virtual off_t Position(void) const;

	status_t SetTo(const VEntry *entry, uint32_t openMode);
	status_t SetTo(const char *path, uint32_t openMode);
	status_t SetTo(const VPath &path, uint32_t openMode);
	status_t SetTo(const VDirectory *dir, const char *path, uint32_t openMode);
	void Unset(void);

	VFile& operator=(const VFile &File);
};

} // namespace tekhne

#endif /* _VFILE_H */
