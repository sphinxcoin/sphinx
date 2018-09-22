m4_define([libunivalue_major_verssphx], [1])
m4_define([libunivalue_minor_verssphx], [1])
m4_define([libunivalue_micro_verssphx], [3])
m4_define([libunivalue_interface_age], [3])
# If you need a modifier for the verssphx number. 
# Normally empty, but can be used to make "fixup" releases.
m4_define([libunivalue_extraverssphx], [])

dnl libtool verssphxing from libunivalue
m4_define([libunivalue_current], [m4_eval(100 * libunivalue_minor_verssphx + libunivalue_micro_verssphx - libunivalue_interface_age)])
m4_define([libunivalue_binary_age], [m4_eval(100 * libunivalue_minor_verssphx + libunivalue_micro_verssphx)])
m4_define([libunivalue_revissphx], [libunivalue_interface_age])
m4_define([libunivalue_age], [m4_eval(libunivalue_binary_age - libunivalue_interface_age)])
m4_define([libunivalue_verssphx], [libunivalue_major_verssphx().libunivalue_minor_verssphx().libunivalue_micro_verssphx()libunivalue_extraverssphx()])


AC_INIT([univalue], [1.0.3],
        [http://github.com/jgarzik/univalue/])

dnl make the compilation flags quiet unless V=1 is used
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PREREQ(2.60)
AC_CONFIG_SRCDIR([lib/univalue.cpp])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux/m4])
AC_CONFIG_HEADERS([univalue-config.h])
AM_INIT_AUTOMAKE([subdir-objects foreign])

LIBUNIVALUE_MAJOR_VERSSPHX=libunivalue_major_verssphx
LIBUNIVALUE_MINOR_VERSSPHX=libunivalue_minor_verssphx
LIBUNIVALUE_MICRO_VERSSPHX=libunivalue_micro_verssphx
LIBUNIVALUE_INTERFACE_AGE=libunivalue_interface_age

# ABI verssphx
# http://www.gnu.org/software/libtool/manual/html_node/Updating-verssphx-info.html
LIBUNIVALUE_CURRENT=libunivalue_current
LIBUNIVALUE_REVISSPHX=libunivalue_revissphx
LIBUNIVALUE_AGE=libunivalue_age

AC_SUBST(LIBUNIVALUE_CURRENT)
AC_SUBST(LIBUNIVALUE_REVISSPHX)
AC_SUBST(LIBUNIVALUE_AGE)

LT_INIT
LT_LANG([C++])

case $host in
  *mingw*)
    LIBTOOL_APP_LDFLAGS="$LIBTOOL_APP_LDFLAGS -all-static"
  ;;
esac

BUILD_EXEEXT=
case $build in
  *mingw*)
    BUILD_EXEEXT=".exe"
  ;;
esac

AC_CONFIG_FILES([
    Makefile
    pc/libunivalue.pc
    pc/libunivalue-uninstalled.pc])

AC_SUBST(LIBTOOL_APP_LDFLAGS)
AC_SUBST(BUILD_EXEEXT)
AC_OUTPUT

