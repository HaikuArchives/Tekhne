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

// writing VMemoryIO
VMemoryIO::VMemoryIO(void *buffer, size_t numBytes): _buf(static_cast<uint8_t *>(buffer)), _constBuf(0),
	_bufferLen(numBytes), _curPosition(0), _lastUsed(0) {
}

// reading VMemoryIO
VMemoryIO::VMemoryIO(const void *buffer, size_t numBytes) : _buf(0), _constBuf(static_cast<uint8_t *>(const_cast<void *>(buffer))),
	_bufferLen(numBytes), _curPosition(0), _lastUsed(numBytes) {
}

VMemoryIO::~VMemoryIO() {
}

const void *VMemoryIO::Buffer(void) const {
	if (_buf) return _buf;
	return _constBuf;
}

size_t VMemoryIO::Length(void) const {
	return _lastUsed;
}

size_t VMemoryIO::BufferLength(void) const {
	return _bufferLen;
}

ssize_t VMemoryIO::Read(void *buffer, size_t numBytes) {
	return ReadAt(_curPosition, buffer, numBytes);
}

ssize_t VMemoryIO::ReadAt(off_t position, void *buffer, size_t numBytes) {

	if ((size_t)position > _bufferLen) {
		return 0;
	}
	if (position+numBytes > _bufferLen) {
		numBytes = _bufferLen - position;
	}
	if (numBytes > 0) {
		memmove(buffer, _buf+_curPosition, numBytes);
	}
	_curPosition = position+numBytes;
	return numBytes;
}

off_t VMemoryIO::Seek(off_t position, int32_t mode) {
	if (mode == SEEK_SET) {
		_curPosition = position;
	} else if (mode == SEEK_CUR) {
		_curPosition += position;
	} else if (mode == SEEK_END) {
		_curPosition = _bufferLen+position;
	}
	if (_curPosition < 0) _curPosition = 0;
	return _curPosition;
}

off_t VMemoryIO::Position(void) const {
	return _curPosition;
}

ssize_t VMemoryIO::Write(const void *buffer, size_t numBytes) {
	return WriteAt(_curPosition, buffer, numBytes);
}

ssize_t VMemoryIO::WriteAt(off_t position, const void *buffer, size_t numBytes) {
	if (!_buf) {
		return V_NOT_ALLOWED;
	}
	if (position < 0) return 0;
	if ((size_t)position > _bufferLen) return 0;
	if (position+numBytes > _bufferLen) numBytes = _bufferLen - position;
	memmove(_buf+position, buffer, numBytes);
	_curPosition = position+numBytes;
	if ((size_t)_curPosition > _lastUsed) _lastUsed = _curPosition;
	return numBytes;
}
