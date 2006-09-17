/***************************************************************************
 *            VWindow.h
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

#ifndef _VWINDOW_H
#define _VWINDOW_H

#include <stdint.h>

namespace tekhne {

const int32_t V_NOT_MOVABLE = 0;
const int32_t V_NOT_CLOSABLE = 1;
const int32_t V_NOT_H_RESIZABLE = 2;
const int32_t V_NOT_ZOOMABLE = 3;
const int32_t V_NOT_V_RESIZABLE = 4;
const int32_t V_NOT_MINIMIZABLE = 5;
const int32_t V_NOT_RESIZABLE = 6;
const int32_t V_WILL_FLOAT = 7;
const int32_t V_WILL_ACCEPT_FIRST_CLICK = 8;

const int32_t V_CURRENT_WORKSPACE = 0;
const int32_t V_ALL_WORKSPACES = 1;

typedef enum {
	V_TITLED_WINDOW,
	V_DOCUMENT_WINDOW,
	V_MODAL_WINDOW,
	V_FLOATING_WINDOW,
	V_BORDERED_WINDOW,
	V_UNTYPED_WINDOW
} window_type;

} // namespace tekhne

#endif /* _VWINDOW_H */
