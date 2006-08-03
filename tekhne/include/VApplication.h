/***************************************************************************
 *            VApplication.h
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

#ifndef _VAPPLICATION_H
#define _VAPPLICATION_H

#include "VLooper.h"

namespace tekhne {

class VMessage;
class pulse_thread;
	
typedef struct app_info {
	thread_t thread;
	team_t team;
	port_t port;
	uint32_t flags;
	// entry_ref ref;
	char signature[V_MIME_TYPE_LENGTH];
	app_info(void) {}
	~app_info(void) {}
} app_info;

class VApplication : public VLooper {
private:
	void InjectStartupMessages(void);
	pulse_thread *_pulse_thread;
	VString _signature;
public:
	VApplication(const char *signature);
	VApplication(const char *signature, status_t *error);
	VApplication(VMessage *archive);

	virtual ~VApplication();

//	static VResources *AppResources(void);

	virtual void AboutRequested(void);

	virtual void AppActivated(bool active);

	virtual void ArgvReceived(int32_t argc, char **argv);

	status_t GetAppInfo(app_info *theInfo) const;

	virtual status_t GetSupportedSuites(VMessage *message);

	bool IsLaunching(void) const;

	virtual void Pulse(void);
	void SetPulseRate(bigtime_t rate);

	virtual bool QuitRequested(void);
	
	virtual void ReadyToRun(void);
	
	virtual void RefsReceived(VMessage *message);
	
	virtual thread_t Run(void);
	virtual void Quit(void);
	
	void SetCursor(const void *cursor);
	//void SetCursor(const VCursor *cursor, bool sync = true);
	void HideCursor(void);
	void ShowCursor(void);
	void ObscureCursor(void);
	bool IsCursorHidden(void) const;
	
	//VWindow *WindowAt(int32_t index) const;
	int32_t CountWindows(void) const;
	
	// VArchivable methods
	static VArchivable *Instantiate(VMessage *archive);
	virtual status_t Archive(VMessage *archive, bool deep = true) const;
};

extern VApplication *v_app;
extern VMessenger *v_app_messenger;
}

#endif /* _VAPPLICATION_H */
