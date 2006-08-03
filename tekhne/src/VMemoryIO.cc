/***************************************************************************
 *            VMemoryIO.cc
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

VMemoryIO::VMemoryIO(void *buffer, size_t numBytes) {
	buf = static_cast<uint8_t *>(buffer);
	constBuf = 0;
	bufferLen = numBytes;
	curPosition = 0;
}

VMemoryIO::VMemoryIO(const void *buffer, size_t numBytes) {
	buf = 0;
	constBuf = static_cast<uint8_t *>(const_cast<void *>(buffer));
	bufferLen = numBytes;
	curPosition = 0;
}

VMemoryIO::~VMemoryIO() {
}

const void *VMemoryIO::Buffer(void) const {
	return buf;
}

size_t VMemoryIO::BufferLength(void) const {
	return bufferLen;
}

ssize_t VMemoryIO::Read(void *buffer, size_t numBytes) {
	return ReadAt(curPosition, buffer, numBytes);
}

ssize_t VMemoryIO::ReadAt(off_t position, void *buffer, size_t numBytes) {
	
	if (position > (off_t)bufferLen) {
		return 0;
	}
	if (position+numBytes > bufferLen) {
		numBytes = bufferLen - position;
	}
	if (numBytes > 0) {
		memmove(buffer, buf+curPosition, numBytes);
	}
	curPosition = position+numBytes;
	return numBytes;
}

off_t VMemoryIO::Seek(off_t position, int32_t mode) {
	if (mode == SEEK_SET) {
		curPosition = position;
	} else if (mode == SEEK_CUR) {
		curPosition += position;
	} else if (mode == SEEK_END) {
		curPosition = bufferLen+position;
	}
	if (curPosition < 0) curPosition = 0;
	return Position();
}

off_t VMemoryIO::Position(void) const {
	return curPosition;
}

ssize_t VMemoryIO::Write(const void *buffer, size_t numBytes) {
	return WriteAt(curPosition, buffer, numBytes);
}

ssize_t VMemoryIO::WriteAt(off_t position, const void *buffer, size_t numBytes) {
	if (!buf) {
		return V_NOT_ALLOWED;
	}
	if (position < 0) return 0;
	if (position > (off_t)bufferLen) return 0;
	if (position+numBytes > bufferLen) numBytes = bufferLen - position;
	memmove(buf+position, buffer, numBytes);
	curPosition = position+numBytes;
	return numBytes;
}
