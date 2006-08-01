/***************************************************************************
 *            VPositionIO.h
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

#ifndef _VPOSITIONIO_H
#define _VPOSITIONIO_H

#include "VDataIO.h"

namespace tekhne {
	
class VPositionIO : public VDataIO {
private:
public:
    inline VPositionIO(void) {};
    virtual ~VPositionIO(){};
	virtual ssize_t Read(void *buffer, size_t numBytes) = 0;
	virtual ssize_t ReadAt(off_t position, void *buffer, size_t numBytes) = 0;
	virtual off_t Seek(off_t position, int32_t mode) = 0;
	virtual off_t Position(void) const = 0;
	virtual status_t SetSize(off_t numBytes) { return 0; }
	virtual ssize_t Write(const void *buffer, size_t numBytes) = 0;
	virtual ssize_t WriteAt(off_t position, const void *buffer, size_t numBytes) = 0;
};

} // namespace tekhne

#endif /* _VPOSITIONIO_H */
