AC_INIT([blc], [0.1.0])
AM_INIT_AUTOMAKE([foreign subdir-objects -Wall])

AC_PROG_CXX
AC_PROG_INSTALL

AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([
 Makefile
 src/Makefile
])

# Debug flag
AC_ARG_ENABLE([debug],
  AS_HELP_STRING([--enable-debug], [Build with debug flags]),
  [debug=$enableval],
  [debug=no])

AM_CONDITIONAL([DEBUG], [test "$debug" = "yes"])

AC_OUTPUT
