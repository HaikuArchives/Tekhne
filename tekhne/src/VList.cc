/***************************************************************************
 *            VList.cc
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

VList::VList(int32_t cnt) : items(0), bufferLen(0), lastItem(-1), count(cnt) {
}

VList::VList(const VList& anotherList) : items(0), bufferLen(0), lastItem(-1), count(anotherList.count) {
	if (anotherList.items) {
		makeBuffer(anotherList.bufferLen);
		lastItem = anotherList.lastItem;
	
		memmove(items, &anotherList.items, sizeof(void*)*(anotherList.lastItem+1));
	}
}

VList::~VList() {
	delete [] items;
}

bool VList::AddItem(void *item, int32_t index) {
	checkBuffer();
	if (index >= bufferLen) {
		void ** oldItems = items;
		int32_t oldLen =  bufferLen;
		makeBuffer(index);
		memmove(items, oldItems, sizeof(void*)*oldLen);
		delete [] oldItems;
	} else if (items[index]) {
		if (lastItem+1 >= bufferLen) {
			void ** oldItems = items;
			int32_t oldLen =  bufferLen;
			makeBuffer(bufferLen+1);
			memmove(items, oldItems, sizeof(void*)*oldLen);
			delete [] oldItems;
		}
		memmove(items+index+1, items+index, sizeof(void*)*(lastItem-index+1));
		lastItem++;
	}
	items[index] = item;
	if (index > lastItem) {
		lastItem = index;
	}
	return true;
}

bool VList::AddItem(void *item) {
	checkBuffer();
	lastItem++;
	if (lastItem >= bufferLen) {
		void ** oldItems = items;
		int32_t oldLen =  bufferLen;
		makeBuffer(bufferLen+1);
		memmove(items, oldItems, sizeof(void*)*oldLen);
	}
	items[lastItem] = item;
	return true;
}

bool VList::AddList(VList *list, int32_t index) {
	if (list->items) {
		for (int32_t i=0;i<=list->lastItem;i++) {
			AddItem(list->items[i], index+i);
		}
	}
	return true;
}

bool VList::AddList(VList *list) {
	for (int32_t i=0;i<=list->lastItem;i++) {
		AddItem(list->items[i]);
	}
	return true;
}

int32_t VList::CountItems(void) const {
	int cnt = 0;
	if (items) {
		for (int32_t i=0;i<=lastItem;i++) {
			if (items[i]) {
				cnt++;
			}
		}
	}
	return cnt;
}

void VList::DoForEach(bool (*func)(void *))  {
	if (items && func) {
		for (int32_t i=0;i<=lastItem;i++) {
			if (items[i]) {
				if (func(items[i])) {
					return;
				}
			}
		}
	}
}

void VList::DoForEach(bool (*func)(void *, void *), void *arg2) {
	if (items && func) {
		for (int32_t i=0;i<=lastItem;i++) {
			if (items[i]) {
				func(items[i], arg2);
			}
		}
	}
}

void *VList::FirstItem(void) const {
	int32_t i = 0;
	while (i <= lastItem) {
		if (items[i]) {
			return items[i];
		}
	}
	return 0;
}

void *VList::GetItem(int32_t index) const {
	if (items && (index < bufferLen || index >= 0)) return items[index];
	return 0;
}

bool VList::HasItem(void *item) const {
	for (int32_t i=0;i<=lastItem;i++) {
		if (items[i] == item) {
			return true;
		}
	}
	return false;
}

int32_t VList::IndexOf(void *item) const {
	for (int32_t i=0;i<=lastItem;i++) {
		if (items[i] == item) {
			return i;
		}
	}
	return -1;
}

bool VList::IsEmpty(void) const{
	for (int32_t i=0;i<=lastItem;i++) {
		if (items[i]) {
			return false;
		}
	}
	return true;
}

void *VList::Items(void) const {
	return items;
}

void *VList::LastItem(void) const {
	for (int32_t i=lastItem;i>=0;i--) {
		if (items[i]) {
			return items[i];
		}
	}
	return 0;
}

void VList::MakeEmpty(void) {
	lastItem = -1;
	bufferLen = 0;
	delete [] items;
	items = 0;
}

bool VList::RemoveItem(void *item) {
	int32_t idx = IndexOf(item);
	if (idx >= 0) {
		RemoveItem(idx);
		return true;
	}
	return false;
}

void *VList::RemoveItem(int32_t index) {

	if (items && index <= lastItem && index >= 0) {
		void *ans = items[index];
		memmove(items, items+1, sizeof(void*)*(lastItem-index));
		lastItem--;
		return ans;
	}
	return 0;
}

bool VList::RemoveItems(int32_t index, int32_t cnt) {
	for(int i=0;i<cnt;i++) {
		if (!RemoveItem(index)) {
			return false;
		}
	}
	return true;
}

void *VList::ReplaceItem(int32_t index, void *item) {
	checkBuffer();
	void *reply = 0;
	if (index >= 0 && index < bufferLen) {
		reply = items[index];
	}
	if (index >= bufferLen) {
		void ** oldItems = items;
		int32_t oldLen =  bufferLen;
		makeBuffer(index);
		memmove(items, oldItems, sizeof(void*)*oldLen);
		delete [] oldItems;
	}
	items[index] = item;
	if (index > lastItem) {
		lastItem = index;
	}
	return reply;
}

void VList::SortItems(int (*compareFunc)(const void *, const void *)) {
	if (items) {
		int32_t cnt = CountItems();
		if (cnt > 1) {
			qsort(items, cnt, sizeof(void*), compareFunc);
		}
	}
}

VList& VList::operator =(const VList& l) {
	if (l.items) {
		makeBuffer(l.bufferLen);
		lastItem = l.lastItem;
	
		memmove(items, &l.items, sizeof(void*)*(l.lastItem+1));
	} else {
		MakeEmpty();
	}
	return *this;
}
