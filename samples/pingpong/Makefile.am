## Makefile.am -- Process this file with automake to produce Makefile.in

INCLUDES =\
	 -I../../tekhne/include

AM_CXXFLAGS =\
	-D__STDC_LIMIT_MACROS\
	-D_REENTRANT\
	-pthread\
	-Wall\
	-g


bin_PROGRAMS = ping pong

ping_SOURCES = ping.cc pingpong.h

ping_LDFLAGS = \
	-pthread\
	-L../../tekhne/src

ping_LDADD = \
	-ltekhne

pong_SOURCES = pong.cc pingpong.h

pong_LDFLAGS = \
	-pthread\
	-L../../tekhne/src

pong_LDADD = \
	-ltekhne
