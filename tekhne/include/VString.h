/***************************************************************************
 *            VString.h
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

#ifndef VSTRING_H_
#define VSTRING_H_

#include "VHashable.h"

#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

namespace tekhne {

class VString : public VHashable {
private:
	char *buf;
	int32_t bufferLen;
	bool bufferLocked;
	inline void makeBuffer(int32_t count, bool deleteBuffer=true) {
		bufferLen = count;
		if (deleteBuffer) delete [] buf;
		buf = new char[count];
		bzero(buf, count);
	}

public:
	VString(void);
	VString(const char *string);
	VString(const char *string, int32_t maxLength);
	VString(const VString &string);

	~VString();

	inline VString &Append(const VString &source) {
		return Append(source.buf);
	}
	VString &Append(const VString &source, int32_t charCount);
	inline VString &Append(const char *source) {
		assert(!bufferLocked);
		if (source != 0) {
			int32_t oldLen = bufferLen -1;
			int32_t sourceLen = strlen(source);
			int32_t newLen = oldLen + sourceLen + 1;
			char* oldBuf = buf;
			makeBuffer(newLen, false);
			memmove(buf, oldBuf, oldLen);
			memmove(buf+oldLen, source, sourceLen);
			delete [] oldBuf;
		}
		return *this;
	}
	VString &Append(const char *source, int32_t charCount);
	VString &Append(char c, int32_t charCount);

	VString &Prepend(const VString &source);
	VString &Prepend(const VString &source, int32_t charCount);
	VString &Prepend(const char *source);
	VString &Prepend(const char *source, int32_t charCount);
	VString &Prepend(char c, int32_t charCount);

	VString &Insert(const VString &source, int32_t insertAt);
	VString &Insert(const VString &source, int32_t charCount, int32_t insertAt);
	VString &Insert(const VString &source, int32_t sourceOffset, int32_t charCount, int32_t insertAt);
	VString &Insert(const char *source, int32_t insertAt);
	VString &Insert(const char *source, int32_t charCount, int32_t insertAt);
	VString &Insert(const char *source, int32_t sourceOffset, int32_t charCount, int32_t insertAt);
	VString &Insert(char c, int32_t charCount, int32_t insertAt);

	VString &CharacterEscape(const char *original, const char *setOfCharsToEscape, char escapeWithChar);
	VString &CharacterEscape(const char *setOfCharsToEscape, char escapeWithChar);
	VString &CharacterDeescape(const char *original, char escapeWithChar);
	VString &CharacterDeescape(char escapeWithChar);

	int Compare(const VString &string) const;
	int Compare(const VString &string, int32_t range) const;
	int Compare(const char *string) const;
	int Compare(const char *string, int32_t range) const;
	static int Compare(const VString &astring, const VString &VString);
	static int Compare(const VString *astring, const VString *VString);
	int ICompare(const VString &string) const;
	int ICompare(const VString &string, int32_t range) const;
	int ICompare(const char *string) const;
	int ICompare(const char *string, int32_t range) const;
	static int ICompare(const VString &astring, const VString &VString);
	static int ICompare(const VString *astring, const VString *VString);

	VString &CopyInto(VString &destination, int32_t sourceOffset, int32_t charCount) const;
	void CopyInto(char *destination, int32_t sourceOffset, int32_t charCount) const;
	VString &MoveInto(VString &destination, int32_t sourceOffset, int32_t charCount);
	void MoveInto(char *destination, int32_t sourceOffset, int32_t charCount);

	int32_t CountChars(void) const;
	inline int32_t Length(void) const {
		return bufferLen-1;
	}

	int32_t FindFirst(const VString &string) const;
	int32_t FindFirst(const VString &string, int32_t offset) const;
	int32_t FindFirst(const char *string) const;
	int32_t FindFirst(const char *string, int32_t offset) const;
	int32_t FindFirst(char c) const;
	int32_t FindFirst(char c, int32_t offset) const;
	int32_t FindLast(const VString &string) const;
	int32_t FindLast(const VString &string, int32_t offset) const;
	int32_t FindLast(const char *string) const;
	int32_t FindLast(const char *string, int32_t offset) const;
	int32_t FindLast(char c) const;
	int32_t FindLast(char c, int32_t offset) const;
	int32_t IFindFirst(const VString &string) const;
	int32_t IFindFirst(const VString &string, int32_t offset) const;
	int32_t IFindFirst(const char *string) const;
	int32_t IFindFirst(const char *string, int32_t offset) const;
	int32_t IFindLast(const VString &string) const;
	int32_t IFindLast(const VString &string, int32_t offset) const;
	int32_t IFindLast(const char *string) const;
	int32_t IFindLast(const char *string, int32_t offset) const;

	char *LockBuffer(int32_t maxLength);
	VString &UnlockBuffer(int32_t length = -1);

	VString &Remove(int32_t startingAt, int32_t charCount);
	VString &RemoveFirst(const VString &string);
	VString &RemoveFirst(const char *string);
	VString &RemoveLast(const VString &string);
	VString &RemoveLast(const char *string);
	VString &RemoveAll(const VString &string);
	VString &RemoveAll(const char *string);
	VString &RemoveSet(const char *charSet);

	VString &Replace(const char *old, const char *_new, int32_t count, int32_t offset = 0);
	VString &Replace(char old, char _new, int32_t count, int32_t offset = 0);
	VString &ReplaceFirst(const char *old, const char *_new);
	VString &ReplaceFirst(char old, char _new);
	VString &ReplaceLast(const char *old, const char *_new);
	VString &ReplaceLast(char old, char _new);
	VString &ReplaceAll(const char *old, const char *_new, int32_t offset = 0);
	VString &ReplaceAll(char old, char _new, int32_t offset = 0);
	VString &ReplaceSet(const char *oldSet, const char *_new);
	VString &ReplaceSet(const char *oldSet, char _new);
	VString &IReplace(const char *old, const char *_new, int32_t count, int32_t offset = 0);
	VString &IReplace(char old, char _new, int32_t count, int32_t offset = 0);
	VString &IReplaceFirst(const char *old, const char *_new);
	VString &IReplaceFirst(char old, char _new);
	VString &IReplaceLast(const char *old, const char *_new);
	VString &IReplaceLast(char old, char _new);
	VString &IReplaceAll(const char *old, const char *_new, int32_t offset = 0);
	VString &IReplaceAll(char old, char _new, int32_t offset = 0);

	inline VString &SetTo(const char *source) {
		assert(!bufferLocked);
		if (source != 0) {
			int32_t len = strlen(source);
			if (len > 0) {
				makeBuffer(len+1);
				memmove(buf, source, len);
				return *this;
			}
		}
		makeBuffer(1);
		return *this;
	}
	VString &SetTo(const char *source, int32_t charCount);
	VString &SetTo(const VString &source);
	VString &SetTo(const VString &source, int32_t charCount);
	VString &SetTo(char c, int32_t charCount);
	VString &Adopt(VString &source);
	VString &Adopt(VString &source, int32_t charCount);

	inline const char *String() const {
		return buf;
	}
	inline char ByteAt(int32_t index) const {
		if (index >=0 && index < bufferLen) {
			return buf[index];
		}
		return 0;
	}

	VString &ToLower(void);
	VString &ToUpper(void);
	VString &Capitalize(void);
	VString &CapitalizeEachWord(void);

	VString &Truncate(int32_t charCount, bool lazy = true);

	VString& operator=(const VString &string);
	VString& operator=(const char *string);
	VString& operator=(const char character);

	inline VString& operator+=(const VString &string) {
		assert(!bufferLocked);
		Append(string);
		return *this;
	}
	VString& operator+=(const char *string);
	VString& operator+=(const char character);

	VString &operator<<(const char *string);
	VString &operator<<(const VString &string);
	VString &operator<<(char c);
	VString &operator<<(uint32_t val);
	VString &operator<<(int32_t val);
	VString &operator<<(uint64_t val);
	VString &operator<<(int64_t val);
	VString &operator<<(float val);

	char operator[](int32_t index) const;
	inline char &operator[](int32_t index) {
		char *cp = new char[1];
		cp[0] = ByteAt(index);
		return *cp;
	}

	inline bool operator==(const VString &string) const {
		return Length() == string.Length() && memcmp(buf, string.buf, Length()) == 0;
	}
	bool operator==(const char *string) const;
	inline bool operator!=(const VString &string) const {
		return Length() != string.Length() || memcmp(buf, string.buf, Length()) != 0;
	}
	inline bool operator!=(const char *string) const {
		if (string != 0) {
			int32_t length = strlen(string);
			return Length() != length || memcmp(buf, string, length) != 0;
		}
		return false;
	}
	inline bool operator<(const VString &string) const {
		return strcmp(buf, string.buf) < 0;
	}
	bool operator<(const char *string) const;
	inline bool operator<=(const VString &string) const {
		return strcmp(buf, string.buf) <= 0;
	}
	bool operator<=(const char *string) const;
	inline bool operator>(const VString &string) const {
		return strcmp(buf, string.buf) > 0;
	}
	bool operator>(const char *string) const;
	inline bool operator>=(const VString &string) const {
		return strcmp(buf, string.buf) >= 0;
	}
	bool operator>=(const char *string) const;

	inline VString& Substring(volatile int32_t start, volatile int32_t end) const {
		if (start > end) { int32_t tmp=start;start=end;end=tmp; }
		if (start < 0) start = 0;
		if (end < 0) end = 0;
		if (end > bufferLen) end = bufferLen;
		if (start > bufferLen) start = end = 0;
		VString *s = new VString();
		s->makeBuffer(end-start+1);
		memmove(s->buf, buf+start, end-start);
		return *s;
	}

	inline VString& Substring(int32_t start) const {
		return Substring(start, bufferLen);
	}

	inline VString &Clear() {
		assert(!bufferLocked);
		makeBuffer(1);
		return *this;
	}

	// friends
	friend bool operator==(const char *string1, const VString &string2);
	friend bool operator!=(const char *string1, const VString &string2);
	friend bool operator<(const char *string1, const VString &string2);
	friend bool operator<=(const char *string1, const VString &string2);
	friend bool operator>(const char *string1, const VString &string2);
	friend bool operator>=(const char *string1, const VString &string2);
	// hashable
	virtual uint32_t hash(void);
};

inline bool operator==(const char *string1, const VString &string2) {
	if (string1 != 0) {
		return strcmp(string1, string2.buf) == 0;
	}
	return false;
}

inline bool operator!=(const char *string1, const VString &string2) {
	if (string1 != 0) {
		return strcmp(string1, string2.buf) != 0;
	}
	return false;
}

inline bool operator<(const char *string1, const VString &string2) {
	if (string1 != 0) {
		return strcmp(string1, string2.buf) < 0;
	}
	return false;
}
inline bool operator<=(const char *string1, const VString &string2) {
	if (string1 != 0) {
		return strcmp(string1, string2.buf) <= 0;
	}
	return false;
}
inline bool operator>(const char *string1, const VString &string2) {
	if (string1 != 0) {
		return strcmp(string1, string2.buf) > 0;
	}
	return false;
}
inline bool operator>=(const char *string1, const VString &string2) {
	if (string1 != 0) {
		return strcmp(string1, string2.buf) >= 0;
	}
	return false;
}

} // namespace tekhne

#endif /*VSTRING_H_*/
