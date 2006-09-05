/***************************************************************************
 *            VShape.cc
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
#include "values.h"

using namespace tekhne;
using namespace std;

namespace tekhne {

const int32_t V_MOVE_TO_OP = 1;
const int32_t V_LINE_TO_OP = 2;

struct shape_point {
	int32_t _type;
	VPoint _pt;
};

}

VShape::VShape() {
}

VShape::VShape(const VShape &copyFrom) {
	AddShape(&copyFrom);
}

VShape::VShape(VMessage *archive) {
	int32_t pt_idx = 0;
	int32_t op_idx = 0;
	int32_t op;

	while (archive->FindInt32("ops", op_idx++, &op) == V_OK) {
		VPoint pt;
		archive->FindPoint("pts", pt_idx++, &pt);
		if (op == V_MOVE_TO_OP) {
			MoveTo(pt);
		} else {
			LineTo(pt);
		}
	}
}

VShape::~VShape() {
	VListIterator iter(_pts);
	while(iter.HasNext()) {
		delete static_cast<shape_point*>(iter.Next());
	}
}

status_t VShape::AddShape(const VShape *otherShape) {
	if (otherShape) {
		VListIterator iter(otherShape->_pts);
		while(iter.HasNext()) {
			void *p = iter.Next();
			if (p) {
				shape_point *pt = static_cast<shape_point*>(p);
				struct shape_point *new_pt = new struct shape_point();
				new_pt->_type = pt->_type;
				new_pt->_pt = pt->_pt;
				_pts.AddItem(new_pt);
			}
		}
	}
	return V_OK;
}

VRect VShape::Bounds(void) const {
	VRect r;
	float left = MAXFLOAT;
	float top  = MAXFLOAT;
	float right = MINFLOAT;
	float bottom = MINFLOAT;
	VPoint last_pt;
	bool start = false;

	VListIterator iter(_pts);
	while(iter.HasNext()) {
		void *p = iter.Next();
		if (p) {
			shape_point *pt = static_cast<shape_point*>(p);
			if (pt->_type == V_LINE_TO_OP) {
				if (!start) {
					left = min(left, last_pt.x);
					top = min(top, last_pt.y);
					right = max(right, last_pt.x);
					bottom = max(bottom, last_pt.y);
					start = true;
				}
				left = min(left, pt->_pt.x);
				top = min(top, pt->_pt.y);
				right = max(right, pt->_pt.x);
				bottom = max(bottom, pt->_pt.y);
				last_pt = pt->_pt;
			} else {
				start = false;
				last_pt = pt->_pt;
			}
		}
	}
	r.Set(left, top, right, bottom);
	return r;
}

void VShape::Clear(void) {
	_pts.MakeEmpty();
}

void VShape::Close(void) {
	// add last line segment if needed
	if (_pts.CountItems() > 1) {
		shape_point *start_pt = static_cast<shape_point*>(_pts.FirstItem());
		shape_point *end_pt = static_cast<shape_point*>(_pts.FirstItem());
		if (end_pt->_pt != start_pt->_pt) {
			LineTo(start_pt->_pt);
		}
	}
}

status_t VShape::LineTo(VPoint point) {
	struct shape_point *pt = new struct shape_point();
	pt->_type = V_LINE_TO_OP;
	pt->_pt = point;
	_pts.AddItem(pt);
	return V_OK;
}

status_t VShape::MoveTo(VPoint point) {
	struct shape_point *pt = new struct shape_point();
	pt->_type = V_MOVE_TO_OP;
	pt->_pt = point;
	_pts.AddItem(pt);
	return V_OK;
}

/* "pts" 	B_POINT_TYPE 	The list of points used by the shape's commands.
   "ops" 	B_INT32_TYPE */
VArchivable *VShape::Instantiate(VMessage *archive) {
	return new VShape(archive);
}

status_t VShape::Archive(VMessage *archive, bool deep) const {
	VListIterator iter(_pts);
	while(iter.HasNext()) {
		void *p = iter.Next();
		if (p) {
			shape_point *pt = static_cast<shape_point*>(p);
			archive->AddInt32("ops", pt->_type);
			archive->AddPoint("pts", pt->_pt);
		}
	}
	return V_OK;
}
