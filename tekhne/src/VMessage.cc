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

namespace tekhne {
union Value {
	bool b;
	int8_t i8;
	int16_t i16;
	int32_t i32;
	int64_t i64;
	float f;
	double d;
	const void *p;
	struct {
		float x;
		float y;
	} pt;
	struct {
		float top;
		float left;
		float bottom;
		float right;
	} r;
};

typedef struct storage_item {
	char name[V_NAME_LENGTH+1];
	type_code type;
	Value data;
	ssize_t numBytes;
	bool fixedSize;
	int32_t numItems;
} storage_item;

inline void *copyBuffer(const void *b, int32_t len) {
	void *nb = malloc(len);
	memmove(nb, b, len);
	return nb;
}

storage_item *make_storage_item(const char *name, type_code type, const void *data,
	ssize_t numBytes, bool fixedSize, int32_t numItems) {
	storage_item *si = new storage_item();

	if ((int32_t)strlen(name) > V_NAME_LENGTH) {
		memmove(si->name, name, V_NAME_LENGTH);
		si->name[V_NAME_LENGTH] = 0;
	} else {
		bzero(si->name, V_NAME_LENGTH+1);
		strcpy(si->name, name);
	}
	si->type = type;
	switch(type) {
		case V_BOOL_TYPE:
			si->data.b = *((const bool *)data);
			break;
		case V_INT8_TYPE:
			si->data.i8 = *((const int8_t *)data);
			break;
		case V_INT16_TYPE:
			si->data.i16 = *((const int16_t *)data);
			break;
		case V_INT32_TYPE:
			si->data.i32 = *((const int32_t *)data);
			break;
		case V_INT64_TYPE:
			si->data.i64 = *((const int64_t *)data);
			break;
		case V_FLOAT_TYPE:
			si->data.f = *((const float *)data);
			break;
		case V_DOUBLE_TYPE:
			si->data.d = *((const double *)data);
			break;
		case V_POINT_TYPE:
			si->data.pt.x = ((const VPoint *)data)->x;
			si->data.pt.y = ((const VPoint *)data)->y;
			break;
		case V_RECT_TYPE:
			si->data.r.top = ((const VRect *)data)->top;
			si->data.r.left = ((const VRect *)data)->left;
			si->data.r.bottom = ((const VRect *)data)->bottom;
			si->data.r.right = ((const VRect *)data)->right;
			break;
		case V_POINTER_TYPE:
		case V_MESSAGE_TYPE:
		case V_MESSENGER_TYPE:
			si->data.p = data;
			break;
		default:
			si->data.p = copyBuffer(data, numBytes);
	}
	si->numBytes = numBytes;
	si->fixedSize = fixedSize;
	si->numItems = numItems;
	return si;
}
void free_storage_item(storage_item *si) {
	switch(si->type) {
		case V_BOOL_TYPE:
		case V_INT8_TYPE:
		case V_INT16_TYPE:
		case V_INT32_TYPE:
		case V_INT64_TYPE:
		case V_FLOAT_TYPE:
		case V_DOUBLE_TYPE:
		case V_POINTER_TYPE:
		case V_POINT_TYPE:
		case V_RECT_TYPE:
		case V_MESSAGE_TYPE:
		case V_MESSENGER_TYPE:
			break;
		default:
			free(const_cast<void *>(si->data.p));
	}
	delete si;
}

static bool deleteItem(void *item) {
	free_storage_item(static_cast<storage_item *>(item));
	return false;
}

}

VMessage::VMessage(uint32_t command) : _isReply(false), _wasDelivered(false), _isSourceWaiting(true),
	_replyHandler(0), _handler(0), _replyMessage(0), _returnAddress(0), what(command) {
}

