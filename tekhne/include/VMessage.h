/***************************************************************************
 *            VMessage.h
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

#ifndef _VMESSAGE_H
#define _VMESSAGE_H

#include "VErrors.h"
#include "VString.h"
#include "VRect.h"
#include "VPoint.h"
#include "VDataIO.h"
#include "VList.h"
#include <new>

namespace tekhne {

class VMessenger;
class VHandler;
class VLooper;
class msg_thread;

extern void *looper_thread_func(void *l);

class VMessage {
private:
	VList l;
	bool _isReply;
	bool _wasDelivered;
	bool _isSourceWaiting;

	// these are used internally by looper in PostMessage and dispatch message
	VHandler *_replyHandler;
	VHandler *_handler;
	// there are 2 sematics for passing replies. 1) throu a reply handler in which
	// case you post a message to post it to the _replyHandler. 2) you fill in the
	// _replyMessage. The first is asynchronous, the second synchronous.
	VMessage *_replyMessage;

	// the messenger return address,created from the _handler and It's VLooper?
	VMessenger *_returnAddress;

	public:
	uint32_t what;

	VMessage(uint32_t command);
	VMessage(const VMessage &message);
	VMessage(void);

	virtual ~VMessage();

	status_t AddData(const char *name, type_code type,
		  const void *data,
		  ssize_t numBytes,
		  bool fixedSize = true,
		  int32_t numItems = 1);
	status_t AddBool(const char *name, bool aBool);
	status_t AddInt8(const char *name, int8_t anInt8);
	status_t AddInt16(const char *name, int16_t anInt16);
	status_t AddInt32(const char *name, int32_t anInt32);
	status_t AddInt64(const char *name, int64_t anInt64);
	status_t AddFloat(const char *name, float aFloat);
	status_t AddDouble(const char *name, double aDouble);
	status_t AddString(const char *name, const char *string);
	status_t AddString(const char *name, const VString &string);
	status_t AddPoint(const char *name, VPoint point);
	status_t AddRect(const char *name, VRect rect);
	//status_t AddRef(const char *name, const entry_ref *ref);
	status_t AddMessage(const char *name, const VMessage *message);
	status_t AddMessenger(const char *name, VMessenger *messenger);
	status_t AddPointer(const char *name, const void *pointer);

	status_t AddSpecifier(const VMessage *message);
	status_t AddSpecifier(const char *property);
	status_t AddSpecifier(const char *property, int32_t index);
	status_t AddSpecifier(const char *property, int32_t index, int32_t range);
	status_t AddSpecifier(const char *property, const char *name);

	int32_t CountNames(type_code type) const;

	status_t FindData(const char *name, type_code type, int32_t index, const void **data, ssize_t *numBytes) const;
	status_t FindData(const char *name, type_code type, const void **data, ssize_t *numBytes) const;
	status_t FindBool(const char *name, int32_t index,  bool *aBool) const;
	status_t FindBool(const char *name, bool *aBool) const;
	status_t FindInt8(const char *name, int32_t index, int8_t *anInt8) const;
	status_t FindInt8(const char *name, int8_t *anInt8) const;
	status_t FindInt16(const char *name, int32_t index, int16_t *anInt16) const;
	status_t FindInt16(const char *name, int16_t *anInt16) const;
	status_t FindInt32(const char *name, int32_t index, int32_t *anInt32) const;
	status_t FindInt32(const char *name, int32_t *anInt32) const;
	status_t FindInt64(const char *name, int32_t index, int64_t *anInt64) const;
	status_t FindInt64(const char *name, int64_t *anInt64) const;
	status_t FindFloat(const char *name, int32_t index, float *aFloat) const;
	status_t FindFloat(const char *name, float *aFloat) const;
	status_t FindDouble(const char *name, int32_t index, double *aDouble) const;
	status_t FindDouble(const char *name, double *aDouble) const;
	status_t FindString(const char *name, int32_t index, const char **string) const;
	status_t FindString(const char *name, const char **string) const;
	status_t FindString(const char *name, VString *string) const;
	status_t FindString(const char *name, int32_t index, VString *string) const;
	status_t FindPoint(const char *name, int32_t index, VPoint *point) const;
	status_t FindPoint(const char *name, VPoint *point) const;
	status_t FindRect(const char *name, int32_t index, VRect *rect) const;
	status_t FindRect(const char *name, VRect *rect) const;
	//status_t FindRef(const char *name, int32_t index, entry_ref *ref) const;
	// status_t FindRef(const char *name, entry_ref *ref) const;
	status_t FindMessage(const char *name, int32_t index, VMessage *message) const;
	status_t FindMessage(const char *name, VMessage *message) const;
	status_t FindMessenger(const char *name, int32_t index, VMessenger *messenger) const;
	status_t FindMessenger(const char *name, VMessenger *messenger) const;
	status_t FindPointer(const char *name, int32_t index, void **pointer) const;
	status_t FindPointer(const char *name, void **pointer) const;

	status_t Flatten(VDataIO *object, ssize_t *numBytes = 0) const;
	status_t Flatten(char *address, ssize_t numBytes = 0) const;
	status_t Unflatten(VDataIO *object);
	status_t Unflatten(const char *address);
	ssize_t FlattenedSize(void) const;

	status_t GetCurrentSpecifier(int32_t *index, VMessage *specifier = 0, int32_t *what = 0, const char **property = 0) const;
	status_t PopSpecifier(void);

	status_t GetInfo(const char *name, type_code *typeFound, int32_t *countFound = 0) const;
	status_t GetInfo(const char *name, type_code *typeFound, bool *fixedSize) const;
	status_t GetInfo(type_code type, int32_t index, char **nameFound, type_code *typeFound, int32_t *countFound = 0) const;

	bool HasSpecifiers(void) const;

	bool IsSystem(void) const;

	status_t MakeEmpty(void);
	bool IsEmpty(void) const;

	void PrintToStream(void) const;

	status_t RemoveName(const char *name);
	status_t RemoveData(const char *name, int32_t index = 0);

	status_t ReplaceData(const char *name, type_code type, const void *data, ssize_t numBytes);
	status_t ReplaceData(const char *name, type_code type, int32_t index, const void *data, ssize_t numBytes);
	status_t ReplaceBool(const char *name, bool aBool);
	status_t ReplaceBool(const char *name, int32_t index, bool aBool);
	status_t ReplaceInt8(const char *name, int8_t anInt8);
	status_t ReplaceInt8(const char *name, int32_t index, int8_t anInt8);
	status_t ReplaceInt16(const char *name, int16_t anInt16);
	status_t ReplaceInt16(const char *name, int32_t index, int16_t anInt16);
	status_t ReplaceInt32(const char *name, long anInt32);
	status_t ReplaceInt32(const char *name, int32_t index, int32_t anInt32);
	status_t ReplaceInt64(const char *name, int64_t anInt64);
	status_t ReplaceInt64(const char *name, int32_t index, int64_t anInt64);
	status_t ReplaceFloat(const char *name, float aFloat);
	status_t ReplaceFloat(const char *name, int32_t index, float aFloat);
	status_t ReplaceDouble(const char *name, double aDouble);
	status_t ReplaceDouble(const char *name, int32_t index, double aDouble);
	status_t ReplaceString(const char *name, const char *string);
	status_t ReplaceString(const char *name, int32_t index, const char *string);
	status_t ReplacePoint(const char *name, VPoint point);
	status_t ReplacePoint(const char *name, int32_t index, VPoint point);
	status_t ReplaceRect(const char *name, VRect rect);
	status_t ReplaceRect(const char *name, int32_t index, VRect rect);
	// status_t ReplaceRef(const char *name, entry_ref *ref);
	// status_t ReplaceRef(const char *name, int32_t index, entry_ref *ref);
	status_t ReplaceMessage(const char *name, VMessage *message);
	status_t ReplaceMessage(const char *name, int32_t index, VMessage *message);
	status_t ReplaceMessenger(const char *name, VMessenger *messenger);
	status_t ReplaceMessenger(const char *name, int32_t index, VMessenger *messenger);
	status_t ReplacePointer(const char *name, const void *pointer);
	status_t ReplacePointer(const char *name, int32_t index, const void *pointer);

	VMessenger *ReturnAddress(void);

	status_t SendReply(VMessage *message, VMessage *reply, bigtime_t sendTimeout = V_INFINITE_TIMEOUT, bigtime_t replyTimeout = V_INFINITE_TIMEOUT);
	status_t SendReply(VMessage *message, VHandler *replyHandler = 0, bigtime_t sendTimeout = V_INFINITE_TIMEOUT);
	status_t SendReply(uint32_t command, VMessage *reply);
	status_t SendReply(uint32_t command, VHandler *replyHandler = 0);

	bool WasDelivered(void) const;
	bool IsSourceRemote(void) const;
	bool IsSourceWaiting(void) const;
	bool IsReply(void) const;
	const VMessage *Previous(void) const;

	bool WasDropped(void) const;
	VPoint DropPoint(VPoint *offset = 0) const;

	VMessage &operator =(const VMessage& msg);
	void *operator new(size_t numBytes) throw ();
	void operator delete(void *memory, size_t numBytes);

	// give VLooper access to message internals...
	friend class VLooper;
	friend void *looper_thread_func(void *l);
	friend class VMessenger;
	friend class msg_thread;
	friend class VApplication;
	friend class ReplyHandler;
};

} // namespace tekhne

#endif /* _VMESSAGE_H */
