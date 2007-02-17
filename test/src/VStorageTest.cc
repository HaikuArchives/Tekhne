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

static inline char *get_cwd() {
	if (!cwd) {
		cwd = (char*)malloc(V_PATH_NAME_LENGTH);
		getcwd(cwd, V_PATH_NAME_LENGTH);
	}
	return cwd;
}

static inline void make_test_file() {
	const char *s = "This is a sample string for the test file.\n";
	int32_t len = strlen(s);
	VFile f("/tmp/test_file", O_CREAT|O_TRUNC|O_RDWR);
	for (int i=0;i<35;i++) {
		f.Write(s, len);
	}
	symlink ("/tmp/test_file", "/tmp/test_file_sym");
	close(open("/tmp/test_file2", O_RDWR | O_CREAT | O_TRUNC, 0644));
}

static inline void remove_test_file() {
	VEntry e1("/tmp/test_file_sym");
	e1.Remove();
	VEntry e2("/tmp/test_file");
	e2.Remove();
	VEntry e3("/tmp/test_file2");
	e3.Remove();
}

inline void output_octal(int32_t x) {
	cout.setf(ios_base::oct, ios_base::basefield);
	cout << x;
	cout.setf(ios_base::dec, ios_base::basefield);
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

	VDirectory d("/tmp");
	VPath p8(&d);
	CPPUNIT_ASSERT(p8.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("/", p8.Path()) == 0);
	CPPUNIT_ASSERT(strcmp("tmp", p8.Leaf()) == 0);

	VEntry e("/tmp");
	VPath p9(&e);
	CPPUNIT_ASSERT(p9.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("/", p9.Path()) == 0);
	CPPUNIT_ASSERT(strcmp("tmp", p9.Leaf()) == 0);
}

void VPathTest::testAppend() {
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
	CPPUNIT_ASSERT(p.SetTo("/home/foo/", "bar/..baz/") == V_BAD_VALUE);
	CPPUNIT_ASSERT(p.InitCheck() == V_NO_INIT);

	VDirectory d("/home/clements");
	CPPUNIT_ASSERT(p.SetTo(&d) == V_OK);
	CPPUNIT_ASSERT(p.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("/home", p.Path()) == 0);
	CPPUNIT_ASSERT(strcmp("clements", p.Leaf()) == 0);
	d.SetTo("/home/clements");
	CPPUNIT_ASSERT(p.SetTo(&d, ".bashrc") == V_OK);
	CPPUNIT_ASSERT(p.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("/home/clements", p.Path()) == 0);
	CPPUNIT_ASSERT(strcmp(".bashrc", p.Leaf()) == 0);

	VEntry e("/home/clements");
	CPPUNIT_ASSERT(p.SetTo(&e) == V_OK);
	CPPUNIT_ASSERT(p.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp("clements", p.Leaf()) == 0);
	CPPUNIT_ASSERT(strcmp("/home", p.Path()) == 0);
}

void VPathTest::testFlatten() {
	VPath p(".");
	CPPUNIT_ASSERT(p.AllowsTypeCode(V_STRING_TYPE));
	ssize_t len = p.FlattenedSize();
	CPPUNIT_ASSERT(len == (ssize_t)strlen(get_cwd())+1);
	char *buf = (char *)malloc(len);
	CPPUNIT_ASSERT(p.Flatten(buf, len) == V_OK);
	CPPUNIT_ASSERT(p.TypeCode() == V_STRING_TYPE);
	VPath q;
	CPPUNIT_ASSERT(q.Unflatten(V_STRING_TYPE, buf, len) == V_OK);
	CPPUNIT_ASSERT(q.InitCheck() == V_OK);
	CPPUNIT_ASSERT(q == p);
	CPPUNIT_ASSERT(!p.IsFixedSize());
}

void VPathTest::testAssignment() {
	VPath p("/home/clements");
	VPath q;
	q = p;
	CPPUNIT_ASSERT(p == q);
	CPPUNIT_ASSERT(!(p != q));
	q.Unset();
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
	make_test_file();
}

void VEntryTest::tearDown() {
	remove_test_file();
}

