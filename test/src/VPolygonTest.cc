/***************************************************************************
 *            VPolygonTest.cc
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

#include "VPolygonTest.h"
#include <iostream>

using namespace std;

void VPolygonTest::setUp() {
}

void VPolygonTest::tearDown() {
}

void VPolygonTest::testCreate() {
	VPoint pts[] = {VPoint(9,8), VPoint(76,65), VPoint(-8,-4) };
	VPolygon *p = new VPolygon(pts, 3);
	p->PrintToStream();
	CPPUNIT_ASSERT( p->CountPoints() == 3 );
	VPolygon x;
	x = *p;
	delete p;
	CPPUNIT_ASSERT( x.CountPoints() == 3 );
}

void VPolygonTest::testFrame() {
	VPoint pts[] = {VPoint(9,8), VPoint(76,65), VPoint(-8,-4) };
	VPolygon x(pts, 3);
	VRect r = x.Frame();
	CPPUNIT_ASSERT( r.left == -8 );
	CPPUNIT_ASSERT( r.top == -4 );
	CPPUNIT_ASSERT( r.right == 76 );
	CPPUNIT_ASSERT( r.bottom == 65 );
	CPPUNIT_ASSERT( x.Contains(VPoint(8.9, 8)) );
	CPPUNIT_ASSERT( !x.Contains(VPoint(10, 100)) );
}

void VPolygonTest::testMapTo() {
	VPoint pts[] = {VPoint(9,8), VPoint(76,65), VPoint(-8,-4) };
	VPolygon x(pts, 3);
	VRect dest(0, 0, 30, 20);
	VRect r = x.Frame();
	x.MapTo(r, dest);
	r = x.Frame();
	CPPUNIT_ASSERT( r.left == 0 );
	CPPUNIT_ASSERT( r.top == 0 );
	CPPUNIT_ASSERT( r.right == 30 );
	CPPUNIT_ASSERT( r.bottom == 20 );
}

void VRegionTest::setUp() {
}

void VRegionTest::tearDown() {
}

void VRegionTest::testCreate() {
	VRect r(-1, -6, 5, 3);
	VRegion *rgn = new VRegion(r);
	rgn->PrintToStream();
	CPPUNIT_ASSERT( rgn->Contains(VPoint(1, 1)) );
	CPPUNIT_ASSERT( !rgn->Contains(VPoint(10, 13)) );
	rgn->OffsetBy(3, 5);
	rgn->PrintToStream();
	CPPUNIT_ASSERT( rgn->CountRects() == 1 );
	rgn->MakeEmpty();
	CPPUNIT_ASSERT( rgn->CountRects() == 0 );
	delete rgn;
}

void VShapeTest::setUp() {
}

void VShapeTest::tearDown() {
}

void VShapeTest::testCreate() {
	VShape s;
	s.MoveTo(VPoint(-1, -1));
	s.LineTo(VPoint(1, -1));
	s.LineTo(VPoint(1, 1));
	s.Close();
	VRect r = s.Bounds();
	CPPUNIT_ASSERT( r.left == -1 );
	CPPUNIT_ASSERT( r.top == -1 );
	CPPUNIT_ASSERT( r.right == 1 );
	CPPUNIT_ASSERT( r.bottom == 1 );

	VMessage msg;
	s.Archive(&msg);
	VShape *s2 = dynamic_cast<VShape*>(VShape::Instantiate(&msg));
	CPPUNIT_ASSERT( s2 );
	CPPUNIT_ASSERT( r.left == -1 );
	r = s2->Bounds();
	CPPUNIT_ASSERT( r.left == -1 );
	CPPUNIT_ASSERT( r.top == -1 );
	CPPUNIT_ASSERT( r.right == 1 );
	CPPUNIT_ASSERT( r.bottom == 1 );
}

class TestShapeIterator : public VShapeIterator {
private:
	int32_t moves;
	int32_t lines;
	int32_t closes;
public:
	TestShapeIterator() : moves(0), lines(0), closes(0) {}
	virtual status_t IterateClose(void) { closes++; return V_OK; }
	virtual status_t IterateLineTo(int32_t lineCount, VPoint * linePoints) { lines += lineCount; return V_OK; }
	virtual status_t IterateMoveTo(VPoint * point) { moves++; return V_OK; }

	int32_t getMoves() { return moves; }
	int32_t getLines() { return lines; }
	int32_t getCloses() { return closes; }
};

void VShapeTest::testIterator() {
	VShape s;
	s.MoveTo(VPoint(-1, -1));
	s.LineTo(VPoint(1, -1));
	s.LineTo(VPoint(1, 1));
	s.Close();

	TestShapeIterator tsi;
	tsi.Iterate(&s);

	CPPUNIT_ASSERT( tsi.getMoves() == 1 );
	CPPUNIT_ASSERT( tsi.getLines() == 2 );
	CPPUNIT_ASSERT( tsi.getCloses() == 1 );
}
