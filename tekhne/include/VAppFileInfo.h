/***************************************************************************
 *            AppFileInfo.h
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

#ifndef _VAPPFILEINFO_H
#define _VAPPFILEINFO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif



#ifdef __cplusplus
}
#endif

namespace tekhne {

const int32_t B_APP_VERSION_KIND = 1;
const int32_t B_SYSTEM_VERSION_KIND = 2;

typedef struct version_info {
	uint32_t major;
	uint32_t middle;
	uint32_t minor;
	uint32_t variety;
	uint32_t internal;
	char short_info[64];
	char long_info[256];
} version_info;

} // namespace tekhne
#endif /* _VAPPFILEINFO_H */
