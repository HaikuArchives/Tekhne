/***************************************************************************
 *            VRegion.h
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

#ifndef _VREGION_H
#define _VREGION_H

#include "VErrors.h"
#include "math.h"
#include "VRect.h"
#include "VTList.h"

namespace tekhne {

class VRegion {
private:
	VTList<VRect*> *_rects;
public:
	VRegion(const VRegion& region);
	VRegion(const VRect rect);
	VRegion(void);
	virtual ~VRegion();

	bool Contains(VPoint point) const;
	VRect Frame(void) const;
	bool Intersects(VRect rect) const;
	void MakeEmpty(void);
	void OffsetBy(int32_t horizontal, int32_t vertical);
	void PrintToStream(void) const;
	VRect RectAt(int32_t index);
	int32_t CountRects(void);
	void Set(VRect rect);
	void Include(VRect rect);
	void Include(const VRegion *region);
	void Exclude(VRect rect);
	void Exclude(const VRegion *region);
	void IntersectWith(const VRegion *region);

	VRegion& operator =(const VRegion& region);
};

}

#endif /* _VREGION_H */
