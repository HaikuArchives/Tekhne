/***************************************************************************
 *            VRegion.cc
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

#include "tekhne.h"
#include <iostream>
#include <limits>

using namespace tekhne;
using namespace std;

VRegion::VRegion(const VRegion& region) {
	_rects = new VTList<VRect*>();
	for (int i=0;i<region._rects->CountItems(); i++) {
		_rects->AddItem(new VRect(*region._rects->GetItem(i)));
	}
}

VRegion::VRegion(const VRect rect) {
	_rects = new VTList<VRect*>();
	_rects->AddItem(new VRect(rect));
}

VRegion::VRegion(void) {
	_rects = new VTList<VRect*>();
}

VRegion::~VRegion(){
	for (int i=0;i<_rects->CountItems(); i++) {
		delete _rects->GetItem(i);
	}
	delete _rects;
}

bool VRegion::Contains(VPoint point) const {
	for (int i=0;i<_rects->CountItems(); i++) {
		VRect *r = _rects->GetItem(i);
		if (r && r->Contains( point)) {
			return true;
		}
	}
	return false;
}

VRect VRegion::Frame(void) const {
	int32_t count = _rects->CountItems();
	float left = 0;
	float top = 0;
	float right = -1;
	float bottom = -1;
	if (count > 0) {
		VRect *p = _rects->GetItem(0);
		left = p->left;
		top = p->top;
		right = p->right;
		bottom = p->bottom;
		for (int i=1;i<count; i++) {
			// do calcs
			p = _rects->GetItem(i);
			left = min(left, p->left);
			top = min(top, p->top);
			right = max(right, p->right);
			bottom = max(bottom, p->bottom);
		}
	}
	VRect r(left, top, right, bottom);
	return r;
}

bool VRegion::Intersects(VRect rect) const {
	for (int i=0;i<_rects->CountItems(); i++) {
		VRect *r = _rects->GetItem(i);
		if (r && r->Intersects(rect)) return true;
	}
	return false;
}

void VRegion::MakeEmpty(void) {
	_rects->MakeEmpty();
}

void VRegion::OffsetBy(int32_t horizontal, int32_t vertical) {
	for (int i=0;i<_rects->CountItems(); i++) {
		VRect *r = _rects->GetItem(i);
		if (r) r->OffsetBy( (float)horizontal, (float)vertical);
	}
}

void VRegion::PrintToStream(void) const {
	cout << "VRegion:";
	for (int i=0;i<_rects->CountItems(); i++) {
		VRect *r = _rects->GetItem(i);
		if (r) {
			cout << " ";
			r->PrintToStream( );
		}
	}
}

VRect VRegion::RectAt(int32_t index) {
	VRect r;
	VRect *p = _rects->GetItem(index);
	if (p)
		return *p;
	return r;
}

int32_t VRegion::CountRects(void) {
	return _rects->CountItems();
}

void VRegion::Set(VRect rect) {
	MakeEmpty();
	_rects->AddItem(new VRect(rect));
}

void VRegion::Include(VRect rect) {
	if (!Intersects(rect)) {
		_rects->AddItem(new VRect(rect));
	} else {
		for (int i=0;i<_rects->CountItems(); i++) {
			VRect *r = _rects->GetItem(i);
			if (r) {
				if(r->Contains(rect)) {
					return;
				} else {
					// we need to break up the rect and only include the bits we need

				}
			}
		}
	}
}

void VRegion::Include(const VRegion *region) {
	for (int i=0;i<region->_rects->CountItems(); i++) {
		Include(*region->_rects->GetItem(i));
	}
}

void VRegion::Exclude(VRect rect) {
	 // no need to do anythin unless it intersects
	if (Intersects(rect)) {

	}
}

void VRegion::Exclude(const VRegion *region) {
	for (int i=0;i<region->_rects->CountItems(); i++) {
		Exclude(*region->_rects->GetItem(i));
	}
}

void VRegion::IntersectWith(const VRegion *region) {
}

VRegion& VRegion::operator=(const VRegion& region) {
	return *this;
}
