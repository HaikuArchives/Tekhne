## Process this file with automake to produce Makefile.in

## Created by Anjuta - will be overwritten
## If you don't want it to overwrite it,
## 	Please disable it in the Anjuta project configuration

INCLUDES =\
	 -I../include -I../../tekhne/include

AM_CXXFLAGS =\
	 -D__STDC_LIMIT_MACROS\
	-pthread\
	 -Wall\
	 -g

bin_PROGRAMS = test

test_SOURCES = \
	main.cc\
	ThreadTest.cc\
	VBlockCacheTest.cc\
	VMallocIOTest.cc\
	VMessageTest.cc\
	VPointTest.cc\
	VStringTest.cc\
	VListTest.cc \
	VMessengerTest.cc

test_LDFLAGS = \
	 -L../../tekhne/src

test_LDADD = \
	 -ltekhne -lcppunit

