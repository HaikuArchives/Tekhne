/***************************************************************************
 *            VPointTest.cc
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

#include "VPointTest.h"
#include <iostream>

using namespace std;

static inline bool cf(float x, float y) {
	float z = x-y;
	if (z < 0) z = -z;
	return z < 0.0001;
}

static inline bool checkPtValue(VPoint p, float x, float y) {
	return cf(p.x,x) && cf(p.y,y);
}

static inline bool checkRectValue(VRect r, float left, float top, float right, float bottom) {
	return cf(r.left,left) && cf(r.top,top) && cf(r.right,right) && cf(r.bottom,bottom);
}

/* ---- Point tests ---- */
void VPointTest::setUp() {
	p = new VPoint(5.0, 64.0);
}

void VPointTest::tearDown() {
	delete p;
}

void VPointTest::testCreate() {
	CPPUNIT_ASSERT(checkPtValue(*p, 5.0, 64.0));
	VPoint x(-6, 8);
	CPPUNIT_ASSERT(checkPtValue(x,-6.0,8.0));
	VPoint z;
	CPPUNIT_ASSERT(checkPtValue(z,0.0,0.0));
	z.Set(9.0, 5);
	z.PrintToStream();
	CPPUNIT_ASSERT(checkPtValue(z,9.0,5));
	VPoint a = z;
	CPPUNIT_ASSERT(checkPtValue(a,9.0,5.0));
	CPPUNIT_ASSERT(a == z);
	CPPUNIT_ASSERT(a != x);
	VPoint y = a + z;
	CPPUNIT_ASSERT(checkPtValue(y,18.0,10.0));
	a += x;
	CPPUNIT_ASSERT(checkPtValue(a,3.0,13.0));
	a -= x;
	CPPUNIT_ASSERT(checkPtValue(a,9.0,5.0));
	VPoint w = a - z;
	CPPUNIT_ASSERT(checkPtValue(w,0.0,0.0));
}

void VPointTest::testConstrainTo() {

	CPPUNIT_ASSERT(VRect(0, 0, 10, 10).IsValid());
	VPoint x(9, 7);
	x.ConstrainTo(VRect(0, 0, 10, 10));
	CPPUNIT_ASSERT(checkPtValue(x,9.0,7.0));

	VPoint p1(94, 75);
	p1.ConstrainTo(VRect(0, 0, 10, 10));
	CPPUNIT_ASSERT(checkPtValue(p1,10.0,10.0));
	VPoint p2(-9, 57);
	p2.ConstrainTo(VRect(0, 0, 10, 10));
	CPPUNIT_ASSERT(checkPtValue(p2,0.0,10.0));
	VPoint p3(9, -7);
	p3.ConstrainTo(VRect(0, 0, 10, 10));
	CPPUNIT_ASSERT(checkPtValue(p3,9.0,0.0));
	VPoint p4(-9, -7);
	p4.ConstrainTo(VRect(0, 0, 10, 10));
	CPPUNIT_ASSERT(checkPtValue(p4,0.0,0.0));
}

/* ---- Rect tests ---- */
void VRectTest::setUp() {
	r = new VRect(5, 64, 8, 93);
}

void VRectTest::tearDown() {
	delete r;
}

