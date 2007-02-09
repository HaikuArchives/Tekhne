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

static char *cwd = 0;

char *get_cwd() {
	if (!cwd) {
		cwd = (char*)malloc(V_PATH_NAME_LENGTH);
		getcwd(cwd, V_PATH_NAME_LENGTH);
	}
	return cwd;
}

void VPathTest::setUp() {
}

void VPathTest::tearDown() {
}

inline void display_path(VPath &p) {
	if (p.InitCheck() == V_OK) cout << "'" << p.Path() << "' '" << p.Leaf() << "'\n";
	else cout << "Path is not initialized" << endl;
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

	VPath p7("/");
	CPPUNIT_ASSERT(p7.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("/", p7.Path()) == 0);
	CPPUNIT_ASSERT(strcmp("", p7.Leaf()) == 0);

	// we needtests to check creation with Directory and Entry
	// VPath(const VDirectory *dir, const char *leaf = 0, bool normalize = false);
	// VPath(const VEntry *entry);
}

void VPathTest::testAppend() {
//	status_t Append(const char *path, bool normalize = false);
	VPath p5(".");
	CPPUNIT_ASSERT(p5.Append("src/VStorageTest.cc") == V_OK);
	CPPUNIT_ASSERT(p5.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("/home/clements/workspace/tekhne/trunk/test/src", p5.Path()) == 0);
	CPPUNIT_ASSERT(strcmp("VStorageTest.cc", p5.Leaf()) == 0);

	VPath p2("/home/clements/");
	CPPUNIT_ASSERT(p2.Append("..") == V_OK);
	CPPUNIT_ASSERT(p2.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("home", p2.Leaf()) == 0);
	CPPUNIT_ASSERT(strcmp("/", p2.Path()) == 0);

	VPath p3(".");
	CPPUNIT_ASSERT(p3.Append("../..") == V_OK);
	CPPUNIT_ASSERT(p3.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("tekhne", p3.Leaf()) == 0);
	CPPUNIT_ASSERT(strcmp("/home/clements/workspace", p3.Path()) == 0);

	VPath p("/home/clements/");
	CPPUNIT_ASSERT(p.Append(".") == V_OK);
	CPPUNIT_ASSERT(p.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("clements", p.Leaf()) == 0);
	CPPUNIT_ASSERT(strcmp("/home", p.Path()) == 0);
}

void VPathTest::testGetParent() {
//	status_t GetParent(VPath *path) const;
	VPath ans;
	VPath p("/home/clements/");
	CPPUNIT_ASSERT(p.GetParent(&ans) == V_OK);
	CPPUNIT_ASSERT(ans.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("home", ans.Leaf()) == 0);
	CPPUNIT_ASSERT(strcmp("/", ans.Path()) == 0);
	p.SetTo("/");
	CPPUNIT_ASSERT(p.GetParent(&ans) == V_BAD_VALUE);
	CPPUNIT_ASSERT(ans.InitCheck() == V_NO_INIT);
	p.SetTo("/");
	CPPUNIT_ASSERT(p.GetParent(&ans) == V_BAD_VALUE);
	CPPUNIT_ASSERT(ans.InitCheck() == V_NO_INIT);
}