VMessage::VMessage(const VMessage &message) : _isReply(false), _wasDelivered(false), _isSourceWaiting(true),
	_replyHandler(message._replyHandler), _handler(message._handler), _replyMessage(message._replyMessage),
	_returnAddress(0), what(message.what) {
	storage_item **items = static_cast<storage_item **>(message.l.Items());
	for (int i=0;i<message.l.CountItems();i++) {
		storage_item *si = items[i];
		storage_item *nsi = new storage_item();
		memmove(nsi, si, sizeof(storage_item));
		switch(si->type) {
			case V_BOOL_TYPE:
			case V_INT8_TYPE:
			case V_INT16_TYPE:
			case V_INT32_TYPE:
			case V_INT64_TYPE:
			case V_FLOAT_TYPE:
			case V_DOUBLE_TYPE:
			case V_POINTER_TYPE:
			case V_POINT_TYPE:
			case V_RECT_TYPE:
			case V_MESSAGE_TYPE:
			case V_MESSENGER_TYPE:
				break;
			default:
				nsi->data.p = copyBuffer(si->data.p, si->numBytes);
		}
		l.AddItem(nsi);
	}
}

VMessage::VMessage(void) : _isReply(false), _wasDelivered(false), _isSourceWaiting(true), _replyHandler(0),
 _handler(0), _replyMessage(0), _returnAddress(0), what(0) {
}

VMessage::~VMessage() {
	MakeEmpty();
}