void VEntryTest::testCreate() {
	VEntry e;
	CPPUNIT_ASSERT(e.InitCheck() == V_NO_INIT);
	VDirectory d("/tmp");
	VEntry e11(&d, "test_file");
	CPPUNIT_ASSERT(e11.InitCheck() == V_OK);
	CPPUNIT_ASSERT(e11.Exists());
	e11.Unset();

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
	e1.Unset();

	e1.SetTo(&d, "test_file");
	CPPUNIT_ASSERT(e1.InitCheck() == V_OK);
	CPPUNIT_ASSERT(e1.Exists());
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
	CPPUNIT_ASSERT(S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH == m);

	CPPUNIT_ASSERT(e1.SetPermissions(0600) == V_OK);
	CPPUNIT_ASSERT(e1.GetPermissions(&m) == V_OK);
	CPPUNIT_ASSERT(0600 == m);

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

	VDirectory d;
	CPPUNIT_ASSERT(e1.GetParent(&d) == V_OK);
	CPPUNIT_ASSERT(d.InitCheck() == V_OK);
	CPPUNIT_ASSERT(d.Exists());
	CPPUNIT_ASSERT(d.GetPath(&p) == V_OK);
	CPPUNIT_ASSERT(p.InitCheck() == V_OK);
	CPPUNIT_ASSERT(strcmp(p.Path(), "/home") == V_OK);
	CPPUNIT_ASSERT(strcmp(p.Leaf(), "clements") == V_OK);
}

