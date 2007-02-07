/***************************************************************************
 *            VStorageTest.cc
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

#include "VStorageTest.h"
#include <iostream>

using namespace std;

void VPathTest::setUp() {
}

void VPathTest::tearDown() {
}

inline void display_path(VPath &p) {
	cout << "'" << p.Path() << "' '" << p.Leaf() << "'\n";
}

void VPathTest::testCreate() {
	VPath p1;
	CPPUNIT_ASSERT(p1.InitCheck() == V_NO_INIT);
	VPath p("/home/clements/", ".bashrc");
	CPPUNIT_ASSERT(p.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp(".bashrc", p.Leaf()) == 0);
	CPPUNIT_ASSERT(strcmp("/home/clements", p.Path()) == 0);
	p.Unset();
	CPPUNIT_ASSERT(p.InitCheck() == V_NO_INIT);

	VPath p2("/home/clements/../foo/./../bar", ".bashrc");
	CPPUNIT_ASSERT(p2.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp(".bashrc", p2.Leaf()) == 0);
	CPPUNIT_ASSERT(strcmp("/home/bar", p2.Path()) == 0);
	p2.Unset();
	CPPUNIT_ASSERT(p2.InitCheck() == V_NO_INIT);

	VPath p3("/home/clements/../foo/./../bar/.bashrc");
	CPPUNIT_ASSERT(p3.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp(".bashrc", p3.Leaf()) == 0);
	CPPUNIT_ASSERT(strcmp("/home/bar", p3.Path()) == 0);
	VPath p4(p3);
	CPPUNIT_ASSERT(p4.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp(".bashrc", p4.Leaf()) == 0);
	CPPUNIT_ASSERT(strcmp("/home/bar", p4.Path()) == 0);
	VPath p5(".");
	CPPUNIT_ASSERT(p5.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("/home/clements/workspace/tekhne/trunk", p5.Path()) == 0);
	CPPUNIT_ASSERT(strcmp("test", p5.Leaf()) == 0);
	VPath p6("..");
	CPPUNIT_ASSERT(p6.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("/home/clements/workspace/tekhne", p6.Path()) == 0);
	CPPUNIT_ASSERT(strcmp("trunk", p6.Leaf()) == 0);

	// we needtests to check creation with Directory and Entry
}

void VPathTest::testAppend() {
//	status_t Append(const char *path, bool normalize = false);
	VPath p5(".");
	display_path(p5);
	CPPUNIT_ASSERT(p5.Append("src/VStorageTest.cc") == V_OK);
	CPPUNIT_ASSERT(p5.InitCheck() == V_OK);
	display_path(p5);
	CPPUNIT_ASSERT(strcmp("/home/clements/workspace/tekhne/trunk/test/src", p5.Path()) == 0);
	CPPUNIT_ASSERT(strcmp("VStorageTest.cc", p5.Leaf()) == 0);

	VPath p("/home/clements/");
	CPPUNIT_ASSERT(p.Append(".") == V_OK);
	CPPUNIT_ASSERT(p.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("clements", p.Leaf()) == 0);
	CPPUNIT_ASSERT(strcmp("/home", p.Path()) == 0);

	VPath p2("/home/clements/");
	CPPUNIT_ASSERT(p2.Append("..") == V_OK);
	CPPUNIT_ASSERT(p2.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("home", p2.Leaf()) == 0);
	CPPUNIT_ASSERT(strcmp("/", p2.Path()) == 0);

	VPath p3(".");
	CPPUNIT_ASSERT(p3.Append("../..") == V_OK);
	CPPUNIT_ASSERT(p3.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("tekhne", p3.Leaf()) == 0);
	CPPUNIT_ASSERT(strcmp("/home/clements/workspace", p5.Path()) == 0);
}

void VPathTest::testGetParent() {
//	status_t GetParent(VPath *path) const;
}

void VPathTest::testSetTo() {
//	status_t SetTo(const char *path, const char *leaf = 0, bool normalize = false);
//	status_t SetTo(const VDirectory *dir, const char *leaf = 0, bool normalize = false);
//	status_t SetTo(const VEntry *entry);
}

void VPathTest::testFlatten() {
//	virtual bool AllowsTypeCode(type_code code) const;

//	virtual status_t Flatten(void *buffer, ssize_t size) const;

//	virtual ssize_t FlattenedSize() const;

//	virtual type_code TypeCode() const;

//	virtual status_t Unflatten(type_code code, const void *buffer, ssize_t size);
//	virtual bool IsFixedSize(void) const { return false; }
}

void VPathTest::testAssignment() {
//	VPath& operator=(const VPath &path);
//	VPath& operator=(const char *string);
}

void VPathTest::testBoolean() {
//	bool operator==(const VPath &path) const;
//	bool operator==(const char *string) const;
//	bool operator!=(const VPath &path) const;
//	bool operator!=(const char *string) const;
}
