/***************************************************************************
 *            VDictionary.cc
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

namespace tekhne {
typedef struct dict_item {
	uint32_t key;
	void *value;
	dict_item(uint32_t k, void *v) {
		key = k;
		value = v;
	}
} dict_item;
}

VDictionary::VDictionary() {
}

VDictionary::~VDictionary() {
	dict_item **di = static_cast<dict_item **>(items.Items());
	for (int i=0;i<items.CountItems(); i++) {
		delete di[i];
		di[i] = 0;
	}
}

status_t VDictionary::AddItem(VHashable &key, void *item) {
	items.AddItem(new dict_item(key.hash(), item));
	return V_OK;
}

void *VDictionary::FindItem(VHashable &k) {
	uint32_t key = k.hash();
	VListIterator iter(items);
	while(iter.HasNext()) {
		dict_item *item = static_cast<dict_item *>(iter.Next());
		if (item) {
			if (item->key == key) {
				return item->value;
			}
		}
	}
	return 0;
}

void *VDictionary::RemoveItem(VHashable &k) {
	uint32_t key = k.hash();
	VListIterator iter(items);
	while(iter.HasNext()) {
		dict_item *item = static_cast<dict_item *>(iter.Next());
		if (item) {
			if (item->key == key) {
				items.RemoveItem(item);
				void *reply = item->value;
				delete item;
				return reply;
			}
		}
	}
	return 0;
}

void *VDictionary::RemoveItem(void *value) {
	void *reply = 0;
	VListIterator iter(items);
	while(iter.HasNext()) {
		dict_item *item = static_cast<dict_item *>(iter.Next());
		if (item) {
			if (item->value == value) {
				items.RemoveItem(item);
				reply = item->value;
				delete item;
				break;
			}
		}
	}
	return reply;
}

void VDictionary::Items(VList &itemList) {
	itemList.MakeEmpty();
	VListIterator iter(items);
	while(iter.HasNext()) {
		dict_item *item = static_cast<dict_item *>(iter.Next());
		if (item) {
			itemList.AddItem(item->value);
		}
	}
}

const VInteger zero(0);
