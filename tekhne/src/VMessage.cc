/***************************************************************************
 *            VMessage.cc
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

static VBlockCache blockCache(100, sizeof(VMessage), V_MALLOC_CACHE);

VMessage::VMessage(uint32_t command) : what(command){
}

VMessage::VMessage(const VMessage &message) : what(message.what) {
}

VMessage::VMessage(void) : what(0){
}

VMessage::~VMessage() {
	MakeEmpty();
}

union Value {
	bool b;
	int8_t i8;
	int16_t i16;
	int32_t i32;
	int64_t i64;
	float f;
	double d;
	const void *p;
};

typedef struct storage_item {
	char name[V_NAME_LENGTH+1];
	type_code type;
	Value data;
	ssize_t numBytes;
	bool fixedSize;
	int32_t numItems;
	inline void *copyBuffer(const void *b, int32_t len) {
		void *nb = malloc(len);
		memmove(nb, b, len);
		return nb;
	}
	
	storage_item(const char *name, type_code type, const void *data,
		ssize_t numBytes, bool fixedSize, int32_t numItems) {
		if ((int32_t)strlen(name) > V_NAME_LENGTH) {
			memmove(this->name, name, V_NAME_LENGTH);
			this->name[V_NAME_LENGTH] = 0;
		} else {
			bzero(this->name, V_NAME_LENGTH+1);
			strcpy(this->name, name);
		}
		this->type = type;
		switch(type) {
			case V_BOOL_TYPE:
				this->data.b = *((const bool *)data);
				break;
			case V_INT8_TYPE:
				this->data.i8 = *((const int8_t *)data);
				break;
			case V_INT16_TYPE:
				this->data.i16 = *((const int16_t *)data);
				break;
			case V_INT32_TYPE:
				this->data.i32 = *((const int32_t *)data);
				break;
			case V_INT64_TYPE:
				this->data.i64 = *((const int64_t *)data);
				break;
			case V_FLOAT_TYPE:
				this->data.f = *((const float *)data);
				break;
			case V_DOUBLE_TYPE:
				this->data.d = *((const double *)data);
				break;
			case V_POINTER_TYPE:
			case V_MESSAGE_TYPE:
			case V_MESSENGER_TYPE:
				this->data.p = data;
				break;
			default:
				this->data.p = copyBuffer(data, numBytes);
		}
		this->numBytes = numBytes;
		this->fixedSize = fixedSize;
		this->numItems = numItems;
	}
	virtual ~storage_item() {
		switch(type) {
			case V_BOOL_TYPE:
			case V_INT8_TYPE:
			case V_INT16_TYPE:
			case V_INT32_TYPE:
			case V_INT64_TYPE:
			case V_FLOAT_TYPE:
			case V_DOUBLE_TYPE:
			case V_POINTER_TYPE:
			case V_MESSAGE_TYPE:
			case V_MESSENGER_TYPE:
				break;
			default:
				free(const_cast<void *>(data.p));
		}
	}
} storage_item;

status_t VMessage::AddData(const char *name, type_code type, const void *data,
	ssize_t numBytes, bool fixedSize, int32_t numItems) {
	if(name != NULL && data != NULL && numBytes > 0) {
		if (numItems > 0) {
			if (l.AddItem(new storage_item(name, type, data, numBytes, fixedSize, numItems))) {
				return V_OK;
			}
		} else {
			return V_OK;
		}
	}
	return V_ERROR;
}

status_t VMessage::AddBool(const char *name, bool aBool) {
	return AddData(name, V_BOOL_TYPE, &aBool, sizeof(bool), true, 1);
}

status_t VMessage::AddInt8(const char *name, int8_t anInt8) {
	return AddData(name, V_INT8_TYPE, &anInt8, sizeof(int8_t), true, 1);
}

status_t VMessage::AddInt16(const char *name, int16_t anInt16) {
	return AddData(name, V_INT16_TYPE, &anInt16, sizeof(int16_t), true, 1);
}

status_t VMessage::AddInt32(const char *name, int32_t anInt32) {
	return AddData(name, V_INT32_TYPE, &anInt32, sizeof(int32_t), true, 1);
}

status_t VMessage::AddInt64(const char *name, int64_t anInt64) {
	return AddData(name, V_INT64_TYPE, &anInt64, sizeof(int64_t), true, 1);
}

status_t VMessage::AddFloat(const char *name, float aFloat) {
	return AddData(name, V_FLOAT_TYPE, &aFloat, sizeof(float), true, 1);
}

status_t VMessage::AddDouble(const char *name, double aDouble) {
	return AddData(name, V_DOUBLE_TYPE, &aDouble, sizeof(double), true, 1);
}

status_t VMessage::AddString(const char *name, const char *string) {
	return AddData(name, V_STRING_TYPE, string, strlen(string)+1, false, 1);
}

status_t VMessage::AddString(const char *name, const VString &string) {
	return AddString(name, string.String());
}

status_t VMessage::AddPoint(const char *name, VPoint point) {
	return V_ERROR;
}

status_t VMessage::AddRect(const char *name, VRect rect) {
	return V_ERROR;
}

status_t VMessage::AddMessage(const char *name, const VMessage *message) {
	return AddData(name, V_MESSAGE_TYPE, &message, sizeof(const void *), true, 1);
}

status_t VMessage::AddMessenger(const char *name, VMessenger *messenger) {
	return AddData(name, V_MESSENGER_TYPE, &messenger, sizeof(const void *), true, 1);
}

status_t VMessage::AddPointer(const char *name, const void *pointer) {
	return AddData(name, V_POINTER_TYPE, &pointer, sizeof(const void *), true, 1);
}

status_t VMessage::AddFlat(const char *name, VFlattenable *object, int32_t numItems) {
	return AddData(name, V_FLAT_TYPE, &object, sizeof(const void *), true, 1);
}

status_t VMessage::AddSpecifier(const VMessage *message) {
	return V_ERROR;
}

status_t VMessage::AddSpecifier(const char *property) {
	return V_ERROR;
}

status_t VMessage::AddSpecifier(const char *property, int32_t index) {
	return V_ERROR;
}

status_t VMessage::AddSpecifier(const char *property, int32_t index, int32_t range) {
	return V_ERROR;
}

status_t VMessage::AddSpecifier(const char *property, const char *name) {
	return V_ERROR;
}

int32_t VMessage::CountNames(type_code type) const {
	return 0;
}

status_t VMessage::FindData(const char *name, type_code type, int32_t index, const void **data, ssize_t *numBytes) const {
	for (int i=0;i<l.CountItems();i++) {
		storage_item *si = (storage_item *)l.GetItem(i);
		if (type == si->type && strcmp(name, si->name) == 0) {
			switch(type) {
				case V_BOOL_TYPE:
					**(bool**)data = si->data.b;
					break;
				case V_INT8_TYPE:
					**(int8_t**)data = si->data.i8;
					break;
				case V_INT16_TYPE:
					**(int16_t**)data = si->data.i16;
					break;
				case V_INT32_TYPE:
					**(int32_t**)data = si->data.i32;
					break;
				case V_INT64_TYPE:
					**(int64_t**)data = si->data.i64;
					break;
				case V_FLOAT_TYPE:
					**(float**)data = si->data.f;
					break;
				case V_DOUBLE_TYPE:
					**(double**)data = si->data.d;
					break;
				case V_POINTER_TYPE:
				case V_MESSAGE_TYPE:
				case V_MESSENGER_TYPE:
					*data = si->data.p;
					break;
				default:
					*data = si->data.p;
			}
			*numBytes = si->numBytes;
			if (index == 0) {
				return V_OK;
			}
			index--;
		}
	}
	return V_NAME_NOT_FOUND;
}

status_t VMessage::FindData(const char *name, type_code type, const void **data, ssize_t *numBytes) const {
	for (int i=0;i<l.CountItems();i++) {
		storage_item *si = (storage_item *)l.GetItem(i);
		if (type == si->type && strcmp(name, si->name) == 0) {
			switch(type) {
				case V_BOOL_TYPE:
					**(bool**)data = si->data.b;
					break;
				case V_INT8_TYPE:
					**(int8_t**)data = si->data.i8;
					break;
				case V_INT16_TYPE:
					**(int16_t**)data = si->data.i16;
					break;
				case V_INT32_TYPE:
					**(int32_t**)data = si->data.i32;
					break;
				case V_INT64_TYPE:
					**(int64_t**)data = si->data.i64;
					break;
				case V_FLOAT_TYPE:
					**(float**)data = si->data.f;
					break;
				case V_DOUBLE_TYPE:
					**(double**)data = si->data.d;
					break;
				case V_POINTER_TYPE:
				case V_MESSAGE_TYPE:
				case V_MESSENGER_TYPE:
					*data = si->data.p;
					break;
				default:
					*data = si->data.p;
			}
			*numBytes = si->numBytes;
			return V_OK;
		}
	}
	return V_NAME_NOT_FOUND;
}

status_t VMessage::FindBool(const char *name, int32_t index,  bool *aBool) const {
	ssize_t x;
	return FindData(name, V_BOOL_TYPE, index, (const void**)&aBool, &x);
}

status_t VMessage::FindBool(const char *name, bool *aBool) const {
	ssize_t x;
	return FindData(name, V_BOOL_TYPE, (const void**)&aBool, &x);
}

status_t VMessage::FindInt8(const char *name, int32_t index, int8_t *anInt8) const {
	ssize_t x;
	return FindData(name, V_INT8_TYPE, index, (const void**)&anInt8, &x);
}

status_t VMessage::FindInt8(const char *name, int8_t *anInt8) const {
	ssize_t x;
	return FindData(name, V_INT8_TYPE, (const void**)&anInt8, &x);
}

status_t VMessage::FindInt16(const char *name, int32_t index, int16_t *anInt16) const {
	ssize_t x;
	return FindData(name, V_INT16_TYPE, index, (const void**)&anInt16, &x);
}

status_t VMessage::FindInt16(const char *name, int16_t *anInt16) const {
	ssize_t x;
	return FindData(name, V_INT16_TYPE, (const void**)&anInt16, &x);
}

status_t VMessage::FindInt32(const char *name, int32_t index, int32_t *anInt32) const {
	ssize_t x;
	return FindData(name, V_INT32_TYPE, index, (const void**)&anInt32, &x);
}

status_t VMessage::FindInt32(const char *name, int32_t *anInt32) const {
	ssize_t x;
	return FindData(name, V_INT32_TYPE, (const void**)&anInt32, &x);
}

status_t VMessage::FindInt64(const char *name, int32_t index, int64_t *anInt64) const {
	ssize_t x;
	return FindData(name, V_INT64_TYPE, index, (const void**)&anInt64, &x);
}

status_t VMessage::FindInt64(const char *name, int64_t *anInt64) const {
	ssize_t x;
	return FindData(name, V_INT64_TYPE, (const void**)&anInt64, &x);
}

status_t VMessage::FindFloat(const char *name, int32_t index, float *aFloat) const {
	ssize_t x;
	return FindData(name, V_FLOAT_TYPE, index, (const void**)&aFloat, &x);
}

status_t VMessage::FindFloat(const char *name, float *aFloat) const {
	ssize_t x;
	return FindData(name, V_FLOAT_TYPE, (const void**)&aFloat, &x);
}

status_t VMessage::FindDouble(const char *name, int32_t index, double *aDouble) const {
	ssize_t x;
	return FindData(name, V_DOUBLE_TYPE, index, (const void**)&aDouble, &x);
}

status_t VMessage::FindDouble(const char *name, double *aDouble) const {
	ssize_t x;
	return FindData(name, V_DOUBLE_TYPE, (const void**)&aDouble, &x);
}

status_t VMessage::FindString(const char *name, int32_t index, const char **string) const {
	ssize_t x;
	return FindData(name, V_STRING_TYPE, index, (const void **)string, &x);
}

status_t VMessage::FindString(const char *name, const char **string) const {
	ssize_t x;
	return FindData(name, V_STRING_TYPE, (const void **)string, &x);
}

status_t VMessage::FindString(const char *name, VString *string) const {
	ssize_t x;
	char *s = NULL;
	status_t ans = FindData(name, V_STRING_TYPE, (const void **)&s, &x);
	if (ans == V_OK) {
		string->SetTo(s);
	}
	return ans;
}

status_t VMessage::FindString(const char *name, int32_t index, VString *string) const {
	ssize_t x;
	char *s = NULL;
	status_t ans = FindData(name, V_STRING_TYPE, index, (const void **)&s, &x);
	if (ans == V_OK) {
		string->SetTo(s);
	}
	return ans;
}

status_t VMessage::FindPoint(const char *name, int32_t index, VPoint *point) const {
	return V_ERROR;
}

status_t VMessage::FindPoint(const char *name, VPoint *point) const {
	return V_ERROR;
}

status_t VMessage::FindRect(const char *name, int32_t index, VRect *rect) const {
	return V_ERROR;
}

status_t VMessage::FindRect(const char *name, VRect *rect) const {
	return V_ERROR;
}


//status_t FindRef(const char *name, int32_t index, entry_ref *ref) const;
// status_t FindRef(const char *name, entry_ref *ref) const;

status_t VMessage::FindMessage(const char *name, int32_t index, VMessage *message) const {
	return V_ERROR;
}

status_t VMessage::FindMessage(const char *name, VMessage *message) const {
	return V_ERROR;
}

status_t VMessage::FindMessenger(const char *name, int32_t index, VMessenger *messenger) const {
	return V_ERROR;
}

status_t VMessage::FindMessenger(const char *name, VMessenger *messenger) const {
	return V_ERROR;
}

status_t VMessage::FindPointer(const char *name, int32_t index, void **pointer) const {
	ssize_t x;
	return FindData(name, V_POINTER_TYPE, index, (const void**)pointer, &x);
}

status_t VMessage::FindPointer(const char *name, void **pointer) const {
	ssize_t x;
	return FindData(name, V_POINTER_TYPE, (const void**)pointer, &x);
}

status_t VMessage::FindFlat(const char *name, int32_t index, VFlattenable *object) const {
	return V_ERROR;
}

status_t VMessage::FindFlat(const char *name, VFlattenable *object) const {
	return V_ERROR;
}


status_t VMessage::Flatten(VDataIO *object, ssize_t *numBytes) const {
	return V_ERROR;
}

status_t VMessage::Flatten(char *address, ssize_t numBytes) const {
	return V_ERROR;
}

status_t VMessage::Unflatten(VDataIO *object) {
	return V_ERROR;
}

status_t VMessage::Unflatten(const char *address) {
	return V_ERROR;
}

ssize_t VMessage::FlattenedSize(void) const {
	return 0;
}

status_t VMessage::GetCurrentSpecifier(int32_t *index, VMessage *specifier, int32_t *what, const char **property) const {
	return V_ERROR;
}

status_t VMessage::PopSpecifier(void) {
	return V_ERROR;
}

status_t VMessage::GetInfo(const char *name, type_code *typeFound, int32_t *countFound) const {
	return V_ERROR;
}

status_t VMessage::GetInfo(const char *name, type_code *typeFound, bool *fixedSize) const {
	return V_ERROR;
}

status_t VMessage::GetInfo(type_code type, int32_t index, char **nameFound, type_code *typeFound, int32_t *countFound) const {
	return V_ERROR;
}


bool VMessage::HasSpecifiers(void) const {
	return false;
}


bool VMessage::IsSystem(void) const {
	return false;
}

static bool deleteItem(void *item) {
	storage_item *si = (storage_item *)item;
	delete si;
	return false;
}

status_t VMessage::MakeEmpty(void) {
	l.DoForEach(deleteItem);
	l.MakeEmpty();
	return V_OK;
}

bool VMessage::IsEmpty(void) const {
	return l.IsEmpty();
}

void VMessage::PrintToStream(void) const {
	cout << "VMessage: "<< endl;
}


status_t VMessage::RemoveName(const char *name) {
	for (int i=l.CountItems()-1;i>0;i--) {
		storage_item *si = (storage_item *)l.GetItem(i);
		if (strcmp(name, si->name) == 0) {
			l.RemoveItem(i);
			delete si;
		}
	}
	return V_OK;
}

status_t VMessage::RemoveData(const char *name, int32_t index) {
	if (index < 0) {
		return V_BAD_INDEX;
	}
	for (int i=0; i<l.CountItems();i++) {
		storage_item *si = (storage_item *)l.GetItem(i);
		if (strcmp(name, si->name) == 0) {
			if (index == 0) {
				l.RemoveItem(i);
				delete si;
				return V_OK;
			} else {
				index--;
			}
		}
	}
	return V_NAME_NOT_FOUND;
}

status_t VMessage::ReplaceData(const char *name, type_code type, const void *data, ssize_t numBytes) {
	for (int i=0; i<l.CountItems();i++) {
		storage_item *si = (storage_item *)l.GetItem(i);
		if (type == si->type && strcmp(name, si->name) == 0) {
			l.ReplaceItem(i, new storage_item(name, type, data, numBytes, false, 1));
			delete si;
			return V_OK;
		}
	}
	return V_NAME_NOT_FOUND;
}

status_t VMessage::ReplaceData(const char *name, type_code type, int32_t index, const void *data, ssize_t numBytes) {
	if (index < 0) {
		return V_BAD_INDEX;
	}
	for (int i=0; i<l.CountItems();i++) {
		storage_item *si = (storage_item *)l.GetItem(i);
		if (type == si->type && strcmp(name, si->name) == 0) {
			if (index == 0) {
				l.ReplaceItem(i, new storage_item(name, type, data, numBytes, false, 1));
				delete si;
				return V_OK;
			} else {
				index--;
			}
		}
	}
	return V_NAME_NOT_FOUND;
}

status_t VMessage::ReplaceBool(const char *name, bool aBool) {
	return ReplaceData(name, V_BOOL_TYPE, &aBool, sizeof(bool));
}

status_t VMessage::ReplaceBool(const char *name, int32_t index, bool aBool) {
	return ReplaceData(name, V_BOOL_TYPE, index, &aBool, sizeof(bool));
}

status_t VMessage::ReplaceInt8(const char *name, int8_t anInt8) {
	return ReplaceData(name, V_INT8_TYPE, &anInt8, sizeof(int8_t));
}

status_t VMessage::ReplaceInt8(const char *name, int32_t index, int8_t anInt8) {
	return ReplaceData(name, V_INT8_TYPE, index, &anInt8, sizeof(int8_t));
}

status_t VMessage::ReplaceInt16(const char *name, int16_t anInt16) {
	return ReplaceData(name, V_INT16_TYPE, &anInt16, sizeof(int16_t));
}

status_t VMessage::ReplaceInt16(const char *name, int32_t index, int16_t anInt16) {
	return ReplaceData(name, V_INT16_TYPE, index, &anInt16, sizeof(int16_t));
}

status_t VMessage::ReplaceInt32(const char *name, long anInt32) {
	return ReplaceData(name, V_INT32_TYPE, &anInt32, sizeof(int32_t));
}

status_t VMessage::ReplaceInt32(const char *name, int32_t index, int32_t anInt32) {
	return ReplaceData(name, V_INT32_TYPE, index, &anInt32, sizeof(int32_t));
}

status_t VMessage::ReplaceInt64(const char *name, int64_t anInt64) {
	return ReplaceData(name, V_INT64_TYPE, &anInt64, sizeof(int64_t));
}

status_t VMessage::ReplaceInt64(const char *name, int32_t index, int64_t anInt64) {
	return ReplaceData(name, V_INT64_TYPE, index, &anInt64, sizeof(int64_t));
}

status_t VMessage::ReplaceFloat(const char *name, float aFloat) {
	return ReplaceData(name, V_FLOAT_TYPE, &aFloat, sizeof(float));
}

status_t VMessage::ReplaceFloat(const char *name, int32_t index, float aFloat) {
	return ReplaceData(name, V_FLOAT_TYPE, index, &aFloat, sizeof(float));
}

status_t VMessage::ReplaceDouble(const char *name, double aDouble) {
	return ReplaceData(name, V_DOUBLE_TYPE, &aDouble, sizeof(double));
}

status_t VMessage::ReplaceDouble(const char *name, int32_t index, double aDouble) {
	return ReplaceData(name, V_DOUBLE_TYPE, index, &aDouble, sizeof(double));
}

status_t VMessage::ReplaceString(const char *name, const char *string) {
	return ReplaceData(name, V_STRING_TYPE, &string, strlen(string)+1);
}

status_t VMessage::ReplaceString(const char *name, int32_t index, const char *string) {
	return ReplaceData(name, V_STRING_TYPE, index, &string, strlen(string)+1);
}

status_t VMessage::ReplacePoint(const char *name, VPoint point) {
	return V_ERROR;
}

status_t VMessage::ReplacePoint(const char *name, int32_t index, VPoint point) {
	return V_ERROR;
}

status_t VMessage::ReplaceRect(const char *name, VRect rect) {
	return V_ERROR;
}

status_t VMessage::ReplaceRect(const char *name, int32_t index, VRect rect) {
	return V_ERROR;
}

// status_t ReplaceRef(const char *name, entry_ref *ref);
// status_t ReplaceRef(const char *name, int32_t index, entry_ref *ref);

status_t VMessage::ReplaceMessage(const char *name, VMessage *message) {
	return V_ERROR;
}

status_t VMessage::ReplaceMessage(const char *name, int32_t index, VMessage *message) {
	return V_ERROR;
}

status_t VMessage::ReplaceMessenger(const char *name, VMessenger *messenger) {
	return V_ERROR;
}

status_t VMessage::ReplaceMessenger(const char *name, int32_t index, VMessenger *messenger) {
	return V_ERROR;
}

status_t VMessage::ReplacePointer(const char *name, const void *pointer) {
	return ReplaceData(name, V_POINTER_TYPE, &pointer, sizeof(void *));
}

status_t VMessage::ReplacePointer(const char *name, int32_t index, const void *pointer) {
	return ReplaceData(name, V_POINTER_TYPE, index, &pointer, sizeof(void *));
}

status_t VMessage::ReplaceFlat(const char *name, VFlattenable *object) {
	return V_ERROR;
}

status_t VMessage::ReplaceFlat(const char *name, int32_t index, VFlattenable *object) {
	return V_ERROR;
}

VMessenger *VMessage::ReturnAddress(void) {
	return NULL;
}

status_t VMessage::SendReply(VMessage *message, VMessage *reply, bigtime_t sendTimeout, bigtime_t replyTimeout) {
	return V_ERROR;
}

status_t VMessage::SendReply(VMessage *message, VHandler *replyHandler, bigtime_t sendTimeout) {
	return V_ERROR;
}

status_t VMessage::SendReply(uint32_t command, VMessage *reply) {
	return V_ERROR;
}

status_t VMessage::SendReply(uint32_t command, VHandler *replyHandler) {
	return V_ERROR;
}


bool VMessage::WasDelivered(void) const {
	return false;
}

bool VMessage::IsSourceRemote(void) const {
	return false;
}

bool VMessage::IsSourceWaiting(void) const {
	return false;
}

bool VMessage::IsReply(void) const {
	return false;
}

const VMessage *VMessage::Previous(void) const {
	return NULL;
}

bool VMessage::WasDropped(void) const {
	return false;
}

VPoint VMessage::DropPoint(VPoint *offset) const {
	return V_ORIGIN;
}

VMessage &VMessage::operator =(const VMessage& msg){
	what = msg.what;
	return *this;
}

void *VMessage::operator new(size_t numBytes) {
	return blockCache.Get(numBytes);
}

void VMessage::operator delete(void *memory, size_t numBytes) {
	return blockCache.Save(memory, numBytes);
}
