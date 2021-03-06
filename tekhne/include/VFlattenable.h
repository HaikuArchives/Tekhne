/***************************************************************************
 *            VFlattenable.h
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

namespace tekhne {

#include "StandardDefs.h"

class VFlattenable {
private:

public:
	virtual ~VFlattenable(){}

	virtual bool AllowsTypeCode(type_code code) const = 0;

	virtual status_t Flatten(void *address, ssize_t size) const = 0;

	virtual ssize_t FlattenedSize(void) const = 0;

	virtual type_code TypeCode() const = 0;

	virtual status_t Unflatten(type_code code, const void *buffer, ssize_t size) = 0;

	virtual bool IsFixedSize(void) const = 0;
};

}// namespace tekhne

#endif /* _VFLATTENABLE_H */
