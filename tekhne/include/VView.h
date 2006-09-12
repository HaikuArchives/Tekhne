/***************************************************************************
 *            VView.h
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

#ifndef _VVIEW_H
#define _VVIEW_H

#include <stdint.h>

namespace tekhne {

const int32_t V_ENTERED_VIEW = 0;
const int32_t V_INSIDE_VIEW = 1;
const int32_t V_EXITED_VIEW = 2;

const int32_t V_FONT_FAMILY_AND_STYLE = 1;
const int32_t V_FONT_SIZE = 2;
const int32_t V_FONT_SHEAR = 4;
const int32_t V_FONT_ROTATION = 8;
const int32_t V_FONT_SPACING = 16;
const int32_t V_FONT_ENCODING = 32;
const int32_t V_FONT_FACE = 64;
const int32_t V_FONT_FLAGS = 128;
const int32_t V_FONT_ALL = 256;

const int32_t V_PRIMARY_MOUSE_BUTTON = 1;
const int32_t V_SECONDARY_MOUSE_BUTTON = 2;
const int32_t V_TERTIARY_MOUSE_BUTTON = 4;

const int32_t V_FOLLOW_LEFT = 0;
const int32_t V_FOLLOW_RIGHT = 1;
const int32_t V_FOLLOW_LEFT_RIGHT = 2;
const int32_t V_FOLLOW_H_CENTER = 3;
const int32_t V_FOLLOW_TOP = 4;
const int32_t V_FOLLOW_BOTTOM = 5;
const int32_t V_FOLLOW_TOP_BOTTOM = 6;
const int32_t V_FOLLOW_V_CENTER = 7;
const int32_t V_FOLLOW_ALL = 8;
const int32_t V_FOLLOW_NONE = 9;

} // namespace tekhne

#endif /* _VVIEW_H */
