/***************************************************************************
 *            VRect.cc
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

using namespace tekhne;

bool VRect::Contains(VPoint point) const {
	return point.x >= left && point.x <= right &&
		point.y <= bottom && point.y >= top;
}

bool VRect::Contains(VRect rect) const {
	if (!rect.IsValid()) return false;
	return top <= rect.top && bottom >= rect.bottom &&
		right >= rect.right && left <= rect.left;
}

bool VRect::Intersects(VRect rect) const {
	// TODO: finish this one
	return false;
}

void VRect::InsetBy(float x, float y) {
	left += x;
	right -= x;
	top += y;
	bottom -= y;
}

void VRect::InsetBy(VPoint point) {
	InsetBy(point.x, point.y);
}

VRect &VRect::InsetBySelf(float x, float y) {
	InsetBy(x, y);
	return *this;
}

VRect &VRect::InsetBySelf(VPoint point) {
	InsetBy(point.x, point.y);
	return *this;
}

VRect VRect::InsetByCopy(float x, float y) {
	VRect r(*this);
	r.InsetBy(x, y);
	return r;
}

VRect VRect::InsetByCopy(VPoint point) {
	VRect r(*this);
	r.InsetBy(point);
	return r;
}

void VRect::OffsetBy(float x, float y) {
	left += x;
	right += x;
	top += y;
	bottom += y;
}

void VRect::OffsetBy(VPoint point) {
	OffsetBy(point.x, point.y);
}

VRect &VRect::OffsetBySelf(float x, float y) {
	OffsetBy(x, y);
	return *this;
}

VRect &VRect::OffsetBySelf(VPoint point) {
	OffsetBy(point.x, point.y);
	return *this;
}

VRect VRect::OffsetByCopy(float x, float y) {
	VRect r(*this);
	r.OffsetBy(x, y);
	return r;
}

VRect VRect::OffsetByCopy(VPoint point) {
	VRect r(*this);
	r.OffsetBy(point);
	return r;
}

void VRect::OffsetTo(float x, float y) {
	OffsetBy(x - left, y - top);
}

void VRect::OffsetTo(VPoint point) {
	OffsetTo(point.x, point.y);
}

VRect &VRect::OffsetToSelf(float x, float y) {
	OffsetTo(x, y);
	return *this;
}

VRect &VRect::OffsetToSelf(VPoint point) {
	OffsetTo(point);
	return *this;
}

VRect VRect::OffsetToCopy(float x, float y) {
	VRect r(*this);
	r.OffsetTo(x, y);
	return r;
}

VRect VRect::OffsetToCopy(VPoint point) {
	VRect r(*this);
	r.OffsetTo(point);
	return r;
}

void VRect::PrintToStream(void) const {
	std::cout << "VRect("<<left<<", "<<top<<", "<<right<<", "<<bottom<<")";
}

void VRect::SetLeftTop(const VPoint point) {
	left = point.x;
	top = point.y;
}

void VRect::SetLeftBottom(const VPoint point) {
	left = point.x;
	bottom = point.y;
}

void VRect::SetRightTop(const VPoint point) {
	right = point.x;
	top = point.y;
}

void VRect::SetRightBottom(const VPoint point) {
	right = point.x;
	bottom = point.y;
}

VPoint VRect::LeftTop(void) const {
	VPoint p(left, top);
	return p;
}

VPoint VRect::LeftBottom(void) const {
	VPoint p(left, bottom);
	return p;
}

VPoint VRect::RightTop(void) const {
	VPoint p(right, top);
	return p;
}

VPoint VRect::RightBottom(void) const {
	VPoint p(right, bottom);
	return p;
}

bool VRect::operator ==(VRect r) const {
	return top == r.top && bottom == r.bottom &&
		left == r.left && right == r.right;
}

bool VRect::operator !=(VRect r) const {
	return top != r.top || bottom != r.bottom ||
		left != r.left || right != r.right;
}

static inline float min(float x, float y) { return x < y ? x : y; }
static inline float max(float x, float y) { return x > y ? x : y; }

VRect VRect::operator &(VRect r) const {
	// TODO: one of these two is WRONG!
	return VRect(min(left,r.left), min(top, r.top), max(right, r.right), max(bottom, r.bottom));
}

VRect VRect::operator |(VRect r) const {
	// TODO: one of these two is WRONG!
	return VRect(min(left,r.left), min(top, r.top), max(right, r.right), max(bottom, r.bottom));
}
