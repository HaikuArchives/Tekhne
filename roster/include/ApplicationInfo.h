/***************************************************************************
 *            ApplicationInfo.h
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
 ****************************************************************************/

#include "tekhne.h"

#ifndef _APPLICATIONINFO_H
#define _APPLICATIONINFO_H

class ApplicationInfo {
private:
	tekhne::app_info *_app_info;
public:
	ApplicationInfo(tekhne::VString sig, tekhne::team_t team);
	virtual ~ApplicationInfo();

	bool IsMe(tekhne::VString &s);

	tekhne::team_t Team(void);
	tekhne::thread_t Thread(void);
	uint32_t Flags(void);
	const char *Signature(void);
};

#endif /* _APPLICATIONINFO_H */
