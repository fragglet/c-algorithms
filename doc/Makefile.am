
EXTRA_DIST = html Doxyfile intro.h

html:
	DOXYGEN_DOC_DIR=$(srcdir)/ doxygen $(srcdir)/Doxyfile

clean-local:
	rm -rf html
