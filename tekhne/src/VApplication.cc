/***************************************************************************
 *            VApplication.cc
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

#include "AppDefs.h"
#include "VMessenger.h"
#include "VApplication.h"
#include "VMessageQueue.h"
#include <iostream>

using namespace tekhne;
using namespace std;

VApplication *tekhne::v_app = NULL;
VMessenger *tekhne::v_app_messenger = NULL;

VApplication::VApplication(const char *signature) :
	VLooper(signature) {
	if (v_app != NULL) {
		cout << "Trying to create a second VApplication!" << endl;
		cout << "Exiting..." << endl;
		exit(-1);
	}
	v_app = this;
	int32_t err;
	v_app_messenger = new VMessenger(this, this, &err);
	InjectStartupMessages();
}

VApplication::VApplication(const char *signature, status_t *error) :
	VLooper(signature) {
	if (v_app != NULL) {
		cout << "Trying to create a second VApplication!" << endl;
		cout << "Exiting..." << endl;
		exit(-1);
	}
	v_app = this;
	int32_t err;
	v_app_messenger = new VMessenger(this, this, &err);
	InjectStartupMessages();
}

VApplication::VApplication(VMessage *archive) : VLooper(archive) {
	if (v_app != NULL) {
		cout << "Trying to create a second VApplication!" << endl;
		cout << "Exiting..." << endl;
		exit(-1);
	}
	v_app = this;
	int32_t err;
	v_app_messenger = new VMessenger(this, this, &err);
	InjectStartupMessages();
}

VApplication::~VApplication() {
}

//VResources *VApplication::AppResources(void) {
//}

void VApplication::AboutRequested(void) {
	cout << "AboutRequested" << endl;
}

void VApplication::AppActivated(bool active) {
	cout << "AppActivated" << endl;
}

void VApplication::ArgvReceived(int32_t argc, char **argv) {
	cout << "ArgvReceived" << endl;
}

status_t VApplication::GetAppInfo(app_info *theInfo) const {
	return V_ERROR;
}

status_t VApplication::GetSupportedSuites(VMessage *message) {
	return V_ERROR;
}

bool VApplication::IsLaunching(void) const {
	return false;
}

void VApplication::Pulse(void) {
	cout << "Pulse" << endl;
}

class tekhne::pulse_thread {
private:
	bool _done;
	bigtime_t _rate;
	pthread_attr_t _attr;
	pthread_t _thread;
	static void* loop(void *t) {
		tekhne::pulse_thread *th = (tekhne::pulse_thread *)t;
		while (!th->_done) {
			usleep(th->_rate);
			if (!th->_done) {
				v_app_messenger->SendMessage(V_PULSE);
			}
		}
		delete th;
		return NULL;
	}
public:
	pulse_thread(int32_t rate) : _done(false), _rate(rate) {
		pthread_attr_init(&_attr);
		pthread_create(&_thread, &_attr, loop, this);
		pthread_detach(_thread);
	}
	virtual ~pulse_thread() {
		pthread_attr_destroy(&_attr);
	}
	void done() {
		_done = true;
	}
};

void VApplication::SetPulseRate(bigtime_t rate) {
	if (_pulse_thread != NULL) {
		_pulse_thread->done();
	}
	if (rate > 0) {
		_pulse_thread = new pulse_thread(rate);
	}
}

bool VApplication::QuitRequested(void) {
	cout << "QuitRequested" << endl;
	return true;
}

void VApplication::ReadyToRun(void) {
	cout << "ReadyToRun" << endl;
}

void VApplication::RefsReceived(VMessage *message) {
	cout << "RefsReceived" << endl;
}

thread_t VApplication::Run(void) {
	cout << "Run" << endl;
	while (!_quitting) {
		VMessage *msg = MessageQueue()->NextMessage();
		Lock();
		switch(msg->what) {
			case V_ABOUT_REQUESTED:
				AboutRequested();
				break;
			case V_APP_ACTIVATED:
				{
					bool active = true;
					msg->FindBool("active", &active);
					AppActivated(active);
				}
				break;
			case V_ARGV_RECEIVED:
				{
					int32_t argc;
					const char *argv[argc];
					const char *cwd;
					msg->FindInt32("argc", &argc);
					for(int i=0; i<argc;i++) {
						msg->FindString("argv", &argv[i]);
					}
					msg->FindString("cwd", &cwd);
					ArgvReceived(argc, (char **)argv);
				}
				break;
			case V_OPEN_IN_WORKSPACE:
				{
					int32_t workspace;
					msg->FindInt32("v:workspace", &workspace);
				}
				break;
			case V_PULSE:
				Pulse();
				break;
			case V_QUIT_REQUESTED:
				if (QuitRequested()) {
					_quitting = true;
				}
				break;
			case V_READY_TO_RUN:
				ReadyToRun();
				break;
			case V_REFS_RECEIVED:
				// we'll never get this...
				break;
			case V_SILENT_RELAUNCH:
				// not sure what to do about this
				break;
			default:
				DispatchMessage(msg, NULL);
		}
		Unlock();
		delete msg;
	}
	return 0;
}

void VApplication::Quit(void) {
	cout << "Quit" << endl;
	PostMessage(V_QUIT_REQUESTED);
}

void VApplication::SetCursor(const void *cursor) {
}

//void VApplication::SetCursor(const VCursor *cursor, bool sync = true) {
//}

void VApplication::HideCursor(void) {
}

void VApplication::ShowCursor(void) {
}

void VApplication::ObscureCursor(void) {
}

bool VApplication::IsCursorHidden(void) const {
	return true;
}

//VWindow *VApplication::WindowAt(int32_t index) const {
//}

int32_t VApplication::CountWindows(void) const {
	return 0;
}

void VApplication::InjectStartupMessages(void) {
	VMessage *msg = new VMessage(V_APP_ACTIVATED);
	msg->AddBool("active", true);
	PostMessage(msg);
	PostMessage(V_READY_TO_RUN);
}
