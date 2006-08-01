/***************************************************************************
 *            VStringTest.cc
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

#include "VStringTest.h"

#include <iostream>
using namespace std;

void VStringTest::setUp() {
}

void VStringTest::tearDown() {
}

void VStringTest::testCreate() {
	s = new VString();
	const char *str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 0 );
	delete s;
	
	s = new VString("foo");
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 3 );
	CPPUNIT_ASSERT( strcmp(str, "foo") == 0 );
	delete s;

	s = new VString("foo", 83);
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 3 );
	CPPUNIT_ASSERT( strcmp(str, "foo") == 0 );
	delete s;

	s = new VString("This is a longer string", 6);
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 6 );
	CPPUNIT_ASSERT( strcmp(str, "This i") == 0 );
	delete s;
	
	VString s2("floppy");
	str = s2.String();
	CPPUNIT_ASSERT( strlen(str) == 6 );
	CPPUNIT_ASSERT( strcmp(str, "floppy") == 0 );
	
	VString s3(s2);
	str = s3.String();
	CPPUNIT_ASSERT( strlen(str) == 6 );
	CPPUNIT_ASSERT( strcmp(str, "floppy") == 0 );
}

void VStringTest::testAppend() {
	s = new VString("foo");
	VString s2("bar");
	s->Append(s2);
	const char *str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 6 );
	CPPUNIT_ASSERT( strcmp(str, "foobar") == 0 );
	delete s;
	
	s = new VString("foo");
	s->Append("bar");
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 6 );
	CPPUNIT_ASSERT( strcmp(str, "foobar") == 0 );
	delete s;

	s = new VString("foo");
	VString s3("A longer string");
	
	str = s3.LockBuffer(0);
	CPPUNIT_ASSERT( strlen(str) == 15 );
	CPPUNIT_ASSERT( strcmp(str, "A longer string") == 0 );
	s3.UnlockBuffer();

	s->Append(s3, 6);
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 9 );
	CPPUNIT_ASSERT( strcmp(str, "fooA long") == 0 );
	delete s;

	s = new VString("foo");
	s->Append(s3, -4);
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 3 );
	CPPUNIT_ASSERT( strcmp(str, "foo") == 0 );
	delete s;
	
	s = new VString("foo");
	s->Append("A longer string", 6);
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 9 );
	CPPUNIT_ASSERT( strcmp(str, "fooA long") == 0 );
	delete s;

	s = new VString("foo");
	s->Append('c', 4);
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 7 );
	CPPUNIT_ASSERT( strcmp(str, "foocccc") == 0 );
	delete s;
}

void VStringTest::testPrepend() {
	s = new VString("foo");
	VString s2("bar");
	s->Prepend(s2);
	const char *str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 6 );
	CPPUNIT_ASSERT( strcmp(str, "barfoo") == 0 );
	delete s;
	
	s = new VString("foo");
	s->Prepend("bar");
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 6 );
	CPPUNIT_ASSERT( strcmp(str, "barfoo") == 0 );
	delete s;

	s = new VString("foo");
	VString s3("A longer string");
	s->Prepend(s3, 6);
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 9 );
	CPPUNIT_ASSERT( strcmp(str, "A longfoo") == 0 );
	delete s;

	s = new VString("foo");
	s->Prepend(s3, -4);
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 3 );
	CPPUNIT_ASSERT( strcmp(str, "foo") == 0 );
	delete s;
	
	s = new VString("foo");
	s->Prepend("A longer string", 6);
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 9 );
	CPPUNIT_ASSERT( strcmp(str, "A longfoo") == 0 );
	delete s;

	s = new VString("foo");
	s->Prepend('c', 4);
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 7 );
	CPPUNIT_ASSERT( strcmp(str, "ccccfoo") == 0 );
	delete s;
}

void VStringTest::testInsert() {
	s = new VString("A bit longer string");
	VString s2("barnyard");
	s->Insert(s2, 5);
	const char *str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 27 );
	CPPUNIT_ASSERT( strcmp(str, "A bitbarnyard longer string") == 0 );
	delete s;
	
	s = new VString("A bit longer string");
	s->Insert(s2, 4, 5);
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 23 );
	CPPUNIT_ASSERT( strcmp(str, "A bitbarn longer string") == 0 );
	delete s;
	
	s = new VString("A bit longer string");
	s->Insert(s2, 3, 4, 5);
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 23 );
	CPPUNIT_ASSERT( strcmp(str, "A bitnyar longer string") == 0 );
	delete s;
	
	s = new VString("A bit longer string");
	s->Insert("barnyard",7);
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 27 );
	CPPUNIT_ASSERT( strcmp(str, "A bit lbarnyardonger string") == 0 );
	delete s;

	s = new VString("A bit longer string");
	s->Insert("barnyard", 5, 7);
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 24 );
	CPPUNIT_ASSERT( strcmp(str, "A bit lbarnyonger string") == 0 );
	delete s;

	s = new VString("A bit longer string");
	s->Insert("barnyard", 3, 4, 7);
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 23 );
	CPPUNIT_ASSERT( strcmp(str, "A bit lnyaronger string") == 0 );
	delete s;

	s = new VString("A bit longer string");
	s->Insert("aa", 3, 4, 7);
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 19 );
	CPPUNIT_ASSERT( strcmp(str, "A bit longer string") == 0 );
	delete s;

	s = new VString("A bit longer string");
	s->Insert('c', 3, 4);
	str = s->String();
	CPPUNIT_ASSERT( str != NULL );
	CPPUNIT_ASSERT( strlen(str) == 22 );
	CPPUNIT_ASSERT( strcmp(str, "A biccct longer string") == 0 );
	delete s;
}

void VStringTest::testCharacterEscape() {
	VString s1("A bit longer string");
	s1.CharacterEscape("oi", '!');
	const char *str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 22 );
	CPPUNIT_ASSERT( strcmp(str, "A b!it l!onger str!ing") == 0 );
	VString s2;
	s2.CharacterEscape("A bit longer string", "oi", '!');
	str = s2.String();
	CPPUNIT_ASSERT( strlen(str) == 22 );
	CPPUNIT_ASSERT( strcmp(str, "A b!it l!onger str!ing") == 0 );

	s1.CharacterDeescape('!');
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 19 );
	CPPUNIT_ASSERT( strcmp(str, "A bit longer string") == 0 );

	s2.CharacterDeescape("A b!it l!onger str!ing", '!');
	str = s2.String();
	CPPUNIT_ASSERT( strlen(str) == 19 );
	CPPUNIT_ASSERT( strcmp(str, "A bit longer string") == 0 );

}

void VStringTest::testCompare() {
	VString s1("aaa");
	VString s2("aaaa");
	VString s3("bbb");
	VString s4("bb");
	VString *s5 = new VString("aaa");
	VString *s6 = new VString("aaaa");
	VString s7("aaa");
	VString *s8 = new VString("aaa");
	
	CPPUNIT_ASSERT(s1.Compare(s2) == -1);
	CPPUNIT_ASSERT(s2.Compare(s1) == 1);
	CPPUNIT_ASSERT(s1.Compare(s7) == 0);
	
	CPPUNIT_ASSERT(s1.Compare(s3, 2) == -1);
	CPPUNIT_ASSERT(s3.Compare(s1, 2) == 1);
	CPPUNIT_ASSERT(s1.Compare(s7, 2) == 0);
	
	CPPUNIT_ASSERT(s1.Compare("aaaa") == -1);
	CPPUNIT_ASSERT(s2.Compare("aaa") == 1);
	CPPUNIT_ASSERT(s1.Compare("aaa") == 0);
	
	CPPUNIT_ASSERT(s1.Compare("bbb", 2) == -1);
	CPPUNIT_ASSERT(s3.Compare("aaa", 2) == 1);
	CPPUNIT_ASSERT(s1.Compare("aaa", 2) == 0);
	
	CPPUNIT_ASSERT(VString::Compare(s1, s2) == -1);
	CPPUNIT_ASSERT(VString::Compare(s2, s1) == 1);
	CPPUNIT_ASSERT(VString::Compare(s1, s7) == 0);
	
	CPPUNIT_ASSERT(VString::Compare(s6, s5) == 1);
	CPPUNIT_ASSERT(VString::Compare(s5, s6) == -1);
	CPPUNIT_ASSERT(VString::Compare(s5, s8) == 0);
	
	CPPUNIT_ASSERT(s1.ICompare("AaAa") == -1);
	CPPUNIT_ASSERT(s2.ICompare("AaA") == 1);
	CPPUNIT_ASSERT(s1.ICompare("aAa") == 0);
	
	CPPUNIT_ASSERT(s1.ICompare("bBb", 2) == -1);
	CPPUNIT_ASSERT(s3.ICompare("aAa", 2) == 1);
	CPPUNIT_ASSERT(s1.ICompare("AaA", 2) == 0);
	
	delete s5;
	delete s6;
	delete s8;
}

void VStringTest::testCopyInto() {
	VString s1("foo");
	VString s2;
	char s3[256];
	
	s1.CopyInto(s2, 1, 2);
	const char *str = s2.String();
	CPPUNIT_ASSERT( strlen(str) == 2 );
	CPPUNIT_ASSERT( strcmp(str, "oo") == 0 );
	s2.Clear();
	
	s1.CopyInto(s3, 1, 2);
	str = s3;
	CPPUNIT_ASSERT( strlen(str) == 2 );
	CPPUNIT_ASSERT( strcmp(str, "oo") == 0 );

	s1.MoveInto(s2, 1, 2);
	str = s2.String();
	CPPUNIT_ASSERT( strlen(str) == 2 );
	CPPUNIT_ASSERT( strcmp(str, "oo") == 0 );
	
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 0 );

	s1.SetTo("foo");
	s1.MoveInto(s3, 1, 2);
	str = s3;
	CPPUNIT_ASSERT( strlen(str) == 2 );
	CPPUNIT_ASSERT( strcmp(str, "oo") == 0 );
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 0 );
}

void VStringTest::testFind() {
	VString s1("A longer string");
	CPPUNIT_ASSERT( s1.FindFirst(VString("ger")) == 5 );
	CPPUNIT_ASSERT( s1.FindFirst(VString("ger"), 3) == 5 );
	CPPUNIT_ASSERT( s1.FindFirst(VString("ger"), 43) == V_ERROR );
	CPPUNIT_ASSERT( s1.FindFirst(VString("ger"), -1) == V_ERROR );
	CPPUNIT_ASSERT( s1.FindFirst("ger") == 5 );
	CPPUNIT_ASSERT( s1.FindFirst("ger", 3) == 5 );
	CPPUNIT_ASSERT( s1.FindFirst("ger", 43) == V_ERROR );
	CPPUNIT_ASSERT( s1.FindFirst('g') == 5 );
	CPPUNIT_ASSERT( s1.FindFirst('g', 8) == 14 );
	CPPUNIT_ASSERT( s1.FindFirst('g', 43) == V_ERROR );
	CPPUNIT_ASSERT( s1.FindFirst('z') == -1 );
	
	CPPUNIT_ASSERT( s1.FindLast(VString("ger")) == 5 );
	CPPUNIT_ASSERT( s1.FindLast(VString("ger"), 3) == 5 );
	CPPUNIT_ASSERT( s1.FindLast(VString("ger"), 43) == V_ERROR );
	CPPUNIT_ASSERT( s1.FindLast(VString("ger"), -1) == V_ERROR );
	CPPUNIT_ASSERT( s1.FindLast("ger") == 5 );
	CPPUNIT_ASSERT( s1.FindLast("ger", 3) == 5 );
	CPPUNIT_ASSERT( s1.FindLast("ger", 43) == V_ERROR );
	CPPUNIT_ASSERT( s1.FindLast('g') == 14 );
	CPPUNIT_ASSERT( s1.FindLast('g', 8) == 14 );
	CPPUNIT_ASSERT( s1.FindLast('g', 43) == V_ERROR );
	CPPUNIT_ASSERT( s1.FindLast('z') == -1 );
	CPPUNIT_ASSERT( s1.FindLast('n') == 13 );
	CPPUNIT_ASSERT( s1.FindLast('A') == 0 );

	CPPUNIT_ASSERT( s1.IFindFirst(VString("Ger")) == 5 );
	CPPUNIT_ASSERT( s1.IFindFirst(VString("geR"), 3) == 5 );
	CPPUNIT_ASSERT( s1.IFindFirst(VString("gEr"), 43) == V_ERROR );
	CPPUNIT_ASSERT( s1.IFindFirst(VString("Ger"), -1) == V_ERROR );
	CPPUNIT_ASSERT( s1.IFindFirst("gEr") == 5 );
	CPPUNIT_ASSERT( s1.IFindFirst("Ger", 3) == 5 );
	CPPUNIT_ASSERT( s1.IFindFirst("geR", 43) == V_ERROR );
	
	CPPUNIT_ASSERT( s1.IFindLast(VString("Ger")) == 5 );
	CPPUNIT_ASSERT( s1.IFindLast(VString("gEr"), 3) == 5 );
	CPPUNIT_ASSERT( s1.IFindLast(VString("geR"), 43) == V_ERROR );
	CPPUNIT_ASSERT( s1.IFindLast(VString("Ger"), -1) == V_ERROR );
	CPPUNIT_ASSERT( s1.IFindLast("gEr") == 5 );
	CPPUNIT_ASSERT( s1.IFindLast("geR", 3) == 5 );
	CPPUNIT_ASSERT( s1.IFindLast("Ger", 43) == V_ERROR );
}

void VStringTest::testRemove() {
	VString s1("Abbb longer bbbstring");
	
	s1.Remove(4, 5);
	const char *str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 16 );
	CPPUNIT_ASSERT( strcmp(str, "Abbber bbbstring") == 0 );
	
	s1.SetTo("Abbb longer bbbstring");
	s1.RemoveFirst(VString("bbb"));
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 18 );
	CPPUNIT_ASSERT( strcmp(str, "A longer bbbstring") == 0 );
	
	s1.SetTo("Abbb longer bbbstring");
	s1.RemoveFirst("bbb");
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 18 );
	CPPUNIT_ASSERT( strcmp(str, "A longer bbbstring") == 0 );
	
	s1.SetTo("Abbb longer bbbstring");
	s1.RemoveLast("bbb");
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 18 );
	CPPUNIT_ASSERT( strcmp(str, "Abbb longer string") == 0 );
	
	s1.SetTo("Abbb longer bbbstring");
	s1.RemoveLast(VString("bbb"));
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 18 );
	CPPUNIT_ASSERT( strcmp(str, "Abbb longer string") == 0 );

	s1.SetTo("Abbb longer bbbstring");
	s1.RemoveAll("bbb");
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 15 );
	CPPUNIT_ASSERT( strcmp(str, "A longer string") == 0 );

	s1.SetTo("Abbb longer bbbstring");
	s1.RemoveAll(VString("bbb"));
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 15 );
	CPPUNIT_ASSERT( strcmp(str, "A longer string") == 0 );

	s1.SetTo("Abbb longer bbbstriGng");
	s1.RemoveSet("oinbG");
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 11 );
	CPPUNIT_ASSERT( strcmp(str, "A lger strg") == 0 );

}

void VStringTest::testReplace() {
	VString s1("A catlonger stcatring focatr thcatis test");
	s1.Replace("cat", "doggy", 2, 4);
	const char *str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 45 );
	CPPUNIT_ASSERT( strcmp(str, "A catlonger stdoggyring fodoggyr thcatis test") == 0 );
	
	s1.Replace('g', 'f', 4, 10);
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 45 );
	CPPUNIT_ASSERT( strcmp(str, "A catlonger stdoffyrinf fodofgyr thcatis test") == 0 );

	s1.ReplaceFirst("stdoffyrinf fodofgyr", "string for");
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 35 );
	CPPUNIT_ASSERT( strcmp(str, "A catlonger string for thcatis test") == 0 );

	s1.ReplaceFirst('c', 'b');
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 35 );
	CPPUNIT_ASSERT( strcmp(str, "A batlonger string for thcatis test") == 0 );

	s1.ReplaceLast("thcatis", "this");
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 32 );
	CPPUNIT_ASSERT( strcmp(str, "A batlonger string for this test") == 0 );
	
	s1.ReplaceLast('i', 'a');
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 32 );
	CPPUNIT_ASSERT( strcmp(str, "A batlonger string for thas test") == 0 );

	s1.ReplaceAll("bat", "", 1);
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 29 );
	CPPUNIT_ASSERT( strcmp(str, "A longer string for thas test") == 0 );
	
	s1.ReplaceAll('e', 'z');
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 29 );
	CPPUNIT_ASSERT( strcmp(str, "A longzr string for thas tzst") == 0 );

	s1.ReplaceSet("z", "e");
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 29 );
	CPPUNIT_ASSERT( strcmp(str, "A longer string for thas test") == 0 );

	s1.ReplaceSet("a", 'i');
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 29 );
	CPPUNIT_ASSERT( strcmp(str, "A longer string for this test") == 0 );

	s1.SetTo("A catlonger stcaTring foCatr thcatis test");
	s1.IReplace("cat", "doggy", 2, 4);
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 45 );
	CPPUNIT_ASSERT( strcmp(str, "A catlonger stdoggyring fodoggyr thcatis test") == 0 );
	
	s1.IReplace('G', 'f', 4, 10);
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 45 );
	CPPUNIT_ASSERT( strcmp(str, "A catlonger stdoffyrinf fodofgyr thcatis test") == 0 );

	s1.IReplaceFirst("stdoFfYrinf fodofgyr", "string for");
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 35 );
	CPPUNIT_ASSERT( strcmp(str, "A catlonger string for thcatis test") == 0 );

	s1.IReplaceFirst('C', 'b');
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 35 );
	CPPUNIT_ASSERT( strcmp(str, "A batlonger string for thcatis test") == 0 );

	s1.IReplaceLast("thcaTis", "this");
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 32 );
	CPPUNIT_ASSERT( strcmp(str, "A batlonger string for this test") == 0 );
	
	s1.IReplaceLast('I', 'a');
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 32 );
	CPPUNIT_ASSERT( strcmp(str, "A batlonger string for thas test") == 0 );

	s1.IReplaceAll("bAt", "", 1);
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 29 );
	CPPUNIT_ASSERT( strcmp(str, "A longer string for thas test") == 0 );
	
	s1.IReplaceAll('E', 'Z');
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 29 );
	CPPUNIT_ASSERT( strcmp(str, "A longZr string for thas tZst") == 0 );
	
	s1.IReplaceAll('z', 'e');
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 29 );
	CPPUNIT_ASSERT( strcmp(str, "A longer string for thas test") == 0 );
}

void VStringTest::testSetTo() {
	VString s("A bit longer string");
	VString s1;
	
	s1.SetTo(s);
	const char *str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 19 );
	CPPUNIT_ASSERT( strcmp(str, "A bit longer string") == 0 );
	s1.Clear();
	
	s1.SetTo(s, 83);
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 19 );
	CPPUNIT_ASSERT( strcmp(str, "A bit longer string") == 0 );
	s1.Clear();

	s1.SetTo(s, -8);
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 0 );
	s1.Clear();

	s1.SetTo(s, 8);
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 8 );
	CPPUNIT_ASSERT( strcmp(str, "A bit lo") == 0 );
	s1.Clear();

	const char *s2 = "A bit longer string";
	s1.SetTo(s2);
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 19 );
	CPPUNIT_ASSERT( strcmp(str, "A bit longer string") == 0 );
	s1.Clear();
	
	s1.SetTo(s2, 83);
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 19 );
	CPPUNIT_ASSERT( strcmp(str, "A bit longer string") == 0 );
	s1.Clear();

	s1.SetTo(s2, -8);
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 0 );
	s1.Clear();

	s1.SetTo(s2, 8);
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 8 );
	CPPUNIT_ASSERT( strcmp(str, "A bit lo") == 0 );
}

void VStringTest::testAdopt() {
	s = new VString("A bit longer string");
	VString s1;
	s1.Adopt(*s);
	const char *str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 19 );
	CPPUNIT_ASSERT( strcmp(str, "A bit longer string") == 0 );
	str = s->String();
	
	CPPUNIT_ASSERT( strlen(str) == 0 );
	delete s;
	s1.Clear();
	
	s = new VString("A bit longer string");
	s1.Adopt(*s, 8);
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 8 );
	CPPUNIT_ASSERT( strcmp(str, "A bit lo") == 0 );
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 0 );
	delete s;
	s1.Clear();

	s = new VString("A bit longer string");
	s1.Adopt(*s, 654);
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 19 );
	CPPUNIT_ASSERT( strcmp(str, "A bit longer string") == 0 );
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 0 );
	delete s;
	s1.Clear();
	
	s = new VString("A bit longer string");
	s1.Adopt(*s, -4);
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 0 );
	str = s->String();
	CPPUNIT_ASSERT( strlen(str) == 0 );
	delete s;
	s1.Clear();
	
	VString s3("A bit longer string");
	s1.Adopt(s3);
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 19 );
	CPPUNIT_ASSERT( strcmp(str, "A bit longer string") == 0 );
	str = s3.String();
	CPPUNIT_ASSERT( strlen(str) == 0 );
}

void VStringTest::testCountChars() {
	VString *s = new VString("foo");
	CPPUNIT_ASSERT( s->CountChars() == 3 );
	CPPUNIT_ASSERT( s->Length() == 3 );
	CPPUNIT_ASSERT( s->ByteAt(-43) == 0 );
	CPPUNIT_ASSERT( s->ByteAt(-1) == 0 );
	CPPUNIT_ASSERT( s->ByteAt(543) == 0 );
	CPPUNIT_ASSERT( s->ByteAt(3) == 0 );
	CPPUNIT_ASSERT( s->ByteAt(0) == 'f' );
	CPPUNIT_ASSERT( s->ByteAt(1) == 'o' );
	CPPUNIT_ASSERT( s->ByteAt(2) == 'o' );
	delete s;
	
	VString s1("This is a LonGer string");
	s1.Truncate(43);
	const char *str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 23 );
	CPPUNIT_ASSERT( strcmp(str, "This is a LonGer string") == 0 );
	s1.Truncate(13);
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 13 );
	CPPUNIT_ASSERT( strcmp(str, "This is a Lon") == 0 );

	s1.SetTo("This is a LonGer string");
	s1.ToLower();
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 23 );
	CPPUNIT_ASSERT( strcmp(str, "this is a longer string") == 0 );

	s1.SetTo("This is a LonGer string");
	s1.ToUpper();
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 23 );
	CPPUNIT_ASSERT( strcmp(str, "THIS IS A LONGER STRING") == 0 );

	s1.SetTo(". 	this is a LonGer string");
	s1.Capitalize();
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 26 );
	CPPUNIT_ASSERT( strcmp(str, ". 	This is a LonGer string") == 0 );

	s1.SetTo("This is a LonGer string");
	s1.CapitalizeEachWord();
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 23 );
	CPPUNIT_ASSERT( strcmp(str, "This Is A LonGer String") == 0 );
}

void VStringTest::testSubstring() {
	VString *s = new VString("A longer string");
	// start, end
	VString &ans = s->Substring(4, 8);
	const char *str = ans.String();
	CPPUNIT_ASSERT( strlen(str) == 4 );
	CPPUNIT_ASSERT( strcmp(str, "nger") == 0 );
	
	ans = s->Substring(4);
	str = ans.String();
	CPPUNIT_ASSERT( strlen(str) == 11 );
	CPPUNIT_ASSERT( strcmp(str, "nger string") == 0 );
	
	ans = s->Substring(8, 4);
	str = ans.String();
	CPPUNIT_ASSERT( strlen(str) == 4 );
	CPPUNIT_ASSERT( strcmp(str, "nger") == 0 );
	
	ans = s->Substring(-8, 4);
	str = ans.String();
	CPPUNIT_ASSERT( strlen(str) == 4 );
	CPPUNIT_ASSERT( strcmp(str, "A lo") == 0 );
	
	ans = s->Substring(-8, -4);
	str = ans.String();
	CPPUNIT_ASSERT( strlen(str) == 0 );
	CPPUNIT_ASSERT( ans.Length() == 0 );
	
	delete s;
	VString s1("foo");
	ans = s1.Substring(1, 3);
	str = ans.String();
	CPPUNIT_ASSERT( strlen(str) == 2 );
	CPPUNIT_ASSERT( strcmp(str, "oo") == 0 );
}

void VStringTest::testOperators() {
	VString s1;
	s1 = "This is a string";
	const char *str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 16 );
	CPPUNIT_ASSERT( strcmp(str, "This is a string") == 0 );
	
	s1 = 'A';
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 1 );
	CPPUNIT_ASSERT( strcmp(str, "A") == 0 );

	s1 += VString(" longer");
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 8 );
	CPPUNIT_ASSERT( strcmp(str, "A longer") == 0 );

	s1 += " strin";
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 14 );
	CPPUNIT_ASSERT( strcmp(str, "A longer strin") == 0 );

	s1 += 'g';
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 15 );
	CPPUNIT_ASSERT( strcmp(str, "A longer string") == 0 );

	s1 << " to ";
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 19 );
	CPPUNIT_ASSERT( strcmp(str, "A longer string to ") == 0 );

	s1 << "test";
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 23 );
	CPPUNIT_ASSERT( strcmp(str, "A longer string to test") == 0 );
	
	s1 << '.';
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 24 );
	CPPUNIT_ASSERT( strcmp(str, "A longer string to test.") == 0 );

	uint32_t a = 786;
	s1 << a;
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 27 );
	CPPUNIT_ASSERT( strcmp(str, "A longer string to test.786") == 0 );

	int32_t b = -97;
	s1 << b;
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 30 );
	CPPUNIT_ASSERT( strcmp(str, "A longer string to test.786-97") == 0 );
	
	s1 ="short";
	uint64_t c = 786;
	s1 << c;
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 8 );
	CPPUNIT_ASSERT( strcmp(str, "short786") == 0 );

	int64_t d = -97;
	s1 << d;
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 11 );
	CPPUNIT_ASSERT( strcmp(str, "short786-97") == 0 );
	
	float e = -83.76;
	s1 << e;
	str = s1.String();
	CPPUNIT_ASSERT( strlen(str) == 21 );
	CPPUNIT_ASSERT( strcmp(str, "short786-97-83.760002") == 0 );
	
	CPPUNIT_ASSERT( s1[5] == '7' );
	CPPUNIT_ASSERT( s1[-1] == 0 );
	CPPUNIT_ASSERT( s1[65] == 0 );
	CPPUNIT_ASSERT( s1[13] == '3' );
	char& charRef = s1[5];
	CPPUNIT_ASSERT( charRef == '7' );
	charRef = s1[-1];
	CPPUNIT_ASSERT( charRef == 0 );
	charRef = s1[76];
	CPPUNIT_ASSERT( charRef == 0 );
	charRef = s1[13];
	CPPUNIT_ASSERT( charRef == '3' );

	s1 ="short";
	CPPUNIT_ASSERT( s1 == VString("short") );
	CPPUNIT_ASSERT( s1 == "short" );

	CPPUNIT_ASSERT( s1 != VString("bleah") );
	CPPUNIT_ASSERT( s1 != "bleah" );
	
	CPPUNIT_ASSERT( s1 < VString("shorter") );
	CPPUNIT_ASSERT( s1 < "shorter" );

	CPPUNIT_ASSERT( s1 <= VString("shorter") );
	CPPUNIT_ASSERT( s1 <= VString("short") );
	CPPUNIT_ASSERT( s1 <= "shorter" );
	CPPUNIT_ASSERT( s1 <= "short" );
	
	CPPUNIT_ASSERT( s1 > VString("bleah") );
	CPPUNIT_ASSERT( s1 > "bleah" );

	CPPUNIT_ASSERT( s1 >= VString("bleah") );
	CPPUNIT_ASSERT( s1 >= VString("short") );
	CPPUNIT_ASSERT( s1 >= "bleah" );
	CPPUNIT_ASSERT( s1 >= "short" );

	CPPUNIT_ASSERT( "short" == s1 );
	CPPUNIT_ASSERT( "bleah" != s1 );
	CPPUNIT_ASSERT( "bleah" < s1 );
	CPPUNIT_ASSERT( "bleah" <= s1 );
	CPPUNIT_ASSERT( "short" <= s1 );
	CPPUNIT_ASSERT( "zip" > s1 );
	CPPUNIT_ASSERT( "zip" >= s1 );
	CPPUNIT_ASSERT( "short" >= s1 );
}
