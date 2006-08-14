## Makefile.am -- Process this file with automake to produce Makefile.in

INCLUDES =\
	 -I../../tekhne/include

AM_CXXFLAGS =\
	-D__STDC_LIMIT_MACROS\
	-D_REENTRANT\
	-pthread\
	-Wall\
	-g


bin_PROGRAMS = ping pong ping.debug pong.debug

ping_SOURCES = ping.cc pingpong.h

ping_LDFLAGS = \
	-pthread\
	-L../../tekhne/src

ping_LDADD = \
	-ltekhne

ping_debug_SOURCES = ping.cc pingpong.h

ping_debug_LDFLAGS = \
	-pthread\
	-static\
	-L../../tekhne/src

ping_debug_LDADD = \
	-ltekhne

pong_SOURCES = pong.cc pingpong.h

pong_LDFLAGS = \
	-pthread\
	-L../../tekhne/src

pong_LDADD = \
	-ltekhne

pong_debug_SOURCES = pong.cc pingpong.h

pong_debug_LDFLAGS = \
	-pthread\
	-static\
	-L../../tekhne/src

pong_debug_LDADD = \
	-ltekhne

