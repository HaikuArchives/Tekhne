/***************************************************************************
 *            VMessageQueue.cc
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

VMessageQueue::VMessageQueue(void) {
	sem_init (&_sem, false, 0);
}

VMessageQueue::~VMessageQueue() {
	sem_destroy (&_sem);
}

void VMessageQueue::AddMessage(VMessage *message) {
	_list.AddItem(message);
	sem_post(&_sem);
}

void VMessageQueue::RemoveMessage(VMessage *message) {
	_list.RemoveItem(message);
}

int32_t VMessageQueue::CountMessages(void) const {
	return _list.CountItems();
}

bool VMessageQueue::IsEmpty(void) const {
	return _list.IsEmpty();
}

VMessage *VMessageQueue::FindMessage(int32_t index) const {
	return (VMessage *)_list.GetItem(index);
}

VMessage *VMessageQueue::FindMessage(uint32_t what, int32_t index) const {
	VMessage **items = (VMessage **)_list.Items();
	for (int i=0;i<_list.CountItems();i++) {
		VMessage *m = items[i];
		if (m->what == what) {
			if (index == 0) {
				return m;
			}
			index--;
		}
	}
	return NULL;
}

bool VMessageQueue::Lock(void) {
	return _lock.Lock();
}

status_t VMessageQueue::LockWithTimeout(bigtime_t timeout) {
	return _lock.LockWithTimeout(timeout);
}

void VMessageQueue::Unlock(void) {
	_lock.Unlock();
}

VMessage *VMessageQueue::NextMessage(void) {
	sem_wait(&_sem);
	VMessage *m = NULL;
	m = (VMessage *)_list.FirstItem();
	_list.RemoveItem(0);
	return m;	
}
