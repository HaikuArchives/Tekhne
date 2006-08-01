/***************************************************************************
 *            VPoint.h
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

#ifndef _VPOINT_H
#define _VPOINT_H

namespace tekhne {
class VRect;

class VPoint {
public:
	float x;
	float y;

	inline VPoint(float x, float y) { this->x = x; this->y = y; }
	inline VPoint(const VPoint& point) { x = point.x; y = point.y; }
	inline VPoint(void) { x = 0; y = 0; }
	
	inline void Set(float x, float y) { this->x = x; this->y = y; }
	inline VPoint& operator =(const VPoint &from) { x = from.x; y = from.y; return *this; }

	void ConstrainTo(VRect rect);
	void PrintToStream(void) const;
	bool operator ==(VPoint) const;
	bool operator !=(VPoint) const;
	VPoint operator +(const VPoint&) const;
	VPoint& operator +=(const VPoint&);
	
	VPoint operator -(const VPoint&) const;
	VPoint& operator -=(const VPoint&);
};

const VPoint V_ORIGIN(0, 0);

} // namespace tekhne

#endif /* _VPOINT_H */