void VRectTest::testCreate() {
	CPPUNIT_ASSERT (r->IsValid());
	CPPUNIT_ASSERT(checkRectValue(*r, 5, 64, 8, 93));

	VRect x(5, 64, -8, 93);
	CPPUNIT_ASSERT(checkRectValue(x, 5, 64, -8, 93));
	CPPUNIT_ASSERT (!x.IsValid());

	VPoint p1(-6.2, 8);
	VPoint p2(9, 18.6);
	VRect y(p1, p2);
	CPPUNIT_ASSERT(checkRectValue(y, -6.2, 8.0, 9.0, 18.6));

	VRect z;
	CPPUNIT_ASSERT(checkRectValue(z, 0.0, 0.0, 0.0, 0.0));

	CPPUNIT_ASSERT(cf(y.Width(),15.2));
	CPPUNIT_ASSERT(y.IntegerWidth() == 16);
	CPPUNIT_ASSERT(cf(y.Height(),10.6));
	CPPUNIT_ASSERT(y.IntegerHeight() == 11);

	VRect a(0, 0, 12, 15);
	a.InsetBy(2, 3);
	CPPUNIT_ASSERT(checkRectValue(a, 2, 3, 10, 12));
	a.InsetBy(VPoint (1, 2));
	CPPUNIT_ASSERT(checkRectValue(a, 3, 5, 9, 10));
	a.PrintToStream();

	CPPUNIT_ASSERT(checkRectValue(VRect(0, 0, 12, 15).InsetBySelf(2, 3), 2, 3, 10, 12));
	CPPUNIT_ASSERT(checkRectValue(VRect(0, 0, 12, 15).InsetBySelf(VPoint(2, 3)), 2, 3, 10, 12));

	a.Set(0, 0, 12, 15);
	VRect b = a.InsetByCopy(2, 3);
	CPPUNIT_ASSERT(a != b);
	CPPUNIT_ASSERT(checkRectValue(b, 2, 3, 10, 12));
	b = a.InsetByCopy(VPoint(2, 4));
	CPPUNIT_ASSERT(a != b);
	CPPUNIT_ASSERT(checkRectValue(b, 2, 4, 10, 11));

	a.OffsetBy(2, 3);
	CPPUNIT_ASSERT(checkRectValue(a, 2, 3, 14, 18));
	a.Set(0, 0, 12, 15);
	a.OffsetBy(VPoint(2, 3));
	CPPUNIT_ASSERT(checkRectValue(a, 2, 3, 14, 18));

	CPPUNIT_ASSERT(checkRectValue(VRect(0, 0, 12, 15).OffsetBySelf(2, 3), 2, 3, 14, 18));
	CPPUNIT_ASSERT(checkRectValue(VRect(0, 0, 12, 15).OffsetBySelf(VPoint(2, 3)), 2, 3, 14, 18));

	a.Set(0, 0, 12, 15);
	b = a.OffsetByCopy(2, -3);
	CPPUNIT_ASSERT(a != b);
	CPPUNIT_ASSERT(checkRectValue(b, 2, -3, 14, 12));

	b = a.OffsetByCopy(VPoint(2, 4));
	CPPUNIT_ASSERT(a != b);
	CPPUNIT_ASSERT(checkRectValue(b, 2, 4, 14, 19));

	a.Set(4, 5, 34, 76);
	a.OffsetTo(2, 3);
	CPPUNIT_ASSERT(checkRectValue(a, 2, 3, 32, 74));
	a.Set(4, 5, 34, 76);
	a.OffsetTo(VPoint(2, 3));
	CPPUNIT_ASSERT(checkRectValue(a, 2, 3, 32, 74));

	CPPUNIT_ASSERT(checkRectValue(VRect(4, 5, 34, 76).OffsetToSelf(2, 3), 2, 3, 32, 74));
	CPPUNIT_ASSERT(checkRectValue(VRect(4, 5, 34, 76).OffsetToSelf(VPoint(2, 3)), 2, 3, 32, 74));

	a.Set(4, 5, 34, 76);
	b = a.OffsetToCopy(2, 3);
	CPPUNIT_ASSERT(a != b);
	CPPUNIT_ASSERT(checkRectValue(b, 2, 3, 32, 74));
	b = a.OffsetToCopy(VPoint(2, 3));
	CPPUNIT_ASSERT(a != b);
	CPPUNIT_ASSERT(checkRectValue(b, 2, 3, 32, 74));
}

void VRectTest::testContains() {
	VRect x(-1, -2, 9, 11);
	CPPUNIT_ASSERT(x.Contains(VPoint(0, 0)));
	CPPUNIT_ASSERT(!x.Contains(VPoint(-4, 0)));
	CPPUNIT_ASSERT(!x.Contains(VPoint(0, 43)));

	CPPUNIT_ASSERT(x.Contains(VRect(0, 0, 8, 8)));
	CPPUNIT_ASSERT(!x.Contains(VRect(-4, 0, 8, 8)));
	CPPUNIT_ASSERT(!x.Contains(VRect(0, -4, 8, 8)));
	CPPUNIT_ASSERT(!x.Contains(VRect(-5, 0, -4, 8)));
	CPPUNIT_ASSERT(!x.Contains(VRect(0, -5, 8, -4)));

	CPPUNIT_ASSERT(x.Intersects(VRect(0, 0, 34, 45)));
	CPPUNIT_ASSERT(x.Intersects(VRect(-5, 0, 0, 45)));
	CPPUNIT_ASSERT(x.Intersects(VRect(0, -3, 8, 18)));
	CPPUNIT_ASSERT(x.Intersects(VRect(-5, -4, 2, 2)));
	CPPUNIT_ASSERT(!x.Intersects(VRect(10, 12, 34, 56)));
	// bool Intersects(VRect rect) const;
}

void VRectTest::testSetPoint() {
	VRect a(4, 5, 34, 76);
	a.SetLeftTop(VPoint(-1, -23));
	CPPUNIT_ASSERT(checkRectValue(a, -1, -23, 34, 76));

	a.Set(4, 5, 34, 76);
	a.SetLeftBottom(VPoint(-1, -23));
	CPPUNIT_ASSERT(checkRectValue(a, -1, 5, 34, -23));

	a.Set(4, 5, 34, 76);
	a.SetRightTop(VPoint(-1, -23));
	CPPUNIT_ASSERT(checkRectValue(a, 4, -23, -1, 76));

	a.Set(4, 5, 34, 76);
	a.SetRightBottom(VPoint(-1, -23));
	CPPUNIT_ASSERT(checkRectValue(a, 4, 5, -1, -23));

	CPPUNIT_ASSERT(checkPtValue(a.LeftTop(),4,5));
	CPPUNIT_ASSERT(checkPtValue(a.LeftBottom(),4,-23));
	CPPUNIT_ASSERT(checkPtValue(a.RightTop(),-1,5));
	CPPUNIT_ASSERT(checkPtValue(a.RightBottom(),-1,-23));
}

void VRectTest::testOperators() {
	VRect a(4, 5, 34, 76);
	VRect b(-2, 3, 18, 16);
	VRect c(4, 5, 34, 76);

	CPPUNIT_ASSERT(a == c);
	CPPUNIT_ASSERT(a != b);

	CPPUNIT_ASSERT(checkRectValue(a&b, -2, 3, 34, 76));
	CPPUNIT_ASSERT(checkRectValue(a|b, -2, 3, 34, 76));
}
