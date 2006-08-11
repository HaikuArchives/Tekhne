## Process this file with automake to produce Makefile.in

SUBDIRS = include src

testdocdir = ${prefix}/doc/test
testdoc_DATA = \
	README\
	AUTHORS\
	ChangeLog\
	LICENSE\
	NEWS\
	TODO

EXTRA_DIST = $(testdoc_DATA)

# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