void VPathTest::testSetTo() {
	VPath p;
//	status_t SetTo(const char *path, const char *leaf = 0, bool normalize = false);
	CPPUNIT_ASSERT(p.SetTo("/") == V_OK);
	CPPUNIT_ASSERT(p.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("/", p.Path()) == 0);
	CPPUNIT_ASSERT(strcmp("", p.Leaf()) == 0);
	CPPUNIT_ASSERT(p.SetTo("/", "foo") == V_OK);
	CPPUNIT_ASSERT(p.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("/", p.Path()) == 0);
	CPPUNIT_ASSERT(strcmp("foo", p.Leaf()) == 0);
	CPPUNIT_ASSERT(p.SetTo(".", "foo") == V_OK);
	CPPUNIT_ASSERT(p.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("/home/clements/workspace/tekhne/trunk/test", p.Path()) == 0);
	CPPUNIT_ASSERT(strcmp("foo", p.Leaf()) == 0);
	CPPUNIT_ASSERT(p.SetTo(".") == V_OK);
	CPPUNIT_ASSERT(p.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("/home/clements/workspace/tekhne/trunk", p.Path()) == 0);
	CPPUNIT_ASSERT(strcmp("test", p.Leaf()) == 0);
	CPPUNIT_ASSERT(p.SetTo("/home", "foo") == V_OK);
	CPPUNIT_ASSERT(p.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("/home", p.Path()) == 0);
	CPPUNIT_ASSERT(strcmp("foo", p.Leaf()) == 0);
	CPPUNIT_ASSERT(p.SetTo("/home/bar", "foo/bar/baz.tar.gz") == V_OK);
	CPPUNIT_ASSERT(p.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("/home/bar/foo/bar", p.Path()) == 0);
	CPPUNIT_ASSERT(strcmp("baz.tar.gz", p.Leaf()) == 0);
	// some bad values
	CPPUNIT_ASSERT(p.SetTo("/home", "./../../foo") == V_BAD_VALUE);
	CPPUNIT_ASSERT(p.InitCheck() == V_NO_INIT);
	CPPUNIT_ASSERT(p.SetTo("/home", "/foo") == V_BAD_VALUE);
	CPPUNIT_ASSERT(p.InitCheck() == V_NO_INIT);
//	status_t SetTo(const VDirectory *dir, const char *leaf = 0, bool normalize = false);
//	status_t SetTo(const VEntry *entry);
}

void VPathTest::testFlatten() {
	VPath p(".");
//	virtual bool AllowsTypeCode(type_code code) const;
	CPPUNIT_ASSERT(p.AllowsTypeCode(V_STRING_TYPE));
//	virtual ssize_t FlattenedSize() const;
	ssize_t len = p.FlattenedSize();
	CPPUNIT_ASSERT(len == (ssize_t)strlen(get_cwd())+1);
//	virtual status_t Flatten(void *buffer, ssize_t size) const;
	char *buf = (char *)malloc(len);
	CPPUNIT_ASSERT(p.Flatten(buf, len) == V_OK);
//	virtual type_code TypeCode() const;
	CPPUNIT_ASSERT(p.TypeCode() == V_STRING_TYPE);
//	virtual status_t Unflatten(type_code code, const void *buffer, ssize_t size);
	VPath q;
	CPPUNIT_ASSERT(q.Unflatten(V_STRING_TYPE, buf, len) == V_OK);
	CPPUNIT_ASSERT(q.InitCheck() == V_OK);
	CPPUNIT_ASSERT(q == p);
//	virtual bool IsFixedSize(void) const { return false; }
	CPPUNIT_ASSERT(!p.IsFixedSize());
}

void VPathTest::testAssignment() {
//	VPath& operator=(const VPath &path);
	VPath p("/home/clements");
	VPath q;
	q = p;
	CPPUNIT_ASSERT(p == q);
	CPPUNIT_ASSERT(!(p != q));
	q.Unset();
//	VPath& operator=(const char *string);
	q = "/home/clements";
	CPPUNIT_ASSERT(p == q);
	CPPUNIT_ASSERT(!(p != q));
	q.Unset();
	q = "/home/foo";
	CPPUNIT_ASSERT(p != q);
	CPPUNIT_ASSERT(!(p == q));
	q.Unset();
}

void VPathTest::testBoolean() {
	VPath p("/home/clements");
	VPath q("/home/clements");
	CPPUNIT_ASSERT(p == q);
	CPPUNIT_ASSERT(!(p != q));
	p.SetTo(".","foo");
	q.SetTo(".","bar");
	CPPUNIT_ASSERT(p != q);
	CPPUNIT_ASSERT(!(p == q));
	p.SetTo(".","../foo");
	q.SetTo(".","./foo");
	CPPUNIT_ASSERT(p != q);
	CPPUNIT_ASSERT(!(p == q));
	p.SetTo(".","./foo");
	q.SetTo(".","./foo");
	CPPUNIT_ASSERT(p == q);
	CPPUNIT_ASSERT(!(p != q));
	p.SetTo("/home/clements");
	q.SetTo("/home/clementsd");
	CPPUNIT_ASSERT(p != q);
	CPPUNIT_ASSERT(!(p == q));
}

