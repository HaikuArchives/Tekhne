/***************************************************************************
 *            VMallocIO.cc
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

VMallocIO::VMallocIO() : _buf(0), _bufferLen(0), _curPosition(0), _blockSize(0), _lastUsed(0) {
	SetBlockSize(256);
	SetSize(_blockSize);
}

VMallocIO::~VMallocIO() {
	if (_buf) free(_buf);
}

const void *VMallocIO::Buffer(void) const {
	return _buf;
}

size_t VMallocIO::Length(void) const {
	return _lastUsed;
}

size_t VMallocIO::BufferLength(void) const {
	return _bufferLen;
}

ssize_t VMallocIO::Read(void *buffer, size_t numBytes) {
	return ReadAt(_curPosition, buffer, numBytes);
}

ssize_t VMallocIO::ReadAt(off_t position, void *buffer, size_t numBytes) {

	if (position > (off_t)_bufferLen) {
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

off_t VMallocIO::Seek(off_t position, int32_t mode) {
	if (mode == SEEK_SET) {
		SetSize(position);
		_curPosition = position;
	} else if (mode == SEEK_CUR) {
		SetSize(_curPosition+position);
		_curPosition += position;
	} else if (mode == SEEK_END) {
		off_t oldBufLen = _bufferLen;
		SetSize(_bufferLen+position);
		_curPosition = oldBufLen+position;
	}
	if (_curPosition < 0) _curPosition = 0;
	return Position();
}

off_t VMallocIO::Position(void) const {
	return _curPosition;
}

void VMallocIO::SetBlockSize(size_t blockSize) {
	if (blockSize >= 256) {
		this->_blockSize = blockSize;
	}
}

status_t VMallocIO::SetSize(off_t numBytes) {
	if (numBytes < (off_t)_bufferLen) return _bufferLen;

	off_t newBufferLen = _blockSize;
	while (newBufferLen < numBytes) {
		newBufferLen += _blockSize;
	}
	uint8_t *newBuf = (uint8_t *)malloc(newBufferLen);
	bzero(newBuf, newBufferLen);
	if (_buf) {
		memmove(newBuf, _buf, _bufferLen);
		free(_buf);
	}
	_buf = newBuf;
	_bufferLen = newBufferLen;
	return BufferLength();
}

ssize_t VMallocIO::Write(const void *buffer, size_t numBytes) {
	return WriteAt(_curPosition, buffer, numBytes);
}

ssize_t VMallocIO::WriteAt(off_t position, const void *buffer, size_t numBytes) {
	SetSize(position+numBytes);
	memmove(_buf+position, buffer, numBytes);
	_curPosition = position+numBytes;
	if ((size_t)_curPosition > _lastUsed) _lastUsed = _curPosition;
	return numBytes;
}
