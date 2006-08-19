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

#include "tekhne.h"
#include "IMessenger.h"
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include <arpa/inet.h>

using namespace tekhne;
using namespace std;

VApplication *tekhne::v_app = 0;
VMessenger *tekhne::v_app_messenger = 0;
VRoster *tekhne::v_roster = 0;

namespace tekhne {

bool print_debug_messages = false;

class ReplyHandler : public VLooper {
	// this handler is used to get replies to send back to originating app
	const char *_signature;
public:
	ReplyHandler(const char *sig) : VLooper("app reply handler"), _signature(sig) {}
	void MessageReceived(VMessage *msg) {
		VString replySignature;
		msg->FindString("_replySignature", &replySignature);
		if (replySignature.Length( )) {
			msg->AddString("_replySignature", _signature);
			msg->_isSourceWaiting = false;
			msg->_isSourceRemote = true;
			SendToRemoteHost(replySignature.String(), msg, 0, 0);
		}
	}
};

class msg_thread {
private:
	bool _done;
	int32_t _socket;
	pthread_attr_t _attr;
	pthread_t _thread;
	ReplyHandler *_replyHandler;
	static void* loop(void *t) {
		tekhne::msg_thread *th = static_cast<tekhne::msg_thread *>(t);
		void *buf = malloc(4096);
		struct sockaddr_in clientname;
		size_t size;
		fd_set active_fd_set, read_fd_set;
		FD_ZERO (&active_fd_set);
		FD_SET (th->_socket, &active_fd_set);

		while (!th->_done) {
			read_fd_set = active_fd_set;
			if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
				perror ("select");
				v_app->Quit();
				return 0;
			}
			for (int i = 0; i < FD_SETSIZE; ++i) {
				if (FD_ISSET (i, &read_fd_set)) {
					if (i == th->_socket) {
						/* Connection request on original socket. */
						int new_socket;
						size = sizeof (clientname);
						new_socket = accept (th->_socket, (struct sockaddr *) &clientname, &size);
						if (new_socket < 0) {
							perror ("accept");
							v_app->Quit();
							return 0;
						}
						if (print_debug_messages) printf ("Server: connect from host %s, port %hd.\n",
							inet_ntoa (clientname.sin_addr),
							ntohs (clientname.sin_port));
						FD_SET (new_socket, &active_fd_set);
					} else {
						/* Data arriving on an already-connected socket. */
						int32_t len = recv (i, buf, 4096, 0);
						if (len > 0) {
							VMemoryIO mio(buf, len);
							VMessage msg;
							msg.Unflatten(&mio);
							if (print_debug_messages) msg.PrintToStream();
							// we don't want to create new sockets in SendToRemoteHost so add
							// this one if it isn't already in the socketDictionary
							VString replySig;
							msg.FindString( "_replySignature", &replySig);
							addSocketForSignature(replySig.String(), i);
							// now we can post it
							v_app->PostMessage(&msg, 0, th->_replyHandler);
						} else if (len < 0) {
							deleteSocket(i);
							FD_CLR (i, &active_fd_set);
						}
					}
				}
			}
		}
		free(buf);
		delete th;
		return 0;
	}
public:
	msg_thread(int32_t s, const char *sig) : _done(false), _socket(s) {
		// start the app message reply Handler
		_replyHandler = new ReplyHandler(sig);
		_replyHandler->Run();
		pthread_attr_init(&_attr);
		pthread_create(&_thread, &_attr, loop, this);
		pthread_detach(_thread);
	}
	virtual ~msg_thread() {
		pthread_attr_destroy(&_attr);
	}
	void done() {
		_done = true;
	}
};

static void termination_handler (int signum) {
	if (signum == SIGPIPE) {
		// don't do anything so the send call returns -1 and we give up
		if (print_debug_messages) cout << "Ignored signal: " << strsignal(signum) << endl;
	} else {
		if (print_debug_messages) cout << "Got signal: " << strsignal(signum) << endl;
		v_app->Quit();
	}
}
static void setup_termination_handler(void) {
	if (signal (SIGINT, tekhne::termination_handler) == SIG_IGN)
		signal (SIGINT, SIG_IGN);
	if (signal (SIGHUP, tekhne::termination_handler) == SIG_IGN)
		signal (SIGHUP, SIG_IGN);
	if (signal (SIGTERM, tekhne::termination_handler) == SIG_IGN)
		signal (SIGTERM, SIG_IGN);
	if (signal (SIGPIPE, tekhne::termination_handler) == SIG_IGN)
		signal (SIGPIPE, SIG_IGN);
}
}

