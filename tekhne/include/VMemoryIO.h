/***************************************************************************
 *            VMemoryIO.h
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

#ifndef _VMEMORYIO_H
#define _VMEMORYIO_H

#include "VPositionIO.h"
#include <unistd.h>

namespace tekhne {

class VMemoryIO : public VPositionIO {
	// size is 256 bytes
private:
	uint8_t *buf;
	const uint8_t *constBuf;
	size_t bufferLen;
	off_t curPosition;

public:
	VMemoryIO(void *buffer, size_t numBytes);
	VMemoryIO(const void *buffer, size_t numBytes);
	virtual ~VMemoryIO();

	const void *Buffer(void) const;
	size_t BufferLength(void) const;
	virtual ssize_t Read(void *buffer, size_t numBytes);
	virtual ssize_t ReadAt(off_t position, void *buffer, size_t numBytes);
	virtual off_t Seek(off_t position, int32_t mode);
	virtual off_t Position(void) const;
	virtual ssize_t Write(const void *buffer, size_t numBytes);
	virtual ssize_t WriteAt(off_t position, const void *buffer, size_t numBytes);
};

} // namespace tekhne

#endif /* _VMEMORYIO_H */
