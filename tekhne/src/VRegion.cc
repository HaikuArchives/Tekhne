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
	return false;
}

VRect VRegion::Frame(void) const {
	VRect r;
	return r;
}

bool VRegion::Intersects(VRect rect) const {
	return false;
}

void VRegion::MakeEmpty(void) {
}

void VRegion::OffsetBy(int32_t horizontal, int32_t vertical) {
}

void VRegion::PrintToStream(void) const {
}

VRect VRegion::RectAt(int32_t index) {
	VRect r;
	return r;
}

int32_t VRegion::CountRects(void) {
	return 0;
}

void VRegion::Set(VRect rect) {
}

void VRegion::Include(VRect rect) {
}

void VRegion::Include(const VRegion *region) {
}

void VRegion::Exclude(VRect rect) {
}

void VRegion::Exclude(const VRegion *region) {
}

void VRegion::IntersectWith(const VRegion *region) {
}

VRegion& VRegion::operator=(const VRegion& region) {
	return *this;
}
