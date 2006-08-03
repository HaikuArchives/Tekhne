/***************************************************************************
 *            VStopWatch.h
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

#ifndef _VSTOPWATCH_H
#define _VSTOPWATCH_H

#include "VErrors.h"

namespace tekhne {

class VStopWatch {
private:
	const char *_name;
	bool _silent;
	VList _laps;
	bigtime_t _starttime;
	bigtime_t _suspendtime;

public:
	VStopWatch(const char *name, bool silent = false);
	~VStopWatch();

	bigtime_t ElapsedTime(void) const;

	void Save(void *pointer, size_t size);

	bigtime_t Lap();

	const char *Name(void) const;
	
	void Suspend(void);
	void Resume(void);
	void Reset(void);
};

} // namespace tekhne

#endif /* _VSTOPWATCH_H */
