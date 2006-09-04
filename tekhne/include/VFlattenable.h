/***************************************************************************
 *            VBlockCache.h
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

#ifndef _VFLATTENABLE_H
#define _VFLATTENABLE_H

#include "StandardDefs.h"

namespace tekhne {

class VFlattenable {
private:
public:

	virtual ~VFlattenable() {}

	virtual status_t Flatten(void *buffer, ssize_t numBytes) const = 0;
	virtual status_t Unflatten(type_code code, const void *buffer, ssize_t numBytes) = 0;
	virtual ssize_t FlattenedSize(void) const = 0;
	virtual bool IsFixedSize(void) const = 0;
	virtual type_code TypeCode(void) const = 0;
	virtual bool AllowsTypeCode(type_code code) { return code == TypeCode(); }
};

} // namespace tekhne

#endif /* _VFLATTENABLE_H */
