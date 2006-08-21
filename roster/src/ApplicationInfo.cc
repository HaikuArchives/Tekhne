/***************************************************************************
 *            Roster.cc
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

#include "IRoster.h"
#include "ApplicationInfo.h"

using namespace tekhne;

ApplicationInfo::ApplicationInfo(VString signature, team_t team) : _app_info(new app_info()) {
	strncpy (_app_info->signature, signature.String(), V_MIME_TYPE_LENGTH);
	// find the thread and team for this
	_app_info->thread = team;
	_app_info->team = team;
	_app_info->flags = 0;
}

ApplicationInfo::~ApplicationInfo() {
	delete _app_info;
}

bool ApplicationInfo::IsMe(VString &s) {
	return s == _app_info->signature;
}

team_t ApplicationInfo::Team(void) {
	return _app_info->team;
}

thread_t ApplicationInfo::Thread(void) {
	return _app_info->thread;
}

uint32_t ApplicationInfo::Flags(void) {
	return _app_info->flags;
}

const char *ApplicationInfo::Signature(void) {
	return _app_info->signature;
}
