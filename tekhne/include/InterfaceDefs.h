/***************************************************************************
 *            InterfaceDefs.h
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

#ifndef _INTERFACEDEFS_H
#define _INTERFACEDEFS_H

#include <stdint.h>

namespace tekhne {

const int32_t V_ALIGN_LEFT = 0;
const int32_t V_ALIGN_RIGHT = 1;
const int32_t V_ALIGN_CENTER = 2;

const int32_t V_WIDTH_AS_USUAL = 0;
const int32_t V_WIDTH_FROM_LABEL = 1;
const int32_t V_WIDTH_FROM_WIDEST = 2;

const int32_t V_PLAIN_BORDER = 0;
const int32_t V_FANCY_BORDER = 1;
const int32_t V_NO_BORDER = 2;

const int8_t V_BACKSPACE = 0x08;
const int8_t V_ENTER = 0x0a;
const int8_t V_RETURN = 0x0a;
const int8_t V_SPACE = 0x20;
const int8_t V_TAB = 0x09;
const int8_t V_ESCAPE = 0x1b;
const int8_t V_LEFT_ARROW = 0x1c;
const int8_t V_RIGHT_ARROW = 0x1d;
const int8_t V_UP_ARROW = 0x1e;
const int8_t V_DOWN_ARROW = 0x1f;
const int8_t V_INSERT = 0x05;
const int8_t V_DELETE = 0x7f;
const int8_t V_HOME = 0x01;
const int8_t V_END = 0x04;
const int8_t V_PAGE_UP = 0x0b;
const int8_t V_PAGE_DOWN = 0x0c;
const int8_t V_FUNCTION_KEY = 0x10;
//const int8_t V_UTF8_ELLIPSIS 	"xE2x80xA6"
//const int8_t V_UTF8_OPEN_QUOTE 	"xE2x80x9C"
//const int8_t V_UTF8_CLOSE_QUOTE 	"xE2x80x9D"
//const int8_t V_UTF8_COPYRIGHT 	"xC2xA9"
//const int8_t V_UTF8_REGISTERED 	"xC2xAE"
//const int8_t V_UTF8_TRADEMARK 	"xE2x84xA2"
//const int8_t V_UTF8_SMILING_FACE 	"xE2x98xBB"
//const int8_t V_UTF8_HIROSHI 	"xE5xBCx98"

const int32_t V_CONTROL_TABLE = 0;
const int32_t V_OPTION_CAPS_SHIFT_TABLE = 1;
const int32_t V_OPTION_CAPS_TABLE = 2;
const int32_t V_OPTION_SHIFT_TABLE = 3;
const int32_t V_OPTION_TABLE = 4;
const int32_t V_CAPS_SHIFT_TABLE = 5;
const int32_t V_CAPS_TABLE = 6;
const int32_t V_SHIFT_TABLE = 7;
const int32_t V_NORMAL_TABLE = 8;

// these are wrong, but OK for now.
const int8_t V_F1_KEY = 1;
const int8_t V_F2_KEY = 2;
const int8_t V_F3_KEY = 3;
const int8_t V_F4_KEY = 4;
const int8_t V_F5_KEY = 5;
const int8_t V_F6_KEY = 6;
const int8_t V_F7_KEY = 7;
const int8_t V_F8_KEY = 8;
const int8_t V_F9_KEY = 9;
const int8_t V_F10_KEY = 10;
const int8_t V_F11_KEY = 11;
const int8_t V_F12_KEY = 12;
const int8_t V_PRINT_KEY = 20;
const int8_t V_SCROLL_KEY = 21;
const int8_t V_PAUSE_KEY = 22;

const int8_t V_SHIFT_KEY = 23;
const int8_t V_OPTION_KEY = 24;
const int8_t V_LEFT_SHIFT_KEY = 25;
const int8_t V_LEFT_OPTION_KEY = 26;
const int8_t V_RIGHT_SHIFT_KEY = 27;
const int8_t V_RIGHT_OPTION_KEY = 28;
const int8_t V_CONTROL_KEY = 29;
const int8_t V_COMMAND_KEY = 30;
const int8_t V_LEFT_CONTROL_KEY = 31;
const int8_t V_LEFT_COMMAND_KEY = 32;
const int8_t V_RIGHT_CONTROL_KEY = 33;
const int8_t V_RIGHT_COMMAND_KEY = 34;
const int8_t V_CAPS_LOCK = 35;
const int8_t V_MENU_KEY = 36;
const int8_t V_SCROLL_LOCK = 37;
const int8_t V_NUM_LOC = 38;

const int8_t V_HORIZONTAL = 0;
const int8_t V_VERTICAL = 1;

} // namespace tekhne

#endif /* _INTERFACEDEFS_H */
