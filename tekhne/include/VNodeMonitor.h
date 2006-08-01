/***************************************************************************
 *            VNodeMonitor.h
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

#ifndef _VNODEMONITOR_H
#define _VNODEMONITOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif



#ifdef __cplusplus
}
#endif

namespace tekhne {

const int32_t V_STOP_WATCHING = 1;
const int32_t V_WATCH_NAME = 2;
const int32_t V_WATCH_STAT = 4;
const int32_t V_WATCH_ATTR = 8;
const int32_t V_WATCH_DIRECTORY = 16;
const int32_t V_WATCH_MOUNT = 32;
const int32_t V_WATCH_ALL = V_WATCH_NAME+V_WATCH_STAT+V_WATCH_ATTR+V_WATCH_DIRECTORY+V_WATCH_MOUNT;

const int32_t V_ENTRY_CREATED = 1;
const int32_t V_ENTRY_REMOVED = 2;
const int32_t V_ENTRY_MOVED = 3;
const int32_t V_STAT_CHANGED = 4;
const int32_t V_ATTR_CHANGED = 5;
const int32_t V_DEVICE_MOUNTED = 6;
const int32_t V_DEVICE_UNMOUNTED = 7;

} // namespace tekhne

#endif /* _VNODEMONITOR_H */
