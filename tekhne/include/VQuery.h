/***************************************************************************
 *            VQuery.h
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

#ifndef _VQUERY_H
#define _VQUERY_H

namespace tekhne {

const int32_t V_EQ = 1;
const int32_t V_NE = 2;
const int32_t V_GT = 3;
const int32_t V_LT = 4;
const int32_t V_GE = 5;
const int32_t V_LE = 6;
const int32_t V_CONTAINS = 7;
const int32_t V_BEGINS_WITH = 8;
const int32_t V_ENDS_WITH = 9;
const int32_t V_AND = 10;
const int32_t V_OR = 11;
const int32_t V_NOT = 12;

} // namespace tekhne

#endif /* _VQUERY_H */
