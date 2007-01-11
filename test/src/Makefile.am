## Process this file with automake to produce Makefile.in

INCLUDES =\
	 -I../include -I../../tekhne/include

AM_CXXFLAGS =\
	-D__STDC_LIMIT_MACROS\
	-D_REENTRANT\
	-pthread\
	-Wall\
	-g

bin_PROGRAMS = test

test_SOURCES = \
	main.cc\
	ThreadTest.cc\
	VBlockCacheTest.cc\
	VDictionaryTest.cc\
	VMallocIOTest.cc\
	VMessageTest.cc\
	VPointTest.cc\
	VPolygonTest.cc\
	VStringTest.cc\
	VListTest.cc \
	VTListTest.cc \
	VMessengerTest.cc\
	VRosterTest.cc\
	VSerialPortTest.cc

test_LDFLAGS = \
	-pthread -static\
	-L../../tekhne/src

test_LDADD = \
	-ltekhne -lcppunit
