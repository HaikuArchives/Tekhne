/***************************************************************************
 *            VRect.h
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

#ifndef _VRECT_H
#define _VRECT_H

#include "VErrors.h"
#include "math.h"
#include "VPoint.h"

namespace tekhne {

class VRect {
public:
	float left;
	float top;
	float right;
	float bottom;

	inline VRect(float _left, float _top, float _right, float _bottom): 
		left(_left), top(_top), right(_right), bottom(_bottom) { }
	inline VRect(VPoint leftTop, VPoint rightBottom) :
		left(leftTop.x), top(leftTop.y), right(rightBottom.x), bottom(rightBottom.y) { }
	inline VRect(const VRect& rect) :
		left(rect.left), top(rect.top), right(rect.right), bottom(rect.bottom) { }
	inline VRect(void) :
		left(0), top(0), right(0), bottom(0) { }

	bool Contains(VPoint point) const;
	bool Contains(VRect rect) const;
	bool Intersects(VRect rect) const;

	void InsetBy(float x, float y);
	void InsetBy(VPoint point);
	VRect &InsetBySelf(float x, float y);
	VRect &InsetBySelf(VPoint point);
	VRect InsetByCopy(float x, float y);
	VRect InsetByCopy(VPoint point);
	void OffsetBy(float x, float y);
	void OffsetBy(VPoint point);
	VRect &OffsetBySelf(float x, float y);
	VRect &OffsetBySelf(VPoint point);
	VRect OffsetByCopy(float x, float y);
	VRect OffsetByCopy(VPoint point);
	void OffsetTo(float x, float y);
	void OffsetTo(VPoint point);
	VRect &OffsetToSelf(float x, float y);
	VRect &OffsetToSelf(VPoint point);
	VRect OffsetToCopy(float x, float y);
	VRect OffsetToCopy(VPoint point);

	inline bool IsValid(void) const {
		return right >= left && bottom >= top;
	}
	
	void PrintToStream(void) const;
	inline void Set(float left, float top, float right, float bottom) {
		this->left = left; this->top = top; this->right = right; this->bottom = bottom;
	}
	void SetLeftTop(const VPoint point);
	void SetLeftBottom(const VPoint point);
	void SetRightTop(const VPoint point);
	void SetRightBottom(const VPoint point);
	VPoint LeftTop(void) const;
	VPoint LeftBottom(void) const;
	VPoint RightTop(void) const;
	VPoint RightBottom(void) const;

	inline float Width(void) const { return right - left; }
	inline int32_t IntegerWidth(void) const { return (int32_t)(ceil(right - left)); }
	inline float Height(void) const { return bottom - top; }
	inline int32_t IntegerHeight(void) const { return (int32_t)(ceil(bottom - top)); }

	inline VRect& operator =(const VRect& from) { if (this != &from) { Set(from.left, from.top, from.right, from.bottom); } return *this; }
	bool operator ==(VRect r) const;
	bool operator !=(VRect r) const;
	VRect operator &(VRect r) const;
	VRect operator |(VRect) const;
};

} // namespace tekhne

#endif /* _VRECT_H */
