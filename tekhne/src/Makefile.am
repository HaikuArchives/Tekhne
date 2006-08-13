## Process this file with automake to produce Makefile.in

INCLUDES =\
	 -I../include\
	 -I../internal

AM_CXXFLAGS =\
	-D__STDC_LIMIT_MACROS\
	-D_REENTRANT\
	-pthread\
	-Wall\
	-g

lib_LTLIBRARIES = libtekhne.la

libtekhne_la_SOURCES = \
	VApplication.cc\
	VArchivable.cc\
	VBlockCache.cc\
	VHandler.cc\
	VList.cc\
	VLocker.cc\
	VLooper.cc\
	VMallocIO.cc\
	VMemoryIO.cc\
	VMessage.cc\
	VMessageFilter.cc\
	VMessageQueue.cc\
	VMessenger.cc\
	VPoint.cc\
	VRect.cc\
	VRoster.cc\
	VString.cc\
	VStopWatch.cc\
	VMessageRunner.cc

libtekhne_la_LDFLAGS = \
	-pthread

libtekhne_la_LIBADD =

