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

typedef enum {
	V_NO_COLOR_SPACE,
	V_RGB32,
	V_RGBA32,
	V_RGB24,
	V_RGB16,
	V_RGB15,
	V_RGBA15,
	V_CMAP8,
	V_GRAY8,
	V_GRAY1,
	V_RGB32_BIG,
	V_RGBA32_BIG,
	V_RGB24_BIG,
	V_RGB16_BIG,
	V_RGB15_BIG,
	V_RGBA15_BIG,
	V_HSI24,
	V_HSI32,
	V_HSIA32,
	V_HSV24,
	V_HSV32,
	V_HSVA32,
	V_HLS24,
	V_HLS32,
	V_HLSA32,
	V_CMY24,
	V_CMY32,
	V_CMYA32,
	V_CMYK32,
} color_space;

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

const int32_t V_8_BIT_640x400 = 1;
const int32_t V_8_BIT_640x480 = 2;
const int32_t V_15_BIT_640x480 = 3;
const int32_t V_16_BIT_640x480 = 4;
const int32_t V_32_BIT_640x480 = 5;
const int32_t V_8_BIT_800x600 = 6;
const int32_t V_15_BIT_800x600 = 7;
const int32_t V_16_BIT_800x600 = 8;
const int32_t V_32_BIT_800x600 = 9;
const int32_t V_8_BIT_1024x768 = 10;
const int32_t V_15_BIT_1024x768 = 11;
const int32_t V_16_BIT_1024x768 = 12;
const int32_t V_32_BIT_1024x768 = 13;
const int32_t V_8_BIT_1152x900 = 14;
const int32_t V_15_BIT_1152x900 = 15;
const int32_t V_16_BIT_1152x900 = 16;
const int32_t V_32_BIT_1152x900 = 17;
const int32_t V_8_BIT_1280x1024 = 18;
const int32_t V_15_BIT_1280x1024 = 18;
const int32_t V_16_BIT_1280x1024 = 20;
const int32_t V_32_BIT_1280x1024 = 21;
const int32_t V_8_BIT_1600x1200 = 22;
const int32_t V_15_BIT_1600x1200 = 23;
const int32_t V_16_BIT_1600x1200 = 24;
const int32_t V_32_BIT_1600x1200 = 25;

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
} rgb_color;

const uint8_t B_TRANSPARENT_8_BIT = 0;
const rgb_color B_TRANSPARENT_32_BIT = {0, 0, 0, 0};

typedef struct {
	int32_t id;
	rgb_color color_list[256];
	uint8_t inversion_map[256];
	uint8_t index_map[32768];
} color_map;

} // namespace tekhne

#endif /* _GRAPHICSDEFS_H */
