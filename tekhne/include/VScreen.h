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

#include "InterfaceDefs.h"

namespace tekhne {

typedef struct {
	int32_t id;
} screen_id;

extern screen_id V_MAIN_SCREEN_ID;

class VWindow;
class VBitmap;

typedef struct {
	uint32_t pixel_clock;
	uint16_t h_display;
	uint16_t h_sync_start;
	uint16_t h_sync_end;
	uint16_t h_total;
	uint16_t v_display;
	uint16_t v_sync_start;
	uint16_t v_sync_end;
	uint16_t v_total;
	uint32_t flags;
} display_timing;

// display timing flags
const int32_t V_BLANK_PEDESTAL = 1;
const int32_t V_TIMING_INTERLACED = 2;
const int32_t V_POSITIVE_HSYNC = 4;
const int32_t V_POSITIVE_VSYNC = 8;
const int32_t V_SYNC_ON_GREEN = 16;

typedef struct {
	display_timing timing;
	uint32_t space;
	uint16_t virtual_width;
	uint16_t virtual_height;
	uint16_t h_display_start;
	uint16_t v_display_start;
	uint32_t flags;
} display_mode;

// display mode flags
const int32_t V_SCROLL = 1;
const int32_t V_8_BIT_DAC = 2;
const int32_t V_HARDWARE_CURSOR = 4;
const int32_t V_PARALLEL_ACCESS = 8;
const int32_t V_DPMS = 16;
const int32_t V_IO_FB_NA = 32;

const int32_t VDPMS_ON = 0;
const int32_t V_DPMS_STAND_BY = 1;
const int32_t V_DPMS_SUSPEND = 2;
const int32_t V_DPMS_OFF = 4;

class VScreen {
private:
public:
	VScreen(VWindow *window);
	VScreen(screen_id id = V_MAIN_SCREEN_ID);
	~VScreen();

	const color_map *ColorMap(void);
	inline uint8_t IndexForColor(rgb_color color);
	uint8_t IndexForColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
	rgb_color ColorForIndex(const uint8_t index);
	uint8_t InvertIndex(uint8_t index);

	color_space ColorSpace(void);

	VRect Frame(void);

	//  need defs for these fields
	// status_t GetDeviceInfo(accelerant_device_info *info);
	// status_t GetTimingConstraints(display_timing_constraints *dtc);

	status_t GetModeList(display_mode **mode_list, uint32_t *count);
	status_t SetMode(display_mode *mode, bool makeDefault = false);
	status_t GetMode(display_mode *mode);

	status_t GetPixelClockLimits(display_mode *mode, uint32_t *low, uint32_t *high);

	screen_id ID(void);

	bool IsValid(void);

	status_t ProposeMode(display_mode *candidate, const display_mode *low, const display_mode *high);


	status_t ReadBitmap(VBitmap *buffer, bool draw_cursor = true, VRect *bounds = NULL);
	status_t GetBitmap(VBitmap **buffer, bool draw_cursor = true, VRect *bounds = NULL);

	void SetDesktopColor(rgb_color color, bool makeDefault = true);
	rgb_color DesktopColor(void);

	status_t SetDPMS(uint32_t dpmsState);
	uint32_t DPMSState(void);
	uint32_t DPMSCapabilities(void);

	status_t SetToNext(void) { return V_ERROR; }

	status_t WaitForRetrace(void);
	status_t WaitForRetrace(bigtime_t timeout);
};

} // namespace tekhne

#endif /* _VSCREEN_H */
