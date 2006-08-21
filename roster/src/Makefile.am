## Process this file with automake to produce Makefile.in

INCLUDES =\
	 -I../include -I../../tekhne/include -I../../tekhne/internal

AM_CXXFLAGS =\
	-D__STDC_LIMIT_MACROS\
	-D_REENTRANT\
	-pthread\
	-Wall\
	-g

bin_PROGRAMS = roster

roster_SOURCES = \
	main.cc\
	ApplicationInfo.cc\
	Roster.cc

roster_LDFLAGS = \
	-pthread\
	 -L../../tekhne/src

roster_LDADD = \
	 -ltekhne
