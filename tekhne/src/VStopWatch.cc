/***************************************************************************
 *            VStopWatch.cc
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
#include <sys/time.h>

using namespace tekhne;
using namespace std;

namespace tekhne {

typedef struct lap {
	bigtime_t _starttime;
	bigtime_t _endtime;
	lap(bigtime_t st, bigtime_t et) : _starttime(st), _endtime(et) {}
	inline bigtime_t elapsed() { return _endtime - _starttime; }
} lap;

inline bigtime_t convertTimeval(struct timeval &tv) { return 1000000L*tv.tv_sec+tv.tv_usec; }
inline bigtime_t now() {
	struct timeval ts;
	gettimeofday(&ts, 0);
	return convertTimeval(ts);
}	

}

VStopWatch::VStopWatch(const char *name, bool silent) : _name(name), _silent(silent), _suspendtime(0) {
	_starttime = now();
}

VStopWatch::~VStopWatch() {
	int endtime = now();
	if (!_silent) {
		cout << "StopWatch \"" << _name << "\": " << endtime-_starttime << " usecs." << endl;
		if (_laps.CountItems() > 0) {
			tekhne::lap *_lap = static_cast<tekhne::lap*>(_laps.LastItem());
			_lap->_endtime = endtime;
			for (int i=0; i<_laps.CountItems(); i++) {
				_lap = static_cast<tekhne::lap*>(_laps.GetItem(i));
				cout << "["<< i+1 << ": " << _lap->elapsed() << "] ";
			}
			cout << endl;
		}
	}
}

bigtime_t VStopWatch::ElapsedTime(void) const {
	return now()-_starttime;
}

void VStopWatch::Save(void *pointer, size_t size) {
}

bigtime_t VStopWatch::Lap() {
	bigtime_t t = now();
	bigtime_t elapsed = 0;
	if (_laps.CountItems() == 0) {
		_laps.AddItem(new tekhne::lap(_starttime, t));
		_laps.AddItem(new tekhne::lap(t, 0));
		elapsed = t - _starttime;
	} else {
		tekhne::lap *_lap = static_cast<tekhne::lap*>(_laps.LastItem());
		_lap->_endtime = t;
		_laps.AddItem(new tekhne::lap(t, 0));
		elapsed = t - _lap->_starttime;
	}
	return elapsed;
}

const char *VStopWatch::Name(void) const {
	return _name;
}
	
void VStopWatch::Suspend(void) {
	// don't suspend again if we are already suspended
	if (!_suspendtime) {
		_suspendtime = now();
	}
}

void VStopWatch::Resume(void) {
	if (_suspendtime) {
		_starttime += now()-_suspendtime;
	}
	_suspendtime = 0;
}

void VStopWatch::Reset(void) {
	_starttime = now();
	_suspendtime = 0;
	_laps.MakeEmpty();
}
