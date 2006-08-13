/***************************************************************************
 *            IRoster.h
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
 ****************************************************************************/

#ifndef _IROSTER_H
#define _IROSTER_H

namespace tekhne {

const int32_t V_ROSTER_REGISTER = 3000;
const int32_t V_ROSTER_UNREGISTER = 3002;
const int32_t V_ROSTER_ACTIVATE = 3003;
const int32_t V_ROSTER_TEAM_FOR = 3004;
const int32_t V_ROSTER_IS_RUNNING = 3005;
const int32_t V_ROSTER_START_WATCHING = 3006;
const int32_t V_ROSTER_STOP_WATCHING = 3007;
const int32_t V_ROSTER_LAUNCH = 3008;
const int32_t V_ROSTER_APP_LIST = 3009;
const int32_t V_ROSTER_APP_INFO = 3010;
const int32_t V_ROSTER_BROADCAST = 3011;

}

#endif /* _IROSTER_H */
