
AUX_DIST_GEN = $(ac_aux_dir)

DOC_FILES=            \
        COPYING.md    \
        NEWS.md       \
        README.md     \
        STYLE.md

EXTRA_DIST = $(AUX_DIST_GEN) libcalg-1.0.pc.in $(DOC_FILES) .clang-format
MAINTAINERCLEANFILES = $(AUX_DIST_GEN)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libcalg-1.0.pc

$(pkgconfig_DATA) : config.status

SUBDIRS=src test doc

format:
	clang-format -i */*.[ch]
