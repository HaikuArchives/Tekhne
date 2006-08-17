## Process this file with automake to produce Makefile.in

INCLUDES =\
	 -I../include -I../../tekhne/include -I../../tekhne/internal

AM_CXXFLAGS =\
	-D__STDC_LIMIT_MACROS\
	-D_REENTRANT\
	-pthread\
	-Wall\
	-g

bin_PROGRAMS = roster roster.dbg

roster_SOURCES = \
	main.cc\
	ApplicationInfo.cc\
	Roster.cc

roster_LDFLAGS = \
	-pthread\
	 -L../../tekhne/src

roster_LDADD = \
	 -ltekhne

roster_dbg_SOURCES = \
	main.cc\
	ApplicationInfo.cc\
	Roster.cc

roster_dbg_LDFLAGS = \
	-pthread\
	-static\
	 -L../../tekhne/src

roster_dbg_LDADD = \
	 -ltekhne

