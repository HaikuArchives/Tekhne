## Process this file with automake to produce Makefile.in

## Created by Anjuta - will be overwritten
## If you don't want it to overwrite it,
## 	Please disable it in the Anjuta project configuration

INCLUDES =\
	 -I../include -I../../tekhne/include

AM_CXXFLAGS =\
	-D__STDC_LIMIT_MACROS\
	-D_REENTRANT\
	-pthread\
	-Wall\
	-g

bin_PROGRAMS = roster

roster_SOURCES = \
	main.cc\
	Roster.cc

roster_LDFLAGS = \
	-pthread\
	 -L../../tekhne/src

roster_LDADD = \
	 -ltekhne

