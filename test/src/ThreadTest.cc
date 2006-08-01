/***************************************************************************
 *            ThreadTest.cc
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

#include "ThreadTest.h"

#include <iostream>

using namespace std;
using namespace tekhne;



void ThreadTest::setUp() {
}

void ThreadTest::tearDown() {
}

int _count = 0;

void *do_stuff(void *x) {
	VLocker *lock = (VLocker *)x;
	
	while(true) {
		lock->Lock();
		if (_count >= 20) {
			lock->Unlock();
			return NULL;
		}
		_count++;
		lock->Unlock();
	}
	return NULL;
}

void ThreadTest::testLock() {
	lock.Lock();
	
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&thread[0], &attr, do_stuff, &lock);
	pthread_detach(thread[0]);
	pthread_create(&thread[1], &attr, do_stuff, &lock);
	pthread_detach(thread[1]);
	lock.Unlock();
	// wait for the others to finish
	pthread_yield();
	while(true) {
		usleep(10);
		lock.Lock();
		if (_count >= 20) {
			lock.Unlock();
			break;
		}
		lock.Unlock();
	}
}

void ThreadTest::testLockCount() {
}