void VEntryTest::setUp() {
	const char *s = "This is a sample string for the test file.\n";
	int32_t len = strlen(s);
	int32_t f = open("/tmp/test_file", O_CREAT|O_TRUNC|O_RDWR, 0600);
	for (int i=0;i<35;i++) {
		write(f, s, len);
	}
	close(f);
	symlink ("/tmp/test_file", "/tmp/test_file_sym");
	close(open("/tmp/test_file2", O_RDWR | O_CREAT | O_TRUNC, 0644));
}

void VEntryTest::tearDown() {
	VEntry e1("/tmp/test_file_sym");
	e1.Remove();
	VEntry e2("/tmp/test_file");
	e2.Remove();
	VEntry e3("/tmp/test_file2");
	e3.Remove();
}

void VEntryTest::testCreate() {
	VEntry e;
	CPPUNIT_ASSERT(e.InitCheck() == V_NO_INIT);
	//VEntry(const VDirectory *dir, const char *path, bool traverse = false);

	VEntry e1("/home/clements/.bashrc");
	CPPUNIT_ASSERT(e1.InitCheck() == V_OK);
	CPPUNIT_ASSERT(e1.Exists());
	e1.Unset();
	CPPUNIT_ASSERT(e1.InitCheck() == V_NO_INIT);
	e1.SetTo("/home/clements/this-doesn't-exist");
	CPPUNIT_ASSERT(e1.InitCheck() == V_OK);
	CPPUNIT_ASSERT(!e1.Exists());

	VEntry e3(e1);
	CPPUNIT_ASSERT(e3.InitCheck() == V_OK);
	CPPUNIT_ASSERT(!e3.Exists());
	VPath p;
	CPPUNIT_ASSERT(e3.GetPath(&p) == V_OK);
	CPPUNIT_ASSERT(strcmp("/home/clements", p.Path()) == 0);
	CPPUNIT_ASSERT(strcmp("this-doesn't-exist", p.Leaf()) == 0);

	e1.SetTo("/tmp/test_file", true);
	CPPUNIT_ASSERT(e1.Exists());

	//status_t SetTo(const VDirectory *dir, const char *path, bool traverse = false);
}

void VEntryTest::testStatable() {
	VEntry e1("/home/clements/.bashrc");
	CPPUNIT_ASSERT(e1.InitCheck() == V_OK);
	struct stat st;
	CPPUNIT_ASSERT(e1.GetStat(&st) == V_OK);
	cout << "size: " << st.st_size << endl;
	CPPUNIT_ASSERT(e1.SetTo("/tmp/test_file", true) == V_OK);
	CPPUNIT_ASSERT(e1.Exists());
	CPPUNIT_ASSERT(e1.GetStat(&st) == V_OK);
	CPPUNIT_ASSERT(st.st_size == 35*strlen("This is a sample string for the test file.\n"));

	time_t t;
	CPPUNIT_ASSERT(e1.GetCreationTime(&t) == V_OK);
	cout << "Creation time: " << ctime(&t);

	CPPUNIT_ASSERT(e1.SetCreationTime(t) == V_ERROR);

	CPPUNIT_ASSERT(e1.GetModificationTime(&t) == V_OK);
	cout << "Modification time: " << ctime(&t);
	t += 5;
	CPPUNIT_ASSERT(e1.SetModificationTime(t) == V_OK);
	CPPUNIT_ASSERT(e1.GetModificationTime(&t) == V_OK);
	cout << "Modification time: " << ctime(&t);

	CPPUNIT_ASSERT(e1.GetAccessTime(&t) == V_OK);
	cout << "Access time: " << ctime(&t);
	t += 5;
	CPPUNIT_ASSERT(e1.SetAccessTime(t) == V_OK);
	CPPUNIT_ASSERT(e1.GetAccessTime(&t) == V_OK);
	cout << "Access time: " << ctime(&t);

	uid_t o;
	CPPUNIT_ASSERT(e1.GetOwner(&o) == V_OK);
	CPPUNIT_ASSERT(getuid() == o);
//	status_t SetOwner(uid_t owner);
	gid_t g;
	CPPUNIT_ASSERT(e1.GetGroup(&g) == V_OK);
	CPPUNIT_ASSERT(getgid() == g);
//	status_t SetGroup(gid_t group);

	mode_t m;
	CPPUNIT_ASSERT(e1.GetPermissions(&m) == V_OK);
	CPPUNIT_ASSERT(0600 == m);

	CPPUNIT_ASSERT(e1.SetPermissions(0644) == V_OK);
	CPPUNIT_ASSERT(e1.GetPermissions(&m) == V_OK);
	CPPUNIT_ASSERT(0644 == m);

	CPPUNIT_ASSERT(e1.IsFile());
	CPPUNIT_ASSERT(!e1.IsDirectory());
	CPPUNIT_ASSERT(!e1.IsSymLink());

	CPPUNIT_ASSERT(e1.SetTo("/tmp/", true) == V_OK);
	CPPUNIT_ASSERT(!e1.IsFile());
	CPPUNIT_ASSERT(e1.IsDirectory());
	CPPUNIT_ASSERT(!e1.IsSymLink());

	CPPUNIT_ASSERT(e1.SetTo("/tmp/test_file_sym", true) == V_OK);
	CPPUNIT_ASSERT(e1.IsFile());
	CPPUNIT_ASSERT(!e1.IsDirectory());
	CPPUNIT_ASSERT(e1.IsSymLink());
}

