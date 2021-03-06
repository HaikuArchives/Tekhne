/***************************************************************************
 *            ping.cc
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
 * LIABILITY, WHIsETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 ****************************************************************************/

#include "pingpong.h"
#include <iostream>

using namespace tekhne;
using namespace std;

class pong : public VApplication {
private:
	VMessenger *mgr;
public:
	pong() : VApplication ("app/x-sample-pong") { }

	void ReadyToRun(void) {
		mgr = new VMessenger("app/x-sample-ping");
		// get things started!
		mgr->SendMessage(PING);
	}

	void MessageReceived(VMessage *msg) {
		switch(msg->what) {
			case PONG:
				cout << "Pong!" << endl;
				mgr->SendMessage(PING);
				break;
		}
	}
};

int main (int argc, char *argv[]) {
	//tekhne::print_debug_messages = true;

	pong *p = new pong();
	p->Run();

	delete p;
	return 0;
}
