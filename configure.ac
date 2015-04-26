AC_INIT(C Algorithms, 1.2.0, fraggle@removethisbit.gmail.com, c-algorithms)
AC_CONFIG_AUX_DIR(autotools)
AC_CONFIG_SRCDIR([src/arraylist.c])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_MAKE_SET

if [[ "$GCC" = "yes" ]]; then
	is_gcc=true
else
	is_gcc=false
fi

TEST_CFLAGS=""

# Turn on all warnings for gcc.  Turn off optimisation for the test build.

if $is_gcc; then
	WARNINGS="-Wall -Wsign-compare -Wconversion"
	CFLAGS="$CFLAGS $WARNINGS"
	TEST_CFLAGS="$TEST_CFLAGS $WARNINGS -O0"
fi

# Support for coverage analysis via gcov:

coverage=no
AC_ARG_ENABLE(coverage, 
[  --enable-coverage       Enable coverage testing. ],
[ coverage=yes ])

if [[ "$coverage" = "yes" ]]; then
        if $is_gcc; then
                TEST_CFLAGS="$TEST_CFLAGS -fprofile-arcs -ftest-coverage"
        else
                AC_MSG_ERROR([Can only enable coverage when using gcc.])
        fi
fi

# Support for running test cases using valgrind:

use_valgrind=false
AC_ARG_ENABLE(valgrind,
[  --enable-valgrind       Use valgrind when running unit tests. ],
[ use_valgrind=true ])

if [[ "$use_valgrind" = "true" ]]; then
        AC_CHECK_PROG(HAVE_VALGRIND, valgrind, yes, no)

        if [[ "$HAVE_VALGRIND" = "no" ]]; then
                AC_MSG_ERROR([Valgrind not found in PATH. ])
        fi
fi

AM_CONDITIONAL(USE_VALGRIND, $use_valgrind)

# Save the default CFLAGS and clear them, so that the test build
# of the library doesn't get the optimisation flags.

MAIN_CFLAGS="$CFLAGS"
CFLAGS=""

AM_CONFIG_HEADER(config.h:config.h.in)

AC_SUBST(MAIN_CFLAGS)
AC_SUBST(TEST_CFLAGS)
AC_SUBST(ac_aux_dir)

AC_OUTPUT([
    Makefile
    libcalg-1.0.pc
    doc/Makefile
    src/Makefile
    test/Makefile
])

