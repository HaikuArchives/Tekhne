/***************************************************************************
 *            VTList.cc
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

using namespace tekhne;

template <class T>
VTList<T>::VTList(int32_t count){
	makeBuffer(count);
	lastItem = -1;
}

template <class T>
VTList<T>::VTList(const VTList<T>& anotherList) {
	makeBuffer(anotherList.bufferLen);
	lastItem = anotherList.lastItem;

	memmove(items, &anotherList.items, sizeof(T)*(anotherList.lastItem+1));
}

template <class T>
VTList<T>::~VTList() {
	delete items;
}

template <class T>
bool VTList<T>::AddItem(T item, int32_t index) {
	if (index >= bufferLen) {
		T *oldItems = items;
		int32_t oldLen =  bufferLen;
		makeBuffer(index);
		memmove(items, oldItems, sizeof(T)*oldLen);
		delete [] oldItems;
	} else if (items[index]) {
		if (lastItem+1 > bufferLen) {
			T *oldItems = items;
			int32_t oldLen =  bufferLen;
			makeBuffer(bufferLen+1);
			memmove(items, oldItems, sizeof(T)*oldLen);
		}
		memmove(items+index+1, items+index, sizeof(T)*(lastItem-index+1));
		lastItem++;
	}
	items[index] = item;
	if (index > lastItem) {
		lastItem = index;
	}
	return true;
}

template <class T>
bool VTList<T>::AddItem(T item) {
	if (lastItem >= bufferLen) {
		T *oldItems = items;
		int32_t oldLen =  bufferLen;
		makeBuffer(bufferLen+1);
		memmove(items, oldItems, sizeof(T)*oldLen);
	}
	lastItem++;
	items[lastItem] = item;
	return true;
}

template <class T>
bool VTList<T>::AddList(VTList<T> *list, int32_t index) {
	for (int32_t i=0;i<=list->lastItem;i++) {
		AddItem(list->items[i], index+i);
	}
	return true;
}

template <class T>
bool VTList<T>::AddList(VTList<T> *list) {
	for (int32_t i=0;i<=list->lastItem;i++) {
		AddItem(list->items[i]);
	}
	return true;
}

template <class T>
int32_t VTList<T>::CountItems(void) const {
	int count = 0;
	for (int32_t i=0;i<=lastItem;i++) {
		if (items[i]) {
			count++;
		}
	}
	return count;
}

template <class T>
void VTList<T>::DoForEach(bool (*func)(T))  {
	if (func) {
		for (int32_t i=0;i<=lastItem;i++) {
			if (items[i]) {
				if (func(items[i])) {
					return;
				}
			}
		}
	}
}

template <class T>
void VTList<T>::DoForEach(bool (*func)(T, void *), void *arg2) {
	if (func) {
		for (int32_t i=0;i<=lastItem;i++) {
			if (items[i]) {
				func(items[i], arg2);
			}
		}
	}
}

template <class T>
T VTList<T>::FirstItem(void) const {
	int32_t i = 0;
	while (i <= lastItem) {
		if (items[i]) {
			return items[i];
		}
	}
	return 0;
}

template <class T>
T VTList<T>::GetItem(int32_t index) const {
	if (index > bufferLen || index < 0) return 0;
	return items[index];
}

template <class T>
bool VTList<T>::HasItem(T item) const {
	for (int32_t i=0;i<=lastItem;i++) {
		if (items[i] == item) {
			return true;
		}
	}
	return false;
}

template <class T>
int32_t VTList<T>::IndexOf(T item) const {
	for (int32_t i=0;i<=lastItem;i++) {
		if (items[i] == item) {
			return i;
		}
	}
	return -1;
}

template <class T>
bool VTList<T>::IsEmpty(void) const{
	for (int32_t i=0;i<=lastItem;i++) {
		if (items[i]) {
			return false;
		}
	}
	return true;
}

template <class T>
T* VTList<T>::Items(void) const {
	return items;
}

template <class T>
T VTList<T>::LastItem(void) const {
	for (int32_t i=lastItem;i>=0;i--) {
		if (items[i]) {
			return items[i];
		}
	}
	return 0;
}

template <class T>
void VTList<T>::MakeEmpty(void) {
	lastItem = -1;
	bzero(items, sizeof(T)*bufferLen);
}

template <class T>
bool VTList<T>::RemoveItem(T item) {
	int32_t idx = IndexOf(item);
	if (idx >= 0) {
		RemoveItem(idx);
		return true;
	}
	return false;
}

template <class T>
T VTList<T>::RemoveItem(int32_t index) {
	if (index <= lastItem) {
		T ans = items[index];
		memmove(items, items+1, sizeof(T)*(lastItem-index));
		lastItem--;
		return ans;
	}
	return 0;
}

template <class T>
bool VTList<T>::RemoveItems(int32_t index, int32_t count) {
	for(int i=0;i<count;i++) {
		if (!RemoveItem(index)) {
			return false;
		}
	}
	return true;
}

template <class T>
T VTList<T>::ReplaceItem(int32_t index, T item) {
	T reply = 0;
	if (index >= 0 && index < bufferLen) {
		reply = items[index];
	}
	if (index >= bufferLen) {
		T * oldItems = items;
		int32_t oldLen =  bufferLen;
		makeBuffer(index);
		memmove(items, oldItems, sizeof(T)*oldLen);
		delete [] oldItems;
	}
	items[index] = item;
	if (index > lastItem) {
		lastItem = index;
	}
	return reply;
}

template <class T>
void VTList<T>::SortItems(int (*compareFunc)(const void *, const void *)) {
	int32_t count = CountItems();
	if (count > 1) {
		qsort(items, count, sizeof(T), compareFunc);
	}
}

template <class T>
VTList<T>& VTList<T>::operator =(const VTList<T> &l) {
	makeBuffer(l.bufferLen);
	lastItem = l.lastItem;

	memmove(items, &l.items, sizeof(T)*(l.lastItem+1));
	return *this;
}
