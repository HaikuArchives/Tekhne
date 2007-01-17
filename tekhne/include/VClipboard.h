/***************************************************************************
 *            VClipboard.h
 *
 * Copyright (c) 2006-2007 Geoffrey Clements
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

#ifndef _VCLIPBOARD_H
#define _VCLIPBOARD_H

namespace tekhne {

class VMessage;
class VMessenger;

class VClipboard {
private:
	char _name[V_NAME_LENGTH];
	VMessage *_data;
public:
	VClipboard(const char *name, bool discard = false);
	~VClipboard();

	status_t Clear(void);
	status_t Commit(void);
	status_t Revert(void);

	VMessage *Data(void) const;
	VMessenger *DataSource(void) const;

	uint32_t LocalCount(void) const;
	uint32_t SystemCount(void) const;

	bool Lock(void);
	void Unlock(void);
	bool IsLocked(void);

	const char *Name(void) const { return _name; }

	status_t StartWatching(VMessenger *target);
	status_t StopWatching(VMessenger *target);
};

extern VClipboard *v_clipboard;

} // namespace tekhne

#endif /* _VCLIPBOARD_H */