status_t VMessage::AddData(const char *name, type_code type, const void *data,
	ssize_t numBytes, bool fixedSize, int32_t numItems) {
	if(name && data && numBytes > 0 && type != V_ANY_TYPE) {
		if (numItems > 0) {
			if (l.AddItem(make_storage_item(name, type, data, numBytes, fixedSize, numItems))) {
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
	return AddData(name, V_POINT_TYPE, &point, sizeof(VPoint), true, 1);
}

status_t VMessage::AddRect(const char *name, VRect rect) {
	return AddData(name, V_RECT_TYPE, &rect, sizeof(VRect), true, 1);
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
	if (index < 0) {
		return V_BAD_INDEX;
	}
	storage_item **items = static_cast<storage_item **>(l.Items());
	for (int i=0;i<l.CountItems();i++) {
		storage_item *si = items[i];
		if ((type == V_ANY_TYPE || type == si->type) && strcmp(name, si->name) == 0) {
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
				case V_POINT_TYPE:
					(*((VPoint**)data))->Set(si->data.pt.x, si->data.pt.y);
					break;
				case V_RECT_TYPE:
					(*((VRect**)data))->Set(si->data.r.left, si->data.r.top, si->data.r.right, si->data.r.bottom);
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
	storage_item **items = static_cast<storage_item **>(l.Items());
	for (int i=0;i<l.CountItems();i++) {
		storage_item *si = items[i];
		if ((type == V_ANY_TYPE || type == si->type) && strcmp(name, si->name) == 0) {
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
				case V_POINT_TYPE:
					(*((VPoint**)data))->Set(si->data.pt.x, si->data.pt.y);
					break;
				case V_RECT_TYPE:
					(*((VRect**)data))->Set(si->data.r.left, si->data.r.top, si->data.r.right, si->data.r.bottom);
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
	char *s = 0;
	status_t ans = FindData(name, V_STRING_TYPE, (const void **)&s, &x);
	if (ans == V_OK) {
		string->SetTo(s);
	}
	return ans;
}

status_t VMessage::FindString(const char *name, int32_t index, VString *string) const {
	ssize_t x;
	char *s = 0;
	status_t ans = FindData(name, V_STRING_TYPE, index, (const void **)&s, &x);
	if (ans == V_OK) {
		string->SetTo(s);
	}
	return ans;
}

status_t VMessage::FindPoint(const char *name, int32_t index, VPoint *point) const {
	ssize_t x;
	return FindData(name, V_POINT_TYPE, index, (const void**)&point, &x);
}

status_t VMessage::FindPoint(const char *name, VPoint *point) const {
	ssize_t x;
	return FindData(name, V_POINT_TYPE, (const void**)&point, &x);
}

status_t VMessage::FindRect(const char *name, int32_t index, VRect *rect) const {
	ssize_t x;
	return FindData(name, V_RECT_TYPE, index, (const void**)&rect, &x);
}

status_t VMessage::FindRect(const char *name, VRect *rect) const {
	ssize_t x;
	return FindData(name, V_RECT_TYPE, (const void**)&rect, &x);
}


//status_t FindRef(const char *name, int32_t index, entry_ref *ref) const;
// status_t FindRef(const char *name, entry_ref *ref) const;

status_t VMessage::FindMessage(const char *name, int32_t index, VMessage *message) const {
	ssize_t x;
	return FindData(name, V_MESSAGE_TYPE, index, (const void**)message, &x);
}

status_t VMessage::FindMessage(const char *name, VMessage *message) const {
	ssize_t x;
	return FindData(name, V_MESSAGE_TYPE, (const void**)message, &x);
}

status_t VMessage::FindMessenger(const char *name, int32_t index, VMessenger *messenger) const {
	ssize_t x;
	return FindData(name, V_MESSENGER_TYPE, index, (const void**)messenger, &x);
}

status_t VMessage::FindMessenger(const char *name, VMessenger *messenger) const {
	ssize_t x;
	return FindData(name, V_MESSENGER_TYPE, (const void**)messenger, &x);
}

status_t VMessage::FindPointer(const char *name, int32_t index, void **pointer) const {
	ssize_t x;
	return FindData(name, V_POINTER_TYPE, index, (const void**)pointer, &x);
}

status_t VMessage::FindPointer(const char *name, void **pointer) const {
	ssize_t x;
	return FindData(name, V_POINTER_TYPE, (const void**)pointer, &x);
}

status_t VMessage::Flatten(VDataIO *object, ssize_t *numBytes) const {
	int32_t bc = 0;
	if (numBytes) *numBytes = 0;

	int32_t rc = object->Write(&what, sizeof(int32_t));
	if (rc != sizeof(int32_t)) return V_ERROR;
	bc += rc;

	int32_t ci = l.CountItems();
	rc = object->Write(&ci, sizeof(int32_t));
	if (rc != sizeof(int32_t)) return V_ERROR;
	bc += rc;

	storage_item **items = static_cast<storage_item **>(l.Items());
	for(int i=0;i<ci;i++) {
		storage_item *si = items[i];
		switch(si->type) {
			case V_BOOL_TYPE:
			case V_INT8_TYPE:
			case V_INT16_TYPE:
			case V_INT32_TYPE:
			case V_INT64_TYPE:
			case V_FLOAT_TYPE:
			case V_DOUBLE_TYPE:
			case V_POINT_TYPE:
			case V_RECT_TYPE:
				rc = object->Write(si, sizeof(storage_item));
				if (rc != sizeof(storage_item)) return V_ERROR;
				bc += rc;
				break;
			case V_STRING_TYPE:
			case V_POINTER_TYPE:
				rc = object->Write(si, sizeof(storage_item));
				if (rc != sizeof(storage_item)) return V_ERROR;
				bc += rc;
				rc = object->Write(si->data.p, si->numBytes);
				if (rc != si->numBytes) return V_ERROR;
				bc += rc;
				break;
			default:
				return V_ERROR;
		}
	}
	if (numBytes) *numBytes = bc;
	return V_OK;
}

status_t VMessage::Flatten(char *address, ssize_t numBytes) const {
	VMemoryIO mio(address, numBytes);
	return Flatten(&mio);
}

status_t VMessage::Unflatten(VDataIO *object) {
	int32_t rc = object->Read(&what, sizeof(int32_t));
	if (rc != sizeof(int32_t)) return V_ERROR;
	int32_t ci;
	rc = object->Read(&ci, sizeof(int32_t));
	if (rc != sizeof(int32_t)) return V_ERROR;
	for(int i=0;i<ci;i++) {
		storage_item *si = new storage_item();
		rc = object->Read(si, sizeof(storage_item));
		if (rc != sizeof(storage_item)) return V_ERROR;
		switch(si->type) {
			case V_BOOL_TYPE:
			case V_INT8_TYPE:
			case V_INT16_TYPE:
			case V_INT32_TYPE:
			case V_INT64_TYPE:
			case V_FLOAT_TYPE:
			case V_DOUBLE_TYPE:
			case V_POINT_TYPE:
			case V_RECT_TYPE:
				break;
			case V_STRING_TYPE:
			case V_POINTER_TYPE:
				si->data.p = malloc(si->numBytes);
				rc = object->Read(const_cast<void *>(si->data.p), si->numBytes);
				if (rc != si->numBytes) return V_ERROR;
				break;
			default:
				return V_ERROR;
		}
		l.AddItem(si);
	}
	return V_OK;
}

status_t VMessage::Unflatten(const char *address) {
	VMemoryIO mio(address, INT32_MAX);
	return Unflatten(&mio);
}

ssize_t VMessage::FlattenedSize(void) const {
	int32_t numBytes = 0;

	numBytes += sizeof(int32_t);
	numBytes += sizeof(int32_t);

	for(int i=0;i<l.CountItems();i++) {
		storage_item *si = static_cast<storage_item*>(l.GetItem(i));
		switch(si->type) {
			case V_BOOL_TYPE:
			case V_INT8_TYPE:
			case V_INT16_TYPE:
			case V_INT32_TYPE:
			case V_INT64_TYPE:
			case V_FLOAT_TYPE:
			case V_DOUBLE_TYPE:
			case V_POINT_TYPE:
			case V_RECT_TYPE:
				numBytes += sizeof(storage_item);
				break;
			case V_POINTER_TYPE:
				numBytes += sizeof(storage_item);
				numBytes += si->numBytes;
				break;
			default:
				return V_ERROR;
		}
	}
	return V_OK;
}

status_t VMessage::GetCurrentSpecifier(int32_t *index, VMessage *specifier, int32_t *what, const char **property) const {
	return V_ERROR;
}

status_t VMessage::PopSpecifier(void) {
	return V_ERROR;
}

status_t VMessage::GetInfo(const char *name, type_code *typeFound, int32_t *countFound) const {
	*countFound = 0;
	*typeFound = 0;
	storage_item **items = static_cast<storage_item **>(l.Items());
	for (int i=0;i<l.CountItems();i++) {
		storage_item *si = items[i];
		if (strcmp(name, si->name) == 0) {
			if (*typeFound == 0) {
				*typeFound++;
				*countFound++;
			} else if (*typeFound == si->type) {
				*countFound++;
			}
		}
	}
	return V_OK;
}

status_t VMessage::GetInfo(const char *name, type_code *typeFound, bool *fixedSize) const {
	*fixedSize = false;
	*typeFound = 0;
	storage_item **items = static_cast<storage_item **>(l.Items());
	for (int i=0;i<l.CountItems();i++) {
		storage_item *si = items[i];
		if (strcmp(name, si->name) == 0) {
			*typeFound = si->type;
			*fixedSize = si->fixedSize;
			return V_OK;
		}
	}
	return V_NAME_NOT_FOUND;
}

status_t VMessage::GetInfo(type_code type, int32_t index, char **nameFound, type_code *typeFound, int32_t *countFound) const {
	if (index < 0) {
		return V_BAD_INDEX;
	}
	storage_item **items = static_cast<storage_item **>(l.Items());
	for (int i=0;i<l.CountItems();i++) {
		storage_item *si = items[i];
		if (type == V_ANY_TYPE || type == si->type) {
			if (index == 0) {
				*nameFound = si->name;
				*typeFound = si->type;
				*countFound = 1;
				return V_OK;
			}
			index--;
		}
	}
	return V_NAME_NOT_FOUND;
}


bool VMessage::HasSpecifiers(void) const {
	return false;
}


bool VMessage::IsSystem(void) const {
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

namespace tekhne {
	typedef struct msg_print_item {
		char *name;
		type_code type;
		int32_t count;
	} msg_print_item;

	bool print_msg_item(void *i, void *c) {
		msg_print_item *mpi = static_cast<msg_print_item *>(i);
		cout << *(static_cast<int *>(c)) <<" "<< mpi->name<<", type = "<<mpi->type<<", count = "<<mpi->count << endl;
		delete mpi;
		(*(static_cast<int *>(c)))++;
		return false;
	}
}

void VMessage::PrintToStream(void) const {
	cout << "VMessage: "<<what<< endl;
	VList pi;
	storage_item **items = static_cast<storage_item **>(l.Items());
	for (int i=0; i<l.CountItems();i++) {
		storage_item *si = items[i];
		bool found = false;
		for (int j=0; j<pi.CountItems();j++) {
			msg_print_item *mpi = static_cast<msg_print_item *>(pi.GetItem(j));
			if (strcmp(mpi->name, si->name) == 0) {
				found = true;
				mpi->count++;
				break;
			}
		}
		if (!found) {
			msg_print_item *mpi = new msg_print_item();
			mpi->name = si->name;
			mpi->type = si->type;
			mpi->count = 1;
			pi.AddItem(mpi);
		}
	}
	int32_t count = 1;
	pi.DoForEach(tekhne::print_msg_item, &count);

}


status_t VMessage::RemoveName(const char *name) {
	storage_item **items = static_cast<storage_item **>(l.Items());
	for (int i=l.CountItems()-1;i>0;i--) {
		storage_item *si = items[i];
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
	storage_item **items = static_cast<storage_item **>(l.Items());
	for (int i=0; i<l.CountItems();i++) {
		storage_item *si = items[i];
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
	storage_item **items = static_cast<storage_item **>(l.Items());
	for (int i=0; i<l.CountItems();i++) {
		storage_item *si = items[i];
		if ((type == V_ANY_TYPE || type == si->type) && strcmp(name, si->name) == 0) {
			l.ReplaceItem(i, make_storage_item(name, type, data, numBytes, false, 1));
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
	storage_item **items = static_cast<storage_item **>(l.Items());
	for (int i=0; i<l.CountItems();i++) {
		storage_item *si = items[i];
		if ((type == V_ANY_TYPE || type == si->type) && strcmp(name, si->name) == 0) {
			if (index == 0) {
				if (type == V_ANY_TYPE) type = si->type;
				l.ReplaceItem(i, make_storage_item(name, type, data, numBytes, false, 1));
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
	return ReplaceData(name, V_POINT_TYPE, &point, sizeof(VPoint));
}

status_t VMessage::ReplacePoint(const char *name, int32_t index, VPoint point) {
	return ReplaceData(name, V_POINT_TYPE, index, &point, sizeof(VPoint));
}

status_t VMessage::ReplaceRect(const char *name, VRect rect) {
	return ReplaceData(name, V_RECT_TYPE, &rect, sizeof(VRect));
}

status_t VMessage::ReplaceRect(const char *name, int32_t index, VRect rect) {
	return ReplaceData(name, V_RECT_TYPE, index, &rect, sizeof(VRect));
}

// status_t ReplaceRef(const char *name, entry_ref *ref);
// status_t ReplaceRef(const char *name, int32_t index, entry_ref *ref);

status_t VMessage::ReplaceMessage(const char *name, VMessage *message) {
	return ReplaceData(name, V_MESSAGE_TYPE, &message, sizeof(void *));
}

status_t VMessage::ReplaceMessage(const char *name, int32_t index, VMessage *message) {
	return ReplaceData(name, V_MESSAGE_TYPE, index, &message, sizeof(void *));
}

status_t VMessage::ReplaceMessenger(const char *name, VMessenger *messenger) {
	return ReplaceData(name, V_MESSENGER_TYPE, &messenger, sizeof(void *));
}

status_t VMessage::ReplaceMessenger(const char *name, int32_t index, VMessenger *messenger) {
	return ReplaceData(name, V_MESSENGER_TYPE, index, &messenger, sizeof(void *));
}

status_t VMessage::ReplacePointer(const char *name, const void *pointer) {
	return ReplaceData(name, V_POINTER_TYPE, &pointer, sizeof(void *));
}

status_t VMessage::ReplacePointer(const char *name, int32_t index, const void *pointer) {
	return ReplaceData(name, V_POINTER_TYPE, index, &pointer, sizeof(void *));
}

VMessenger *VMessage::ReturnAddress(void) {
	return _returnAddress;
}

status_t VMessage::SendReply(VMessage *message, VMessage *reply, bigtime_t sendTimeout, bigtime_t replyTimeout) {
	status_t err = V_ERROR;
	_isSourceWaiting = false;
	if (_replyHandler && _replyHandler->Looper()) {
		if (_replyHandler->LockLooperWithTimeout(sendTimeout) == V_OK) {
			message->_replyMessage = reply;
			// becasue we are delivering the message ourselves we don't need the replyTimeout?
			_replyHandler->MessageReceived(message);
			_replyHandler->UnlockLooper( );
			err = V_OK;
		}
	}
	return err;
}

status_t VMessage::SendReply(VMessage *message, VHandler *replyHandler, bigtime_t sendTimeout) {
	status_t err = V_ERROR;
	_isSourceWaiting = false;
	if (_replyHandler && _replyHandler->Looper()) {
		if (_replyHandler->LockLooperWithTimeout(sendTimeout) == V_OK) {
			message->_replyHandler = replyHandler;
			err = _replyHandler->Looper()->PostMessage(message);
			_replyHandler->UnlockLooper( );
			err = V_OK;
		}
	} else if (_replyMessage) {
		*_replyMessage = *message;
		err = V_OK;
	}
	return err;
}

status_t VMessage::SendReply(uint32_t command, VMessage *reply) {
	status_t err = V_ERROR;
	_isSourceWaiting = false;
	if (_replyHandler && _replyHandler->Looper()) {
		if (_replyHandler->LockLooper( ) == V_OK) {
			VMessage *msg = new VMessage(command);
			msg->_replyMessage = reply;
			_replyHandler->MessageReceived(msg);
			delete msg;
			_replyHandler->UnlockLooper( );
			err = V_OK;
		}
	}
	return err;
}

status_t VMessage::SendReply(uint32_t command, VHandler *replyHandler) {
	status_t err = V_ERROR;
	_isSourceWaiting = false;
	if (_replyHandler && _replyHandler->Looper()) {
		if (_replyHandler->LockLooper( )) {
			VMessage *msg = new VMessage(command);
			msg->_replyHandler = replyHandler;
			err = _replyHandler->Looper()->PostMessage(msg);
			delete msg;
			_replyHandler->UnlockLooper( );
			err = V_OK;
		}
	} else if (_replyMessage) {
		_replyMessage->MakeEmpty( );
		_replyMessage->what = command;
		err = V_OK;
	}
	return err;
}


bool VMessage::WasDelivered(void) const {
	return _wasDelivered;
}

bool VMessage::IsSourceRemote(void) const {
	return false;
}

bool VMessage::IsSourceWaiting(void) const {
	return _isSourceWaiting;
}

bool VMessage::IsReply(void) const {
	return _isReply;
}

const VMessage *VMessage::Previous(void) const {
	return 0;
}

bool VMessage::WasDropped(void) const {
	return false;
}

VPoint VMessage::DropPoint(VPoint *offset) const {
	return V_ORIGIN;
}

VMessage &VMessage::operator =(const VMessage& msg){
	if (this != &msg) {
		what = msg.what;
		_handler = msg._handler;
		_replyHandler = msg._replyHandler;
		_replyMessage = msg._replyMessage;
		storage_item **items = static_cast<storage_item **>(msg.l.Items());
		for (int i=0;i<msg.l.CountItems();i++) {
			storage_item *si = items[i];
			storage_item *nsi = new storage_item();
			memmove(nsi, si, sizeof(storage_item));
			switch(si->type) {
				case V_BOOL_TYPE:
				case V_INT8_TYPE:
				case V_INT16_TYPE:
				case V_INT32_TYPE:
				case V_INT64_TYPE:
				case V_FLOAT_TYPE:
				case V_DOUBLE_TYPE:
				case V_POINTER_TYPE:
				case V_POINT_TYPE:
				case V_RECT_TYPE:
				case V_MESSAGE_TYPE:
				case V_MESSENGER_TYPE:
					break;
				default:
					nsi->data.p = copyBuffer(si->data.p, si->numBytes);
			}
			l.AddItem(nsi);
		}
	}
	return *this;
}

void *VMessage::operator new(size_t numBytes) throw () {
	new_handler h = set_new_handler(0);
	set_new_handler(h);
	while(1) {
		void *ptr = blockCache.Get(numBytes);
		if (ptr) {
			return ptr;
		}
		if (h) {
			(*h)();
		} else {
			throw std::bad_alloc();
		}
	}
}

void VMessage::operator delete(void *memory, size_t numBytes) {
	if (memory) {
		return blockCache.Save(memory, numBytes);
	}
}