VApplication::VApplication(const char *signature) :
	VLooper(), _pulse_thread(0), _signature(signature), _isLaunching(true), _socket(-1),
	_msg_thread(0) {
	if (v_app) {
		cout << "Trying to create a second VApplication!" << endl;
		cout << "Exiting..." << endl;
		exit(-1);
	}
	setup_termination_handler();
	v_app = this;
	int32_t err;
	v_app_messenger = new VMessenger(this, this, &err);
	// this will resgister with the roster app
	v_roster = new VRoster();
	open_server_socket();
	InjectStartupMessages();
}

VApplication::VApplication(const char *signature, status_t *error) :
	VLooper(), _pulse_thread(0), _signature(signature), _isLaunching(true), _socket(-1),
	_msg_thread(0) {
	if (v_app) {
		cout << "Trying to create a second VApplication!" << endl;
		cout << "Exiting..." << endl;
		exit(-1);
	}
	setup_termination_handler();
	v_app = this;
	int32_t err;
	v_app_messenger = new VMessenger(this, this, &err);
	// this will resgister with the roster app
	v_roster = new VRoster();
	open_server_socket();
	InjectStartupMessages();
}

VApplication::VApplication(VMessage *archive) :
	VLooper(archive), _pulse_thread(0), _signature(0), _isLaunching(true), _socket(-1),
	_msg_thread(0) {
	if (v_app) {
		cout << "Trying to create a second VApplication!" << endl;
		cout << "Exiting..." << endl;
		exit(-1);
	}
	setup_termination_handler();
	v_app = this;
	archive->FindString("mime_sig", &_signature);
	int32_t err;
	v_app_messenger = new VMessenger(this, this, &err);
	// this will resgister with the roster app
	v_roster = new VRoster();
	open_server_socket();
	InjectStartupMessages();
}

VApplication::~VApplication() {
	// this will unresgister from the roster app
	delete v_roster;
	if (_socket >= 0) {
		close(_socket);
		VString socket_name("/tmp/");
		socket_name.Append(_signature);
		socket_name.ReplaceAll('/', '-', 5);
		unlink(socket_name.String());
		if (_msg_thread) {
			_msg_thread->done();
		}
	}
	v_app = 0;
	delete v_app_messenger;
	v_app_messenger = 0;
}

//VResources *VApplication::AppResources(void) {
//}

void VApplication::AboutRequested(void) {
	if (print_debug_messages) cout << "AboutRequested" << endl;
}

void VApplication::AppActivated(bool active) {
	if (print_debug_messages) cout << "AppActivated" << endl;
}

void VApplication::ArgvReceived(int32_t argc, char **argv) {
	if (print_debug_messages) cout << "ArgvReceived" << endl;
}

status_t VApplication::GetAppInfo(app_info *theInfo) const {
	if (theInfo) {
		theInfo->thread = pthread_self();
		theInfo->team = 0; // this needs to be set to the process id
		theInfo->port = 0;
		theInfo->flags = 0;
		if (_signature.CountChars() > V_MIME_TYPE_LENGTH) {
			strncpy(theInfo->signature, _signature.String(), V_MIME_TYPE_LENGTH);
		} else {
			strcpy(theInfo->signature, _signature.String());
		}
		return V_OK;
	}
	return V_BAD_VALUE;
}

status_t VApplication::GetSupportedSuites(VMessage *message) {
	return V_ERROR;
}

bool VApplication::IsLaunching(void) const {
	return _isLaunching;
}

void VApplication::Pulse(void) {
	if (print_debug_messages) cout << "Pulse" << endl;
}

