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

#ifndef _VBLOCKCACHE_H
#define _VBLOCKCACHE_H

#include "VErrors.h"
#include "VList.h"

namespace tekhne {

const uint32_t V_OBJECT_CACHE = 0;
const uint32_t V_MALLOC_CACHE = 1;

class VBlockCache {
private:
	uint32_t type;
	size_t size;
	VList *memoryList;
public:
	VBlockCache(size_t count, size_t size, uint32_t type);
	~VBlockCache();

	void *Get(size_t size);
	void Save(void *pointer, size_t size);
};

} // namespace tekhne

#endif /* _VBLOCKCACHE_H */
