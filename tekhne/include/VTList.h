/***************************************************************************
 *            VList.h
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
 ***************************************************************************/
#ifndef VTLIST_H_
#define VTLIST_H_

#include <VErrors.h>
#include <string.h>

namespace tekhne {

template <class T>
class VTList {
	private:

	// the items
	T *items;
	// the length of the buffer
	int32_t bufferLen;
	// the index of the last item in the buffer or -1 if there are no items
	int32_t lastItem;

	// allocate a buffer at least as big as asked for. (Usually 2^x where 2^x is
	// the smallest 2^x larger than count)
	inline void makeBuffer(int32_t count) {
		int32_t len = 16;
		while(len < count) len *=2;

		bufferLen = len;
		items = new T[len];
		bzero(items, sizeof(T)*bufferLen);
	}

	public:

		VTList(int32_t count = 20);
		VTList(const VTList<T>& anotherList);
		virtual ~VTList();

		bool AddItem(T item, int32_t index);
		bool AddItem(T item);

		bool AddList(VTList<T> *list, int32_t index);
		bool AddList(VTList<T> *list);

		int32_t CountItems(void) const;

		void DoForEach(bool (*func)(T));
		void DoForEach(bool (*func)(T, void *), void *arg2);

		T FirstItem(void) const;

		T GetItem(int32_t index) const;

		bool HasItem(T item) const;

		int32_t IndexOf(T item) const;

		bool IsEmpty(void) const;

		T* Items(void) const;

		T LastItem(void) const;

		void MakeEmpty(void);

		bool RemoveItem(T item);
		T RemoveItem(int32_t index);
		bool RemoveItems(int32_t index, int32_t count);

		T ReplaceItem(int32_t index, T item);

		void SortItems(int (*compareFunc)(const void *, const void *));

		VTList<T>& operator=(const VTList<T> &l);

};

} // namespace tekhne

#include "VTList.cc"

#endif /*VTLIST_H_*/
