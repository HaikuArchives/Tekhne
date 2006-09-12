/***************************************************************************
 *            GraphicsDefs.h
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

#ifndef _GRAPHICSDEFS_H
#define _GRAPHICSDEFS_H

#include <stdint.h>

namespace tekhne {

const int32_t V_NO_COLOR_SPACE = 0;
const int32_t V_RGB32 = 1;
const int32_t V_RGBA32 = 2;
const int32_t V_RGB24 = 3;
const int32_t V_RGB16 = 4;
const int32_t V_RGB15 = 5;
const int32_t V_RGBA15 = 6;
const int32_t V_CMAP8 = 7;
const int32_t V_GRAY8 = 8;
const int32_t V_GRAY1 = 9;
const int32_t V_RGB32_BIG = 10;
const int32_t V_RGBA32_BIG = 11;
const int32_t V_RGB24_BIG = 12;
const int32_t V_RGB16_BIG = 13;
const int32_t V_RGB15_BIG = 14;
const int32_t V_RGBA15_BIG = 15;
const int32_t V_HSI24 = 16;
const int32_t V_HSI32 = 17;
const int32_t V_HSIA32 = 18;
const int32_t V_HSV24 = 19;
const int32_t V_HSV32 = 20;
const int32_t V_HSVA32 = 21;
const int32_t V_HLS24 = 22;
const int32_t V_HLS32 = 23;
const int32_t V_HLSA32 = 24;
const int32_t V_CMY24 = 25;
const int32_t V_CMY32 = 26;
const int32_t V_CMYA32 = 27;
const int32_t V_CMYK32 = 28;

const int32_t V_OP_COPY = 0;
const int32_t V_OP_ADD = 1;
const int32_t V_OP_OVER = 2;
const int32_t V_OP_SUBTRACT = 3;
const int32_t V_OP_ERASE = 4;
const int32_t V_OP_BLEND = 5;
const int32_t V_OP_INVERT = 6;
const int32_t V_OP_MIN = 7;
const int32_t V_OP_SELECT = 8;
const int32_t V_OP_MAX = 9;
const int32_t V_OP_ALPHA = 10;

typedef uint8_t pattern[8];

const pattern V_SOLID_HIGH = { 0xff, 0xff, 0xff, 0xff, 0xff,0xff, 0xff, 0xff };
const pattern V_SOLID_LOW = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const pattern V_MIXED_COLORS = { 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55 };

} // namespace tekhne

#endif /* _GRAPHICSDEFS_H */
