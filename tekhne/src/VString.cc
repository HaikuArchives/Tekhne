/***************************************************************************
 *            VString.cc
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
#include <ctype.h>


using namespace tekhne;

#include <iostream>
using namespace std;

static inline int32_t convertCompareResult(int32_t result) {
	if (result < 0) return -1;
	else if (result > 0) return 1;
	return 0;
}

VString::VString(void) : buf(0), bufferLocked(false) {
	makeBuffer(1);
}

VString::VString(const char *string) : buf(0), bufferLocked(false) {
	if (string) {
		int32_t len = strlen(string)+1;
		makeBuffer(len);
		memmove(buf, string, len);
	} else {
		makeBuffer(1);
	}
}

VString::VString(const char *string, int32_t maxLength) : buf(0), bufferLocked(false) {
	if (string && maxLength > 0) {
		int32_t len = strlen(string);
		if (maxLength < len) {
			len = maxLength;
		}
		makeBuffer(len+1);
		memmove(buf, string, len);
	} else {
		makeBuffer(1);
	}
}

VString::VString(const VString &string) : buf(0), bufferLocked(false) {
	makeBuffer(string.bufferLen);
	memmove(buf, string.buf, bufferLen);
}

VString::~VString() {
	assert(!bufferLocked);
	delete [] buf;
}

VString &VString::Append(const VString &source, int32_t charCount) {
	return Append(source.buf, charCount);
}

VString &VString::Append(const char *source, int32_t charCount) {
	assert(!bufferLocked);
	if (source && charCount > 0) {
		int32_t sourceLen = strlen(source);
		if (sourceLen < charCount) {
			return Append(source);
		}
		int32_t oldLen = bufferLen-1;
		int32_t newLen = bufferLen + charCount;
		char* oldBuf = buf;
		makeBuffer(newLen, false);
		memmove(buf, oldBuf, oldLen);
		memmove(buf+oldLen, source, charCount);
		delete [] oldBuf;
	}
	return *this;
}
VString &VString::Append(char c, int32_t charCount) {
	assert(!bufferLocked);
	if (charCount > 0) {
		int32_t oldLen = bufferLen-1;
		int32_t newLen = oldLen + charCount + 1;
		char* oldBuf = buf;
		makeBuffer(newLen, false);
		memmove(buf, oldBuf, oldLen);
		while(charCount > 0) {
			charCount--;
			buf[oldLen+charCount] = c;
		}
		delete [] oldBuf;
	}
	return *this;
}

VString &VString::Prepend(const VString &source) {
	return Prepend(source.buf);
}

VString &VString::Prepend(const VString &source, int32_t charCount) {
	return Prepend(source.buf, charCount);
}

VString &VString::Prepend(const char *source) {
	assert(!bufferLocked);
	if (source) {
		int32_t oldLen = bufferLen -1;
		int32_t sourceLen = strlen(source);
		int32_t newLen = oldLen + sourceLen + 1;
		char* oldBuf = buf;
		makeBuffer(newLen, false);
		memmove(buf+sourceLen, oldBuf, oldLen);
		memmove(buf, source, sourceLen);
		delete [] oldBuf;
	}
	return *this;
}

VString &VString::Prepend(const char *source, int32_t charCount) {
	assert(!bufferLocked);
	if (source && charCount > 0) {
		int32_t sourceLen = strlen(source);
		if (sourceLen < charCount) {
			return Prepend(source);
		}
		int32_t oldLen = bufferLen-1;
		int32_t newLen = bufferLen + charCount;
		char* oldBuf = buf;
		makeBuffer(newLen, false);
		memmove(buf+charCount, oldBuf, oldLen);
		memmove(buf, source, charCount);
		delete [] oldBuf;
	}
	return *this;
}

VString &VString::Prepend(char c, int32_t charCount) {
	assert(!bufferLocked);
	if (charCount > 0) {
		int32_t oldLen = bufferLen-1;
		int32_t newLen = oldLen + charCount + 1;
		char* oldBuf = buf;
		makeBuffer(newLen, false);
		memmove(buf+charCount, oldBuf, oldLen);
		while(charCount > 0) {
			charCount--;
			buf[charCount] = c;
		}
		delete [] oldBuf;
	}
	return *this;
}

VString &VString::Insert(const VString &source, int32_t insertAt) {
	return Insert(source.buf, insertAt);
}
VString &VString::Insert(const VString &source, int32_t charCount, int32_t insertAt) {
	return Insert(source.buf, charCount, insertAt);
}

VString &VString::Insert(const VString &source, int32_t sourceOffset, int32_t charCount, int32_t insertAt) {
	return Insert(source.buf, sourceOffset, charCount, insertAt);
}

VString &VString::Insert(const char *source, int32_t insertAt) {
	assert(!bufferLocked);
	if (source && insertAt >= 0 && insertAt <= Length()) {
		if (insertAt == 0) {
			return Prepend(source);
		} else if (insertAt == Length()) {
			return Append(source);
		}
		int32_t oldLen = bufferLen -1;
		int32_t sourceLen = strlen(source);
		int32_t newLen = oldLen + sourceLen + 1;
		char* oldBuf = buf;
		makeBuffer(newLen, false);

		memmove(buf, oldBuf, insertAt);
		memmove(buf+sourceLen+insertAt, oldBuf+insertAt, oldLen-insertAt);
		memmove(buf+insertAt, source, sourceLen);

		delete [] oldBuf;
	}
	return *this;
}

VString &VString::Insert(const char *source, int32_t charCount, int32_t insertAt) {
	assert(!bufferLocked);
	if (source && insertAt >= 0 && insertAt <= Length()) {
		if (insertAt == 0) {
			return Prepend(source, charCount);
		} else if (insertAt == Length()) {
			return Append(source, charCount);
		}
		int32_t oldLen = bufferLen -1;
		int32_t sourceLen = strlen(source);
		if (sourceLen > charCount) {
			sourceLen = charCount;
		}
		int32_t newLen = oldLen + sourceLen + 1;
		char* oldBuf = buf;
		makeBuffer(newLen, false);

		memmove(buf, oldBuf, insertAt);
		memmove(buf+sourceLen+insertAt, oldBuf+insertAt, oldLen-insertAt);
		memmove(buf+insertAt, source, sourceLen);

		delete [] oldBuf;
	}
	return *this;
}

VString &VString::Insert(const char *source, int32_t sourceOffset, int32_t charCount, int32_t insertAt) {
	assert(!bufferLocked);
	if (source && insertAt >= 0 && insertAt <= Length() && charCount > 0) {
		if (sourceOffset < (int32_t)strlen(source)) {
			VString chunk(source+sourceOffset, charCount);
			if (insertAt == 0) {
				return Prepend(chunk);
			} else if (insertAt == Length()) {
				return Append(chunk);
			}
			return Insert(chunk.buf, insertAt);
		}
	}
	return *this;
}

VString &VString::Insert(char c, int32_t charCount, int32_t insertAt) {
	assert(!bufferLocked);
	if (insertAt >= 0 && insertAt <= Length() && charCount > 0) {
		if (insertAt == 0) {
			return Prepend(c, charCount);
		} else if (insertAt == Length()) {
			return Append(c, charCount);
		}
		int32_t oldLen = bufferLen - 1;
		int32_t newLen = oldLen + charCount + 1;
		char* oldBuf = buf;
		makeBuffer(newLen, false);

		memmove(buf, oldBuf, insertAt);
		memmove(buf+charCount+insertAt, oldBuf+insertAt, oldLen-insertAt);
		for (int i=0;i<charCount;i++) {
			buf[insertAt+i] = c;
		}
		delete [] oldBuf;
	}
	return *this;
}

VString &VString::CharacterEscape(const char *original, const char *setOfCharsToEscape, char escapeWithChar) {
	assert(!bufferLocked);
	if (original && setOfCharsToEscape && escapeWithChar != 0) {
		int32_t len = strlen(original);
		int32_t lec = strlen(setOfCharsToEscape);
		if (len > 0) {
			char *s = new char[len*2];
			bzero(s, len*2);
			for(int32_t i=0, j=0;i<len;i++) {
				char c = original[i];
				for(int32_t k=0;k<lec;k++) {
					if (c == setOfCharsToEscape[k]) {
						s[j++] = escapeWithChar;
					}
				}
				s[j++] = c;
			}
			SetTo(s);
			delete [] s;
			return *this;
		}
	}
	SetTo(original);
	return *this;
}

VString &VString::CharacterEscape(const char *setOfCharsToEscape, char escapeWithChar) {
	assert(!bufferLocked);
	if (setOfCharsToEscape && escapeWithChar != 0) {
		int32_t lec = strlen(setOfCharsToEscape);
		if (bufferLen > 0) {
			char *s = new char[bufferLen*2];
			bzero(s, bufferLen*2);
			for(int32_t i=0, j=0;i<bufferLen;i++) {
				char c = buf[i];
				for(int32_t k=0;k<lec;k++) {
					if (c == setOfCharsToEscape[k]) {
						s[j++] = escapeWithChar;
					}
				}
				s[j++] = c;
			}
			SetTo(s);
			delete [] s;
		}
	}
	return *this;
}

VString &VString::CharacterDeescape(const char *original, char escapeWithChar) {
	assert(!bufferLocked);
	if (original) {
		int32_t len = strlen(original);
		if (len > 0) {
			char *s = new char[len+1];
			bzero(s, len+1);
			for(int32_t i=0, j=0;i<len;i++) {
				if (escapeWithChar != original[i]) {
					s[j++] = original[i];
				}
			}
			SetTo(s);
			delete [] s;
			return *this;
		}
	}
	SetTo(original);
	return *this;
}

VString &VString::CharacterDeescape(char escapeWithChar) {
	assert(!bufferLocked);
	if (bufferLen > 0) {
		char *s = new char[bufferLen];
		bzero(s, bufferLen);
		for(int32_t i=0, j=0;i<bufferLen;i++) {
			if (escapeWithChar != buf[i]) {
				s[j++] = buf[i];
			}
		}
		SetTo(s);
		delete [] s;
		return *this;
	}
	return *this;
}

int VString::Compare(const VString &string) const {
	return convertCompareResult(strcmp(buf, string.buf));
}

int VString::Compare(const VString &string, int32_t range) const {
	return convertCompareResult(strncmp(buf, string.buf, range));
}

int VString::Compare(const char *string) const {
	return convertCompareResult(strcmp(buf, string));
}

int VString::Compare(const char *string, int32_t range) const {
	return convertCompareResult(strncmp(buf, string, range));
}

int VString::Compare(const VString &astring, const VString &bstring) { // global
	return convertCompareResult(strcmp(astring.buf, bstring.buf));
}

int VString::Compare(const VString *astring, const VString *bstring) { // global
	return convertCompareResult(strcmp(astring->buf, bstring->buf));
}

int VString::ICompare(const VString &string) const {
	return convertCompareResult(strcasecmp(buf, string.buf));
}

int VString::ICompare(const VString &string, int32_t range) const {
	return convertCompareResult(strncasecmp(buf, string.buf, range));
}

int VString::ICompare(const char *string) const  {
	return convertCompareResult(strcasecmp(buf, string));
}

int VString::ICompare(const char *string, int32_t range) const  {
	return convertCompareResult(strncasecmp(buf, string, range));
}

int VString::ICompare(const VString &astring, const VString &bstring) { // global
	return convertCompareResult(strcasecmp(astring.buf, bstring.buf));
}

int VString::ICompare(const VString *astring, const VString *bstring) { // global
	return convertCompareResult(strcasecmp(astring->buf, bstring->buf));
}

VString &VString::CopyInto(VString &destination, int32_t sourceOffset, int32_t charCount) const {
	assert(!bufferLocked);
	destination = Substring(sourceOffset, sourceOffset+charCount);
	return destination;
}

void VString::CopyInto(char *destination, int32_t sourceOffset, int32_t charCount) const {
	assert(!bufferLocked);
	VString s = Substring(sourceOffset, sourceOffset+charCount);
	memmove(destination, s.buf, s.bufferLen);
}

VString &VString::MoveInto(VString &destination, int32_t sourceOffset, int32_t charCount) {
	CopyInto(destination, sourceOffset, charCount);
	Clear();
	return destination;
}

void VString::MoveInto(char *destination, int32_t sourceOffset, int32_t charCount) {
	CopyInto(destination, sourceOffset, charCount);
	Clear();
}

int32_t VString::CountChars(void) const {
	return Length();
}

inline int32_t convertFindResult(char *ans, char *buf) {
	if (ans) return (int32_t)(ans-buf);
	return V_ERROR;
}

int32_t VString::FindFirst(const VString &string) const {
	return convertFindResult(strstr(buf, string.buf), buf);
}

int32_t VString::FindFirst(const VString &string, int32_t offset) const {
	if (offset < bufferLen && offset >= 0) {
		return convertFindResult(strstr(buf+offset, string.buf), buf);
	}
	return V_ERROR;
}

int32_t VString::FindFirst(const char *string) const {
	return convertFindResult(strstr(buf, string), buf);
}

int32_t VString::FindFirst(const char *string, int32_t offset) const {
	if (offset < bufferLen && offset >= 0) {
		return convertFindResult(strstr(buf+offset, string), buf);
	}
	return V_ERROR;
}

int32_t VString::FindFirst(char c) const {
	return convertFindResult(strchr(buf, c), buf);
}

int32_t VString::FindFirst(char c, int32_t offset) const {
	if (offset < bufferLen && offset >= 0) {
		return convertFindResult(strchr(buf+offset, c), buf);
	}
	return V_ERROR;
}

int32_t VString::FindLast(const VString &string) const {
	return FindLast(string, 0);
}

int32_t VString::FindLast(const VString &string, int32_t offset) const {
	if (string.Length() > 0 && offset < bufferLen && offset >= 0 ) {
		int32_t len = bufferLen-string.Length()-1;
		while (len >= offset) {
			if (buf[len] == string.buf[0]) {
				if (memcmp(buf+len, string.buf, string.bufferLen-1) == 0) {
					return len;
				}
			}
			len--;
		}
	}
	return V_ERROR;
}

int32_t VString::FindLast(const char *string) const {
	return FindLast(string, 0);
}

int32_t VString::FindLast(const char *string, int32_t offset) const {
	int32_t length = strlen(string);
	if (length > 0 && offset < bufferLen && offset >= 0 ) {
		int32_t len = bufferLen-length;
		while (len >= offset) {
			if (buf[len] == string[0]) {
				if (strncmp(buf+len, string, length) == 0) {
					return len;
				}
			}
			len--;
		}
	}
	return V_ERROR;
}

int32_t VString::FindLast(char c) const {
	return convertFindResult(strrchr(buf, c), buf);
}

int32_t VString::FindLast(char c, int32_t offset) const {
	if (offset < bufferLen && offset >= 0) {
		return convertFindResult(strrchr(buf+offset, c), buf);
	}
	return V_ERROR;
}

int32_t VString::IFindFirst(const VString &string) const {
	return IFindFirst(string, 0);
}

int32_t VString::IFindFirst(const VString &string, int32_t offset) const {
	return IFindFirst(string.buf, offset);
}

int32_t VString::IFindFirst(const char *string) const {
	return IFindFirst(string, 0);
}

int32_t VString::IFindFirst(const char *string, int32_t offset) const {
	if (offset < bufferLen && offset >= 0) {
		return convertFindResult(strcasestr(buf+offset, string), buf);
	}
	return V_ERROR;
}

int32_t VString::IFindLast(const VString &string) const {
	return IFindLast(string.buf);
}

int32_t VString::IFindLast(const VString &string, int32_t offset) const {
	return IFindLast(string.buf, offset);
}

int32_t VString::IFindLast(const char *string) const {
	return IFindLast(string, 0);
}

int32_t VString::IFindLast(const char *string, int32_t offset) const {
	int32_t length = strlen(string);
	if (length > 0 && offset < bufferLen && offset >= 0 ) {
		int32_t len = bufferLen-length;
		while (len >= offset) {
			if (strncasecmp(buf+len, string, length) == 0) {
				return len;
			}
			len--;
		}
	}
	return V_ERROR;
}

char *VString::LockBuffer(int32_t maxLength) {
	bufferLocked = true;
	if (maxLength > bufferLen) {
		int32_t oldLen = bufferLen;
		char *oldBuf = buf;
		makeBuffer(maxLength+1, false);
		memmove(buf, oldBuf, oldLen);
		delete [] oldBuf;
	}
	return buf;
}

VString &VString::UnlockBuffer(int32_t length) {
	if (length == -1) length = strlen(buf);
	if (length > 0) {
		int32_t oldLen = bufferLen;
		char *oldBuf = buf;
		makeBuffer(length+1, false);
		memmove(buf, oldBuf, oldLen);
		delete [] oldBuf;
	} else {
		makeBuffer(1);
	}
	bufferLocked = false;
	return *this;
}

VString &VString::Remove(int32_t startingAt, int32_t charCount) {
	if (startingAt < bufferLen && startingAt >= 0) {
		if (startingAt+charCount >= bufferLen) {
			charCount = bufferLen - startingAt;
		}
		char *oldBuf = buf;
		int32_t oldLen = bufferLen;
		makeBuffer(bufferLen - charCount, false);
		memmove(buf, oldBuf, startingAt);
		memmove(buf+startingAt, oldBuf+startingAt+charCount, oldLen-startingAt-charCount-1);
		delete [] oldBuf;
	}
	return *this;
}

VString &VString::RemoveFirst(const VString &string) {
	int32_t where = FindFirst(string);
	if (where >= 0) {
		Remove(where, string.Length());
	}
	return *this;
}

VString &VString::RemoveFirst(const char *string) {
	if (string) {
		int32_t length = strlen(string);
		if (length > 0) {
			int32_t where = FindFirst(string);
			if (where >= 0) {
				Remove(where, length);
			}
		}
	}
	return *this;
}

VString &VString::RemoveLast(const VString &string) {
	int32_t where = FindLast(string);
	if (where >= 0) {
		Remove(where, string.Length());
	}
	return *this;
}

VString &VString::RemoveLast(const char *string) {
	if (string) {
		int32_t length = strlen(string);
		if (length > 0) {
			int32_t where = FindLast(string);
			if (where >= 0) {
				Remove(where, length);
			}
		}
	}
	return *this;
}

VString &VString::RemoveAll(const VString &string) {
	int32_t where = FindFirst(string);
	while (where >= 0) {
		Remove(where, string.Length());
		where = FindFirst(string);
	}
	return *this;
}

VString &VString::RemoveAll(const char *string) {
	if (string) {
		int32_t length = strlen(string);
		if (length > 0) {
			int32_t where = FindFirst(string);
			while (where >= 0) {
				Remove(where, length);
				where = FindFirst(string);
			}
		}
	}
	return *this;
}

VString &VString::RemoveSet(const char *charSet) {
	if (charSet) {
		int32_t length = strlen(charSet);
		if (length > 0) {
			for(int i=0;i<length;i++) {
				int32_t where = FindFirst(charSet[i]);
				while (where >= 0) {
					Remove(where, 1);
					where = FindFirst(charSet[i]);
				}
			}
		}
	}
	return *this;
}

VString &VString::Replace(const char *old, const char *_new, int32_t count, int32_t offset) {
	if (old && _new) {
		int32_t length = strlen(old);
		int32_t new_length = strlen(_new);
		if (length > 0) {
			for (int i=0;i<count;i++) {
				int32_t loc = FindFirst(old, offset);
				if (loc >= 0) {
					Remove(loc, length);
					if (new_length > 0) {
						Insert(_new, loc);
					}
					offset = loc+new_length;
				}
			}
		}
	}
	return *this;
}

VString &VString::Replace(char old, char _new, int32_t count, int32_t offset) {
	while (count > 0 && offset < bufferLen) {
		if (buf[offset] == old) {
			count--;
			buf[offset] = _new;
		}
		offset++;
	}
	return *this;
}

VString &VString::ReplaceFirst(const char *old, const char *_new) {
	if (old && _new) {
		int32_t length = strlen(old);
		int32_t new_length = strlen(_new);
		int32_t loc = FindFirst(old);
		if (loc >= 0) {
			Remove(loc, length);
			if (new_length > 0) {
				Insert(_new, loc);
			}
		}
	}
	return *this;
}

VString &VString::ReplaceFirst(char old, char _new) {
	int32_t offset = convertFindResult(strchr(buf, old), buf);
	if (offset >= 0) {
		buf[offset] = _new;
	}
	return *this;
}

VString &VString::ReplaceLast(const char *old, const char *_new) {
	if (old && _new) {
		int32_t length = strlen(old);
		int32_t new_length = strlen(_new);
		int32_t loc = FindLast(old);
		if (loc >= 0) {
			Remove(loc, length);
			if (new_length > 0) {
				Insert(_new, loc);
			}
		}
	}
	return *this;
}

VString &VString::ReplaceLast(char old, char _new) {
	int32_t offset = convertFindResult(strrchr(buf, old), buf);
	if (offset >= 0) {
		buf[offset] = _new;
	}
	return *this;
}

VString &VString::ReplaceAll(const char *old, const char *_new, int32_t offset) {
	if (old && _new) {
		int32_t length = strlen(old);
		int32_t new_length = strlen(_new);
		if (length > 0) {
			int32_t loc = FindFirst(old, offset);
			while (loc >= 0) {
				Remove(loc, length);
				if (new_length > 0) {
					Insert(_new, loc);
				}
				offset = loc+new_length;
				loc = FindFirst(old, offset);
			}
		}
	}
	return *this;
}

VString &VString::ReplaceAll(char old, char _new, int32_t offset) {
	while (offset < bufferLen) {
		if (buf[offset] == old) {
			buf[offset] = _new;
		}
		offset++;
	}
	return *this;
}

VString &VString::ReplaceSet(const char *oldSet, const char *_new) {
	if (oldSet) {
		int32_t length = strlen(oldSet);
		char s[2];
		s[1] = 0;
		for (int i=0;i<length;i++) {
			s[0] = oldSet[i];
			ReplaceAll(s, _new);
		}
	}
	return *this;
}

VString &VString::ReplaceSet(const char *oldSet, char _new) {
	if (oldSet) {
		int32_t length = strlen(oldSet);
		for (int i=0;i<length;i++) {
			ReplaceAll(oldSet[i], _new);
		}
	}
	return *this;
}

VString &VString::IReplace(const char *old, const char *_new, int32_t count, int32_t offset) {
	if (old && _new) {
		int32_t length = strlen(old);
		int32_t new_length = strlen(_new);
		if (length > 0) {
			for (int i=0;i<count;i++) {
				int32_t loc = IFindFirst(old, offset);
				if (loc >= 0) {
					Remove(loc, length);
					if (new_length > 0) {
						Insert(_new, loc);
					}
					offset = loc+new_length;
				}
			}
		}
	}
	return *this;
}

VString &VString::IReplace(char old, char _new, int32_t count, int32_t offset) {
	while (count > 0 && offset < bufferLen) {
		if (toupper(buf[offset]) == toupper(old)) {
			count--;
			buf[offset] = _new;
		}
		offset++;
	}
	return *this;
}

VString &VString::IReplaceFirst(const char *old, const char *_new) {
	if (old && _new) {
		int32_t length = strlen(old);
		int32_t new_length = strlen(_new);
		int32_t loc = IFindFirst(old);
		if (loc >= 0) {
			Remove(loc, length);
			if (new_length > 0) {
				Insert(_new, loc);
			}
		}
	}
	return *this;
}

VString &VString::IReplaceFirst(char old, char _new) {
	int32_t offset1 = convertFindResult(strchr(buf, toupper(old)), buf);
	int32_t offset2 = convertFindResult(strchr(buf, tolower(old)), buf);
	if (offset1 >= 0) {
		if (offset2 >=0) {
			buf[min(offset1,offset2)] = _new;
		} else {
			buf[offset1] = _new;
		}
	} else if (offset2 >=0) {
		buf[offset2] = _new;
	}
	return *this;
}

VString &VString::IReplaceLast(const char *old, const char *_new) {
	if (old && _new) {
		int32_t length = strlen(old);
		int32_t new_length = strlen(_new);
		int32_t loc = IFindLast(old);
		if (loc >= 0) {
			Remove(loc, length);
			if (new_length > 0) {
				Insert(_new, loc);
			}
		}
	}
	return *this;
}

VString &VString::IReplaceLast(char old, char _new) {
	int32_t offset1 = convertFindResult(strrchr(buf, toupper(old)), buf);
	int32_t offset2 = convertFindResult(strrchr(buf, tolower(old)), buf);
	if (offset1 >= 0) {
		if (offset2 >=0) {
			buf[max(offset1,offset2)] = _new;
		} else {
			buf[offset1] = _new;
		}
	} else if (offset2 >=0) {
		buf[offset2] = _new;
	}
	return *this;
}

VString &VString::IReplaceAll(const char *old, const char *_new, int32_t offset) {
	if (old && _new) {
		int32_t length = strlen(old);
		int32_t new_length = strlen(_new);
		if (length > 0) {
			int32_t loc = IFindFirst(old, offset);
			while (loc >= 0) {
				Remove(loc, length);
				if (new_length > 0) {
					Insert(_new, loc);
				}
				offset = loc+new_length;
				loc = IFindFirst(old, offset);
			}
		}
	}
	return *this;
}

VString &VString::IReplaceAll(char old, char _new, int32_t offset) {
	while (offset < bufferLen) {
		if (toupper(buf[offset]) == toupper(old)) {
			buf[offset] = _new;
		}
		offset++;
	}
	return *this;
}

VString &VString::SetTo(const char *source, int32_t charCount) {
	if (source && charCount > 0) {
		int32_t len = strlen(source);
		if (charCount > len) charCount = len;
		makeBuffer(charCount+1);
		memmove(buf, source, charCount);

	} else {
		makeBuffer(1);
	}
	return *this;
}

VString &VString::SetTo(const VString &source) {
	return SetTo(source.buf);
}

VString &VString::SetTo(const VString &source, int32_t charCount) {
	return SetTo(source.buf, charCount);
}

VString &VString::SetTo(char c, int32_t charCount) {
	if (charCount > 0) {
		makeBuffer(charCount+1);
		memset(buf, c, charCount);

	} else {
		makeBuffer(1);
	}
	return *this;
}

VString &VString::Adopt(VString &source) {
	SetTo(source);
	source.makeBuffer(1);
	return *this;
}

VString &VString::Adopt(VString &source, int32_t charCount) {
	SetTo(source, charCount);
	source.makeBuffer(1);
	return *this;
}

VString &VString::ToLower(void) {
	for (int i=0;i<bufferLen;i++) {
		buf[i] = tolower(buf[i]);
	}
	return *this;
}

VString &VString::ToUpper(void) {
	for (int i=0;i<bufferLen;i++) {
		buf[i] = toupper(buf[i]);
	}
	return *this;
}

VString &VString::Capitalize(void) {
	int pos = 0;
	while (pos < bufferLen) {
		if (isalpha(buf[pos])) {
			buf[pos] = toupper(buf[pos]);
			break;
		}
		pos++;
	}
	return *this;
}

VString &VString::CapitalizeEachWord(void) {
	bool in_middle = false;
	for(int32_t i=0;i<bufferLen;i++) {
		if (isalpha(buf[i])) {
			if (!in_middle) {
				buf[i] = toupper(buf[i]);
				in_middle = true;
			}
		} else {
			in_middle = false;
		}
	}
	return *this;
}

VString &VString::Truncate(int32_t charCount, bool lazy) {
	SetTo(Substring(0, charCount));
	return *this;
}


VString& VString::operator=(const VString &string) {
	if (this != &string) {
		int32_t length = string.Length();
		if (length > 0) {
			makeBuffer(length+1);
			memmove(buf, string.buf, length);
		} else {
			makeBuffer(1);
		}
	}
	return *this;
}

VString& VString::operator=(const char *string) {
	if (string) {
		int32_t length = strlen(string);
		if (length > 0) {
			makeBuffer(length+1);
			memmove(buf, string, length);
		} else {
			makeBuffer(1);
		}
	} else {
		makeBuffer(1);
	}
	return *this;
}

VString& VString::operator=(const char character) {
	makeBuffer(2);
	buf[0] = character;
	return *this;
}


VString& VString::operator+=(const char *string) {
	Append(string);
	return *this;
}

VString& VString::operator+=(const char character) {
	Append(character, 1);
	return *this;
}


VString &VString::operator<<(const char *string) {
	Append(string);
	return *this;
}

VString &VString::operator<<(const VString &string) {
	Append(string);
	return *this;
}

VString &VString::operator<<(char c) {
	Append(c, 1);
	return *this;
}

VString &VString::operator<<(uint32_t val) {
	char s[256];
	bzero(s, 256);
	sprintf(s, "%u", val);
	Append(s);
	return *this;
}

VString &VString::operator<<(int32_t val) {
	char s[256];
	bzero(s, 256);
	sprintf(s, "%d", val);
	Append(s);
	return *this;
}

VString &VString::operator<<(uint64_t val) {
	char s[256];
	bzero(s, 256);
	sprintf(s, "%Lu", val);
	Append(s);
	return *this;
}

VString &VString::operator<<(int64_t val) {
	char s[256];
	bzero(s, 256);
	sprintf(s, "%Ld", val);
	Append(s);
	return *this;
}

VString &VString::operator<<(float val) {
	char s[256];
	bzero(s, 256);
	sprintf(s, "%f", val);
	Append(s);
	return *this;
}

char VString::operator[](int32_t index) const {
	return ByteAt(index);
}


bool VString::operator==(const char *string) const {
	if (string) {
		return strcmp(buf, string) == 0;
	}
	return false;
}

bool VString::operator<(const char *string) const {
	if (string) {
		return strcmp(buf, string) < 0;
	}
	return false;
}

bool VString::operator<=(const char *string) const {
	if (string) {
		return strcmp(buf, string) <= 0;
	}
	return false;
}

bool VString::operator>(const char *string) const {
	if (string) {
		return strcmp(buf, string) > 0;
	}
	return true;
}

bool VString::operator>=(const char *string) const {
	if (string) {
		return strcmp(buf, string) >= 0;
	}
	return true;
}


uint32_t VString::hash(void) {
	const uint32_t shift = 6;
	const uint32_t mask = ~0 << (32 - shift);
	uint32_t result = 0;

	int pos = 0;
	while (pos < bufferLen) {
		result = (result & mask) ^ (result << shift) ^ buf[pos++];
	}
	return result;
}
