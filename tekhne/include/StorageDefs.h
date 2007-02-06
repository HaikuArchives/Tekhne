/***************************************************************************
 *            StorageDefs.h
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

#ifndef _STORAGEDEFS_H
#define _STORAGEDEFS_H

#include <stdint.h>
#include <fcntl.h>
#include <linux/limits.h>

namespace tekhne {

	const int32_t B_FILE_NAME_LENGTH = NAME_MAX;
	const int32_t B_PATH_NAME_LENGTH = PATH_MAX;
	const int32_t B_MIME_TYPE_LENGTH = 256;
	const int32_t B_MAX_SYMLINKS = 127; // LINK_MAX;

	const int32_t B_READ_ONLY = O_RDONLY;
	const int32_t B_WRITE_ONLY = O_WRONLY;
	const int32_t B_READ_WRITE = O_RDWR;
	const int32_t B_FAIL_IF_EXISTS = O_EXCL;
	const int32_t B_CREATE_FILE = O_CREAT;
	const int32_t B_ERASE_FILE = O_TRUNC;
	const int32_t B_OPEN_AT_END = O_APPEND;

	const int32_t V_FILE_NODE = 1;
	const int32_t V_SYMLINK_NODE = 2;
	const int32_t V_DIRECTORY_NODE = 4;
	const int32_t V_ANY_NODE = 7;

} // namespace tekhne

#endif /* _STORAGEDEFS_H */
