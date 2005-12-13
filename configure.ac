AC_INIT(C Algorithms, 0.0.1, fraggle@users.sourceforge.net, c-algorithms)
AC_CONFIG_AUX_DIR(autotools)

AM_INIT_AUTOMAKE($PACKAGE_TARNAME, $PACKAGE_VERSION, no-define)

AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_MAKE_SET

AM_CONFIG_HEADER(config.h:config.h.in)

AC_SUBST(ac_aux_dir)

AC_OUTPUT([
    Makefile
    libcalg-1.0.pc
    doc/Makefile
    src/Makefile
    test/Makefile
])