class tekhne::pulse_thread {
private:
	bool _done;
	bigtime_t _rate;
	pthread_attr_t _attr;
	pthread_t _thread;
	static void* loop(void *t) {
		tekhne::pulse_thread *th = static_cast<tekhne::pulse_thread *>(t);
		while (!th->_done) {
			usleep(th->_rate);
			if (!th->_done) {
				v_app_messenger->SendMessage(V_PULSE);
			}
		}
		delete th;
		return 0;
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
	if (_pulse_thread) {
		_pulse_thread->done();
	}
	if (rate > 0) {
		_pulse_thread = new pulse_thread(rate);
	}
}

bool VApplication::QuitRequested(void) {
	if (print_debug_messages) cout << "QuitRequested" << endl;
	return true;
}

void VApplication::ReadyToRun(void) {
	if (print_debug_messages) cout << "ReadyToRun" << endl;
}

void VApplication::RefsReceived(VMessage *message) {
	if (print_debug_messages) cout << "RefsReceived" << endl;
}

void VApplication::ProcessMessage(VMessage *msg) {
	VAutoLock lock(this);
	_currentMessage = msg;
	switch(_currentMessage->what) {
		case V_ABOUT_REQUESTED:
			AboutRequested();
			break;
		case V_APP_ACTIVATED:
			{
				bool active = true;
				_currentMessage->FindBool("active", &active);
				AppActivated(active);
			}
			break;
		case V_ARGV_RECEIVED:
			{
				int32_t argc;
				const char *argv[argc];
				const char *cwd;
				_currentMessage->FindInt32("argc", &argc);
				for(int i=0; i<argc;i++) {
					_currentMessage->FindString("argv", &argv[i]);
				}
				_currentMessage->FindString("cwd", &cwd);
				ArgvReceived(argc, (char **)argv);
			}
			break;
		case V_OPEN_IN_WORKSPACE:
			{
				int32_t workspace;
				_currentMessage->FindInt32("v:workspace", &workspace);
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
			_isLaunching = false;
			break;
		case V_REFS_RECEIVED:
			// we'll never get this...
			break;
		case V_SILENT_RELAUNCH:
			// not sure what to do about this
			break;
		default:
			DispatchMessage(_currentMessage, 0);
	}
	delete _currentMessage;
	_currentMessage = 0;
}

thread_t VApplication::Run(void) {
	if (print_debug_messages) cout << "Run" << endl;
	while (!_quitting) {
		VMessage *msg = MessageQueue()->NextMessage();
		if (msg) {
			ProcessMessage(msg);
		} else {
			if (tekhne::print_debug_messages) cout << "Got null message in application message loop\n";
		}
	}
	return 0;
}

void VApplication::Quit(void) {
	if (print_debug_messages) cout << "Quit" << endl;
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

VArchivable *VApplication::Instantiate(VMessage *archive) {
	return new VApplication(archive);
}
status_t VApplication::Archive(VMessage *archive, bool deep) const {
	// Every Archive method should look like this...
	if (archive) {
		// add class specific stuff here
		archive->AddString("mime_sig", _signature);

		if (deep) {
			// call return super::Archive()
			return VLooper::Archive(archive);
		}
		return V_OK;
	}
	return V_BAD_VALUE;
}

void VApplication::InjectStartupMessages(void) {
	VMessage *msg = new VMessage(V_APP_ACTIVATED);
	msg->AddBool("active", true);
	PostMessage(msg);
	PostMessage(V_READY_TO_RUN);
}

int32_t VApplication::open_server_socket() {
	if (_signature.Length() > 0) {
		int err;
		// Create socket
		_socket = socket(PF_LOCAL, SOCK_STREAM, 0);
		if (_socket > 0) {
			struct linger l = { 1, 1 };
			err = setsockopt(_socket, SOL_SOCKET, SO_LINGER, &l, sizeof(struct linger));
			if (err) {
				close(_socket);
				_socket = -1;
			} else {
				// it's a server socket
				struct sockaddr_un name;
				name.sun_family = AF_LOCAL;
				VString socket_name("/tmp/");
				socket_name.Append(_signature);
				socket_name.ReplaceAll('/', '-', 5);
				// make sure this is gone
				unlink(socket_name.String());
				// limit this to 108 bytes...
				strncpy(name.sun_path, socket_name.String(), sizeof (name.sun_path));
				err = bind(_socket, (struct sockaddr*)&name, SUN_LEN(&name));
				if (err) {
					close(_socket);
					_socket = -1;
				} else {
					err = listen(_socket, 5);
					if (err) {
						close(_socket);
						_socket = -1;
					} else {
						_msg_thread = new msg_thread(_socket, _signature.String());
					}
				}
			}
		}
	}
	if (_socket == -1) {
		if (print_debug_messages) cout << "Couldn't create Server socket. Exiting..." << endl;
		Quit();
	}
	return _socket;
}
