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
const int32_t V_CLOSE_OP = 3;

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
		if (op != V_CLOSE_OP) {
			archive->FindPoint("pts", pt_idx++, &pt);
			if (op == V_MOVE_TO_OP) {
				MoveTo(pt);
			} else {
				LineTo(pt);
			}
		} else {
			Close();
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
				if (pt->_type != V_CLOSE_OP) {
					new_pt->_pt = pt->_pt;
				}
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
			switch (pt->_type) {
				case V_LINE_TO_OP:
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
					break;
				case V_MOVE_TO_OP:
					start = false;
					last_pt = pt->_pt;
				case V_CLOSE_OP:
					shape_point *pt = static_cast<shape_point*>(_pts.FirstItem());
					left = min(left, pt->_pt.x);
					top = min(top, pt->_pt.y);
					right = max(right, pt->_pt.x);
					bottom = max(bottom, pt->_pt.y);
					break;
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
	struct shape_point *pt = new struct shape_point();
	pt->_type = V_CLOSE_OP;
	_pts.AddItem(pt);
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
			if (pt->_type != V_CLOSE_OP) {
				archive->AddPoint("pts", pt->_pt);
			}
		}
	}
	return V_OK;
}

		// implemented in VShape.cc
status_t VShapeIterator::Iterate(VShape *shape) {
	VListIterator iter(shape->_pts);
	VList lines;

	while(iter.HasNext()) {
		void *p = iter.Next();
		if (p) {
			shape_point *pt = static_cast<shape_point*>(p);
			switch(pt->_type) {
				case V_MOVE_TO_OP:
					if (lines.CountItems() > 0) {
						IterateLineTo(lines.CountItems(), (VPoint *)lines.Items());
						lines.MakeEmpty();
					}
					IterateMoveTo(&pt->_pt);
					break;
				case V_LINE_TO_OP:
					lines.AddItem(&pt->_pt);
					break;
				case V_CLOSE_OP:
					if (lines.CountItems() > 0) {
						IterateLineTo(lines.CountItems(), (VPoint *)lines.Items());
						lines.MakeEmpty();
					}
					IterateClose();
					break;
			}
		}
	}
	// if we are all finished and we have any lines left...
	if (lines.CountItems() > 0) {
		IterateLineTo(lines.CountItems(), (VPoint *)lines.Items());
	}
	return V_OK;
}
