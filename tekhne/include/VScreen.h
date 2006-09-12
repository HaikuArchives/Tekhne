/***************************************************************************
 *            VScreen.h
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

#ifndef _VSCREEN_H
#define _VSCREEN_H

#include "VErrors.h"
#include "VList.h"
#include "VLocker.h"

namespace tekhne {

const int32_t V_MAIN_SCREEN_ID = 0;

typedef struct {
	int32 id;
	rgb_color color_list[256];
	uint8 inversion_map[256];
	uint8 index_map[32768];
} color_map;

class VScreen {
	private:
	public:
		VScreen(VWindow *window);
		VScreen(int32_t id = V_MAIN_SCREEN_ID);
		~VScreen();

		const color_map *ColorMap(void);
		inline uint8 IndexForColor(rgb_color color);
		uint8 IndexForColor(uint8 red, uint8 green, uint8 blue, uint8 alpha = 255);
		rgb_color ColorForIndex(const uint8 index);
		uint8 InvertIndex(uint8 index);
};

} // namespace tekhne

#endif /* _VSCREEN_H */
