/***************************************************************************
 *            StandardDefs.h
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

#ifndef _STANDARDDEFS_H
#define _STANDARDDEFS_H

#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

#include <pthread.h>

namespace tekhne {

typedef uint32_t type_code;
typedef int64_t bigtime_t;
typedef int32_t team_t;
typedef int32_t status_t;
typedef int32_t port_t; // need to fix this
typedef pthread_t thread_t;

const bigtime_t V_INFINITE_TIMEOUT = INT64_MAX;

const int32_t V_LOW_PRIORITY = -1;
const int32_t V_NORMAL_PRIORITY = 0;
const int32_t V_DISPLAY_PRIORITY = 1;
const int32_t V_URGENT_DISPLAY_PRIORITY = 2;
const int32_t V_REAL_TIME_DISPLAY_PRIORITY = 3;
const int32_t V_URGENT_PRIORITY = 4;
const int32_t V_REAL_TIME_PRIORITY = 5;


// type codes	
const int32_t V_BOOL_TYPE = 1;
const int32_t V_INT8_TYPE = 2;
const int32_t V_INT16_TYPE = 3;
const int32_t V_INT32_TYPE = 4;
const int32_t V_INT64_TYPE = 5;
const int32_t V_FLOAT_TYPE = 6;
const int32_t V_DOUBLE_TYPE = 7;
const int32_t V_POINT_TYPE = 8;
const int32_t V_RECT_TYPE = 9;
const int32_t V_STRING_TYPE = 10;
const int32_t V_MESSAGE_TYPE = 11;
const int32_t V_MESSENGER_TYPE = 12;
const int32_t V_POINTER_TYPE = 13;
const int32_t V_FLAT_TYPE = 14;

// random constants
const int32_t V_NAME_LENGTH = 255;
const int32_t V_MIME_TYPE_LENGTH = 255;
const int32_t V_LOOPER_PORT_DEFAULT_CAPACITY = 100;

}

#endif /* _STANDARDDEFS_H */
