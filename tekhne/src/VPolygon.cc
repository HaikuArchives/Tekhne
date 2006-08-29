/***************************************************************************
 *            VPolygon.cc
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

VPolygon::VPolygon(const VPoint *pointList, int32_t numPoints) {
	_points = new VTList<VPoint*>();
	AddPoints(pointList, numPoints);
}

VPolygon::VPolygon(const VPolygon *polygon) {
	_points = new VTList<VPoint*>();
	for (int i=0;i<polygon->CountPoints(); i++) {
		_points->AddItem(new VPoint(*polygon->_points->GetItem(i)));
	}
}

VPolygon::VPolygon() {
	_points = new VTList<VPoint*>();
}

VPolygon::~VPolygon() {
	for (int i=0;i<_points->CountItems(); i++) {
		delete _points->GetItem(i);
	}
	delete _points;
}

void VPolygon::AddPoints(const VPoint *pointList, int32_t numPoints) {
	for(int i=0;i<numPoints;i++) {
		_points->AddItem(new VPoint(pointList[i]));
	}
}

int32_t VPolygon::CountPoints(void) const {
	return _points->CountItems();
}

VRect VPolygon::Frame(void) const {
	int32_t count = _points->CountItems();
	float left = 0;
	float top = 0;
	float right = 0;
	float bottom = 0;
	if (count > 0) {
		VPoint *p = _points->GetItem(0);
		left = p->x;
		top = p->y;
		right = p->x;
		bottom = p->y;
		for (int i=1;i<count; i++) {
			// do calcs
			VPoint *p = _points->GetItem(i);
			left = min(left, p->x);
			top = min(top, p->y);
			right = max(right, p->x);
			bottom = max(bottom, p->y);
		}
	}
	VRect r(left, top, right, bottom);
	return r;
}

void VPolygon::MapTo(VRect source, VRect destination) {
	float x_trans = destination.left - source.left; //(destination.left + destination.Width()/2) - (source.left + source.Width()/2);
	float y_trans = destination.top - source.top; // (destination.top + destination.Height()/2) - (source.top + source.Height()/2);
	float x_scale = destination.Width()/source.Width();
	float y_scale = destination.Height()/source.Height();

	for (int i=0;i<_points->CountItems();i++) {
		VPoint *p = _points->GetItem(i);
		p->x = (p->x + x_trans)*x_scale ;
		p->y = (p->y + y_trans)*y_scale;
	}
}

void VPolygon::PrintToStream(void) const {
	cout << "VPolygon:\n";
	for (int i=0;i<_points->CountItems(); i++) {
		_points->GetItem(i)->PrintToStream();
		cout << endl;
	}
}

VPolygon& VPolygon::operator=(const VPolygon& p) {
	_points->MakeEmpty();
	for (int i=0;i<p.CountPoints(); i++) {
		_points->AddItem(new VPoint(*p._points->GetItem(i)));
	}
	return *this;
}

bool VPolygon::Contains(VPoint p) {
	VPoint **pt = _points->Items();
	int32_t npol = _points->CountItems();
	bool c = false;
	int i, j;
	for (i = 0, j = npol-1; i < npol; j = i++) {
		if ((((pt[i]->y<=p.y) && (p.y<pt[j]->y)) ||
					 ((pt[j]->y<=p.y) && (p.y<pt[i]->y))) &&
					 (p.x < (pt[j]->x - pt[i]->x) * (p.y - pt[i]->y) / (pt[j]->y - pt[i]->y) + pt[i]->x))
			c = !c;
	}
	return c;
}