void VEntryTest::testPathOps() {
	char buf[V_FILE_NAME_LENGTH];

	VEntry e1("/tmp/test_file");
	CPPUNIT_ASSERT(e1.Exists());
	CPPUNIT_ASSERT(e1.GetName(buf) == V_OK);
	CPPUNIT_ASSERT(strcmp(buf, "test_file") == V_OK);
	VPath p("/home");
	CPPUNIT_ASSERT(e1.GetPath(&p) == V_OK);
	CPPUNIT_ASSERT(p.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp(p.Path(), "/tmp") == V_OK);
	CPPUNIT_ASSERT(strcmp(p.Leaf(), "test_file") == V_OK);
	VEntry e;
	CPPUNIT_ASSERT(e.GetPath(&p) == V_NO_INIT);
	CPPUNIT_ASSERT(p.InitCheck() == V_NO_INIT);

	CPPUNIT_ASSERT(e1.SetTo("/home/clements/.bashrc") == V_OK);
	CPPUNIT_ASSERT(e.SetTo("/tmp") == V_OK);
	CPPUNIT_ASSERT(e1.GetParent(&e) == V_OK);
	CPPUNIT_ASSERT(e.GetPath(&p) == V_OK);
	CPPUNIT_ASSERT(p.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp(p.Path(), "/home") == V_OK);
	CPPUNIT_ASSERT(strcmp(p.Leaf(), "clements") == V_OK);

//	status_t GetParent(VDirectory *dir) const;
}

void VEntryTest::testFileOps() {
	VEntry e1("/tmp/test_file");
	VEntry e2("/tmp/test_file2");
	CPPUNIT_ASSERT(e1.Exists());
	CPPUNIT_ASSERT(e2.Exists());
	CPPUNIT_ASSERT(e1.Rename("/tmp/test_file2") == V_FILE_EXISTS);
	CPPUNIT_ASSERT(e1.Exists());
	CPPUNIT_ASSERT(e2.Exists());
	CPPUNIT_ASSERT(e1.Rename("/tmp/test_file2", true) == V_OK);
	CPPUNIT_ASSERT(!e1.Exists());
	CPPUNIT_ASSERT(e2.Exists());
	CPPUNIT_ASSERT(e2.Rename("/tmp/test_file") == V_OK);
	CPPUNIT_ASSERT(e1.Exists());
	CPPUNIT_ASSERT(!e2.Exists());

//	status_t MoveTo(VDirectory *dir, const char *path = NULL, bool clobber = false);
}

void VEntryTest::testOperator() {
	VEntry e1("/tmp/test_file");
	VEntry e2;
	CPPUNIT_ASSERT(e1.InitCheck() == V_OK);
	CPPUNIT_ASSERT(e2.InitCheck() == V_NO_INIT);

	CPPUNIT_ASSERT(e2 != e1);
	CPPUNIT_ASSERT(!(e2 == e1));
	e2 = e1;
	CPPUNIT_ASSERT(e1.InitCheck() == V_OK);
	CPPUNIT_ASSERT(e2.InitCheck() == V_OK);
	CPPUNIT_ASSERT(e2 == e1);
	CPPUNIT_ASSERT(!(e2 != e1));
}