void VEntryTest::testFileOps() {
	VEntry e1("/tmp/test_file");
	VEntry e2("/tmp/test_file2");
	CPPUNIT_ASSERT(e1.Exists());
	CPPUNIT_ASSERT(e2.Exists());
	CPPUNIT_ASSERT(e1.Rename("test_file2") == V_FILE_EXISTS);
	CPPUNIT_ASSERT(e1.Exists());
	CPPUNIT_ASSERT(e2.Exists());
	CPPUNIT_ASSERT(e1.Rename("test_file2", true) == V_OK);
	CPPUNIT_ASSERT(!e1.Exists());
	CPPUNIT_ASSERT(e2.Exists());
	CPPUNIT_ASSERT(e2.Rename("/tmp/test_file") == V_OK);
	CPPUNIT_ASSERT(e1.Exists());
	CPPUNIT_ASSERT(!e2.Exists());

	VDirectory home("/home/clements");
	VDirectory tmp("/tmp");
	VEntry eHome("/home/clements/test_file");
	VEntry eTmp("/home/clements/test_file3");
	CPPUNIT_ASSERT(e1.MoveTo(&home) == V_OK);
	CPPUNIT_ASSERT(!e1.Exists());
	CPPUNIT_ASSERT(eHome.Exists());
	CPPUNIT_ASSERT(!eTmp.Exists());
	eHome.MoveTo(&tmp, "test_file3");
	VEntry eTmp2("/tmp/test_file3");
	CPPUNIT_ASSERT(eTmp2.Exists());
	CPPUNIT_ASSERT(eTmp2.Rename("test_file") == V_OK);
	VPath tmpPath(&eTmp2);
	tmpPath.SetTo(&e1);
	CPPUNIT_ASSERT(e1.Exists());
	CPPUNIT_ASSERT(!e2.Exists());

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

void VDirectoryTest::setUp() {
	make_test_file();
}

void VDirectoryTest::tearDown() {
	remove_test_file();
	VEntry e3("/tmp/test_file3");
	e3.Remove();
}

void VDirectoryTest::testCreate() {
	VEntry e("/etc/init.d");
	VDirectory d1(&e);
	CPPUNIT_ASSERT(d1.InitCheck() == V_OK);
	VDirectory d2("/home");
	CPPUNIT_ASSERT(d2.InitCheck() == V_OK);
	VDirectory d3(&d1);
	CPPUNIT_ASSERT(d3.InitCheck() == V_OK);
	VDirectory d4(&d2, "clements");
	CPPUNIT_ASSERT(d4.InitCheck() == V_OK);
	VDirectory d5;
	CPPUNIT_ASSERT(d5.InitCheck() == V_NO_INIT);
	VDirectory d6(d4);
	CPPUNIT_ASSERT(d6.InitCheck() == V_OK);
}

void VDirectoryTest::testContains() {
	VDirectory d("/tmp");
	CPPUNIT_ASSERT(d.Contains("test_file") == V_OK);
	CPPUNIT_ASSERT(d.Contains("/tmp/test_file_sym", V_SYMLINK_NODE));
	CPPUNIT_ASSERT(d.Contains("/tmp/test_file_sym", V_FILE_NODE));
	CPPUNIT_ASSERT(d.Contains("/tmp/test_file_sym", V_DIRECTORY_NODE));
	CPPUNIT_ASSERT(d.Contains("/tmp/test_file_sym"));

	VEntry e(&d, "test_file2");
	CPPUNIT_ASSERT(d.Contains(&e));
	CPPUNIT_ASSERT(!d.Contains(&e, V_DIRECTORY_NODE));
	CPPUNIT_ASSERT(d.Contains(&e, V_FILE_NODE));
	CPPUNIT_ASSERT(d.Contains(&e));
	e.SetTo(&d, "foo");
	CPPUNIT_ASSERT(!d.Contains(&e));
}

void VDirectoryTest::testCreateStuff() {
	VFile f;
	VDirectory d("/tmp");
	CPPUNIT_ASSERT(d.CreateFile("test_file", &f, true) == V_FILE_EXISTS);
	CPPUNIT_ASSERT(f.InitCheck() == V_NO_INIT);
	CPPUNIT_ASSERT(d.CreateFile("test_file", &f) == V_OK);
	CPPUNIT_ASSERT(f.InitCheck() == V_OK);
	f.Unset();
	CPPUNIT_ASSERT(d.CreateFile("test_file3", &f, true) == V_OK);
	CPPUNIT_ASSERT(f.InitCheck() == V_OK);
	f.Unset();
	CPPUNIT_ASSERT(d.CreateFile("test_file3", &f) == V_OK);
	CPPUNIT_ASSERT(f.InitCheck() == V_OK);
	f.Unset();

	VDirectory d2;
	CPPUNIT_ASSERT(d.CreateDirectory("foo", &d2) == V_OK);
	CPPUNIT_ASSERT(d2.InitCheck() == V_OK);
	CPPUNIT_ASSERT(d2.Exists());
	CPPUNIT_ASSERT(d2.Remove() == V_OK);
	CPPUNIT_ASSERT(!d2.Exists());

//	status_t CreateSymLink(const char *path, const char *linkToPath, VSymLink *link);
}

void VDirectoryTest::testEntry() {
//	status_t FindEntry(const char *path, VEntry *entry, bool traverse = false) const;

//	status_t GetEntry(VEntry *entry) const;

//	virtual status_t GetNextEntry(VEntry *entry, bool traverse = false);
//	virtual int32_t GetNextDirents(dirent *buf, size_t bufsize, int32_t count = INT_MAX);
//	virtual int32_t CountEntries(void);
//	virtual status_t Rewind(void);
}

void VDirectoryTest::testInfo() {
	VDirectory d3("/tmp");
	struct stat st;
	CPPUNIT_ASSERT(d3.GetStatFor("test_file", &st) == V_OK);
	CPPUNIT_ASSERT(!S_ISDIR(st.st_mode));
	CPPUNIT_ASSERT(S_ISREG(st.st_mode));
	CPPUNIT_ASSERT(!S_ISLNK(st.st_mode));

	CPPUNIT_ASSERT(!d3.IsRootDirectory());
	d3.SetTo("/");
	CPPUNIT_ASSERT(d3.IsRootDirectory());
	d3.Unset();
	CPPUNIT_ASSERT(!d3.IsRootDirectory());
}

void VDirectoryTest::testSetTo() {
	VEntry e("/etc/init.d");
	VDirectory d1;
	CPPUNIT_ASSERT(d1.SetTo(&e) == V_OK);
	CPPUNIT_ASSERT(d1.InitCheck() == V_OK);
	CPPUNIT_ASSERT(d1.SetTo("/home") == V_OK);
	CPPUNIT_ASSERT(d1.InitCheck() == V_OK);
	VDirectory d2;
	CPPUNIT_ASSERT(d2.SetTo(&d1, "clements") == V_OK);
	CPPUNIT_ASSERT(d2.InitCheck() == V_OK);
	d2.Unset();
	CPPUNIT_ASSERT(d2.InitCheck() == V_NO_INIT);

	d2 = d1;
	CPPUNIT_ASSERT(d2.InitCheck() == V_OK);
}

void VFileTest::setUp() {
	make_test_file();
}

void VFileTest::tearDown() {
	remove_test_file();
}

void VFileTest::testCreate() {
	VFile bashrc("/home/clements/.bashrc", O_RDONLY);
	CPPUNIT_ASSERT(bashrc.InitCheck() == V_OK);
	VFile f;
	CPPUNIT_ASSERT(f.InitCheck() == V_NO_INIT);
	VFile f2(bashrc);
	CPPUNIT_ASSERT(f2.InitCheck() == V_OK);
	bashrc.Unset();
	VEntry e("/tmp/test_file");
	VFile f3(&e, O_WRONLY|O_APPEND);
	CPPUNIT_ASSERT(f3.InitCheck() == V_OK);
	VDirectory d("/tmp");
	VFile f4(&d, "test_file", O_RDWR|O_APPEND);
	CPPUNIT_ASSERT(f4.InitCheck() == V_OK);
}

void VFileTest::testSize() {
	off_t size;
	VFile f("/tmp/test_file", O_RDONLY);
	CPPUNIT_ASSERT(f.InitCheck() == V_OK);
	CPPUNIT_ASSERT(f.GetSize(&size) == V_OK);
	CPPUNIT_ASSERT(f.SetSize(6000) == V_OK);
	CPPUNIT_ASSERT(f.GetSize(&size) == V_OK);
	CPPUNIT_ASSERT(size == 6000);
	CPPUNIT_ASSERT(f.SetSize(100) == V_OK);
	CPPUNIT_ASSERT(f.GetSize(&size) == V_OK);
	CPPUNIT_ASSERT(size == 100);
}

void VFileTest::testReadWrite() {
	VFile bashrc("/home/clements/.bashrc", O_RDONLY);
	CPPUNIT_ASSERT(bashrc.InitCheck() == V_OK);
	size_t len = 16;
	char *buf = (char *)malloc(len);
	int32_t count = 4;
	CPPUNIT_ASSERT(bashrc.Position() == 0);
	while (bashrc.ReadLine(&buf, &len) > 0 && count-- > 0) {
		cout << buf;
	}
	CPPUNIT_ASSERT(bashrc.Seek(0, SEEK_SET) == V_OK);
	count = 4;
	int32_t bytesRead = 0;
	while ((bytesRead += bashrc.Read(buf, len)) > 0 && count-- > 0) {
	}
	CPPUNIT_ASSERT(bashrc.Position() == bytesRead);
	CPPUNIT_ASSERT(bashrc.IsReadable());
	CPPUNIT_ASSERT(!bashrc.IsWritable());

	VFile f2("/tmp/test_file", O_RDWR);
	CPPUNIT_ASSERT(f2.InitCheck() == V_OK);
	CPPUNIT_ASSERT(f2.ReadAt(4, buf, 12) == 12l);
	CPPUNIT_ASSERT(memcmp(" is a sample", buf, 12) == 0);
	CPPUNIT_ASSERT(f2.WriteAt(534, "foo", strlen("foo")) == 3);
	CPPUNIT_ASSERT(f2.ReadAt(534, buf, 3) == 3l);
	CPPUNIT_ASSERT(memcmp("foo", buf, 3) == 0);
	free(buf);
}

void VFileTest::testSetTo() {
	VFile f;
	VEntry e("/tmp/test_file");
	CPPUNIT_ASSERT(f.SetTo(&e, O_RDONLY) == V_OK);
	CPPUNIT_ASSERT(f.InitCheck() == V_OK);
	off_t size;
	CPPUNIT_ASSERT(f.GetSize(&size) == V_OK);
	f.Unset();
	CPPUNIT_ASSERT(f.SetTo("/tmp/test_file", O_WRONLY) == V_OK);
	CPPUNIT_ASSERT(f.InitCheck() == V_OK);
	CPPUNIT_ASSERT(f.GetSize(&size) == V_OK);
	f.Unset();
	VDirectory d("/tmp");
	CPPUNIT_ASSERT(f.SetTo(&d, "test_file", O_WRONLY) == V_OK);
	CPPUNIT_ASSERT(f.InitCheck() == V_OK);
	CPPUNIT_ASSERT(f.GetSize(&size) == V_OK);
	VFile g;
	g = f;
	CPPUNIT_ASSERT(g.InitCheck() == V_OK);
	CPPUNIT_ASSERT(g.GetSize(&size) == V_OK);

}
