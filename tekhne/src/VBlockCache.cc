/***************************************************************************
 *            VBlockCache.cc
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
#include <iostream>

using namespace tekhne;
using namespace std;

typedef struct CacheItem {
	void *ptr;
	bool used;
	size_t size;
} CacheItem;

inline void *allocateMemory(size_t size, uint32_t type) {
	void *ptr;

	if (type == V_OBJECT_CACHE) {
		ptr = new int8_t[size];
	} else {
		ptr = malloc(size);
	}
	bzero(ptr, size);
	return ptr;
}

inline void freeMemory(void *ptr, uint32_t type) {
	if (ptr) {
		if (type == V_OBJECT_CACHE) {
			delete [] (int8_t *)ptr;
		} else {
			free(ptr);
		}
	}
}

inline void *allocateCacheItem(size_t size, uint32_t type) {
	CacheItem *ci = new CacheItem();
	ci->used = false;
	ci->size = size;
	ci->ptr = allocateMemory(size, type);
	return ci;
}

inline void freeCacheItem(CacheItem *ci, uint32_t type) {
	if (ci) {
		if(ci->used) {
			freeMemory(ci->ptr, type);
		}
		delete ci;
	}
}

VBlockCache::VBlockCache(size_t count, size_t size, uint32_t type) {
	_lock = new VLocker();
	if (count <= 0) count = 32;
	this->type = type;
	this->size = size;
	memoryList = new VList(count);
	for (uint i=0;i<count; i++) {
		memoryList->AddItem(allocateCacheItem(size, type), i);
	}
}

VBlockCache::~VBlockCache() {
	_lock->Lock( );
	if (_lock) {
		VListIterator iter(*memoryList);
		while(iter.HasNext()) {
			freeCacheItem(static_cast<CacheItem *>(iter.Next()), type);
		}
		delete memoryList;
		_lock->Unlock();
		_lock = 0;
		delete _lock;
	}
}

void *VBlockCache::Get(size_t size) {
	VAutoLock l(_lock);
	if (size == this->size) {
		VListIterator iter(*memoryList);
		while(iter.HasNext()) {
			CacheItem *ci = static_cast<CacheItem *>(iter.Next());
			if (!ci->used && ci->size == size) {
				ci->used = true;
				return ci->ptr;
			}
		}
	}
	// no more free ones
	CacheItem *ci = (CacheItem *)allocateCacheItem(size, type);
	memoryList->AddItem(ci);
	ci->used = true;
	return ci->ptr;
}

void VBlockCache::Save(void *pointer, size_t size) {
	VAutoLock l(_lock);
	VListIterator iter(*memoryList);
	while(iter.HasNext()) {
		CacheItem *ci = static_cast<CacheItem *>(iter.Next());
		// Clear even if not in use. Is someone wrote to it who wasn't supposed
		// to then we need to guarantee that the buffer is zeroed
		if (ci->ptr == pointer) {
			ci->used = false;
			bzero(ci->ptr, ci->size);
		}
	}
}
