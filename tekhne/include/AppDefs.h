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

}
