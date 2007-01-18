/***************************************************************************
 *            AppDefs.h
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

#include <stdint.h>

#ifndef _APPDEFS_H
#define _APPDEFS_H

namespace tekhne {

const int32_t V_ABOUT_REQUESTED = 1000;
const int32_t V_ACQUIRE_OVERLAY_LOCK = 1001;
const int32_t V_APP_ACTIVATED = 1002;
const int32_t V_ARCHIVED_OBJECT = 1003;
const int32_t V_ARGV_RECEIVED = 1004;
const int32_t V_CANCEL = 1005;
const int32_t V_CLIPBOARD_CHANGED = 1006;
const int32_t V_CONTROL_INVOKED = 1007;
const int32_t V_INPUT_DEVICES_CHANGED = 1008;
const int32_t V_INPUT_METHOD_EVENT = 1009;
const int32_t V_KEY_DOWN = 1010;
const int32_t V_KEY_UP = 1011;
const int32_t V_KEYBOARD_DEVICE = 1012;
const int32_t V_MEDIA_BUFFER_CREATED = 1013;
const int32_t V_MEDIA_BUFFER_DELETED = 1014;
const int32_t V_MEDIA_CONNECTION_MADE = 1015;
const int32_t V_MEDIA_CONNECTION_BROKEN = 1016;
const int32_t V_MEDIA_FLAVORS_CHANGED = 1017;
const int32_t V_MEDIA_FORMAT_CHANGED = 1018;
const int32_t V_MEDIA_NODE_CREATED = 1019;
const int32_t V_MEDIA_NODE_DELETED = 1020;
const int32_t V_MEDIA_NODE_STOPPED = 1021;
const int32_t V_MEDIA_PARAMETER_CHANGED = 1022;
const int32_t V_MEDIA_NEW_PARAMETER_VALUE = 1023;
const int32_t V_MEDIA_WEB_CHANGED = 1024;
const int32_t V_MEDIA_WILDCARD = 1025;
const int32_t V_MINIMIZE = 1026;
const int32_t V_MODIFIERS_CHANGED = 1027;
const int32_t V_MOUSE_DOWN = 1028;
const int32_t V_MOUSE_MOVED = 1029;
const int32_t V_MOUSE_UP = 1030;
const int32_t V_MOUSE_WHEEL_CHANGED = 1031;
const int32_t V_NODE_MONITOR = 1032;
const int32_t V_OBSERVER_NOTICE_CHANGE = 1033;
const int32_t V_OPEN_IN_WORKSPACE = 1034;
const int32_t V_POINTING_DEVICE = 1035;
const int32_t V_PRINTER_CHANGED = 1036;
const int32_t V_PULSE  = 1037;
const int32_t V_QUERY_UPDATE = 1038;
const int32_t V_QUIT_REQUESTED = 1039;
const int32_t V_READY_TO_RUN = 1040;
const int32_t V_REFS_RECEIVED = 1041;
const int32_t V_RELEASE_OVERLAY_LOCK = 1042;
const int32_t V_RESET_STATUS_BAR = 1043;
const int32_t V_SCREEN_CHANGED = 1044;
const int32_t V_SILENT_RELAUNCH = 1045;
const int32_t V_SOME_APP_ACTIVATED = 1046;
const int32_t V_SOME_APP_LAUNCHED = 1047;
const int32_t V_SOME_APP_QUIT = 1048;
const int32_t V_UNMAPPED_KEY_DOWN = 1049;
const int32_t V_UNMAPPED_KEY_UP = 1050;
const int32_t V_UPDATE_STATUS_BAR = 1051;
const int32_t V_VALUE_CHANGED = 1052;
const int32_t V_VIEW_MOVED = 1053;
const int32_t V_VIEW_RESIZED = 1054;
const int32_t V_WINDOW_ACTIVATED = 1055;
const int32_t V_WINDOW_MOVE_BY = 1056;
const int32_t V_WINDOW_MOVE_TO = 1057;
const int32_t V_WINDOW_MOVED = 1058;
const int32_t V_WINDOW_RESIZED = 1059;
const int32_t V_WORKSPACE_ACTIVATED = 1060;
const int32_t V_WORKSPACES_CHANGED = 1061;
const int32_t V_ZOOM = 1062;

const int32_t V_REPLY = 1063;
const int32_t V_NO_REPLY = 1064;
const int32_t V_MESSAGE_NOT_UNDERSTOOD = 1065;

const int32_t V_REQUEST_LAUNCHED = 1066;
const int32_t V_REQUEST_QUIT = 1067;

// VMessageFilter constants
const int32_t V_ANY_DELIVERY = 1068;
const int32_t V_PROGRAMMED_DELIVERY = 1069;
const int32_t V_DROPPED_DELIVERY = 1070;

const int32_t V_ANY_SOURCE = 1071;
const int32_t V_LOCAL_SOURCE = 1072;
const int32_t V_REMOTE_SOURCE = 1073;

const int32_t V_ARGV_ONLY = 5100;

const int32_t V_BACKGROUND_APP = 0x1;
const int32_t V_EXCLUSIVE_LAUNCH = 0x2;
const int32_t V_SINGLE_LAUNCH = 0x4;
const int32_t V_MULTIPLE_LAUNCH = 0x8;
const int32_t V_LAUNCH_MASK = 0xF;

const int32_t V_CREATE_PROPERTY = 5000;
const int32_t V_DELETE_PROPERTY = 5001;
const int32_t V_GET_PROPERTY = 5002;
const int32_t V_SET_PROPERTY = 5003;

const int32_t V_NO_SPECIFIER = 0;
const int32_t V_DIRECT_SPECIFIER = 1;
const int32_t V_ID_SPECIFIER = 2;
const int32_t V_INDEX_SPECIFIER = 3;
const int32_t V_NAME_SPECIFIER = 4;
const int32_t V_RANGE_SPECIFIER = 5;
const int32_t V_REVERSE_INDEX_SPECIFIER = 6;
const int32_t V_REVERSE_RANGE_SPECIFIER = 7;
const int32_t V_SPECIFIERS_END  = 128;

const int32_t V_COPY = 2000;
const int32_t V_CUT = 2001;
const int32_t V_PASTE = 2002;
const int32_t V_SELECT_ALL = 2003;

const int32_t V_MIME_DATA = 2020;
const int32_t V_SIMPLE_DATA = 2021;

const int32_t V_SAVE_REQUESTED = 3000;

const int32_t V_GET_SUPPORTED_SUITES = 4000;

const int32_t V_ANY_COMMAND = 0;

const int32_t V_DISPATCH_MESSAGE = 1074;
const int32_t V_SKIP_MESSAGE = 1075;

const int32_t V_MILLISECOND = 1000;
const int32_t V_SECOND = 1000*V_MILLISECOND;
const int32_t V_MINUTE = 60*V_SECOND;

// V_HAND_CURSOR
// V_I_BEAM_CURSOR

}

#endif /* _APPDEFS_H */
