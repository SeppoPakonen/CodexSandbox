#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([gravity-strike], [1.0], [jh@gravity-strike.de])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_FILES([Makefile src/Makefile])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Check for required libraries
AC_ARG_WITH([allegro],
            [AS_HELP_STRING([--with-allegro=PATH],
                           [Path to Allegro installation])],
            [ALLEGRO_CFLAGS="-I$with_allegro/include"
             ALLEGRO_LIBS="-L$with_allegro/lib -lalleg"])
AC_SUBST([ALLEGRO_CFLAGS])
AC_SUBST([ALLEGRO_LIBS])

AC_ARG_WITH([allegttf],
            [AS_HELP_STRING([--with-allegttf=PATH],
                           [Path to AllegTTF installation])],
            [ALLEGTTFCFLAGS="-I$with_allegro/include"
             ALLEGTTFLIBS="-L$with_allegro/lib -lalttf"])
AC_SUBST([ALLEGTTFCFLAGS])
AC_SUBST([ALLEGTTFLIBS])

# Check for required libraries using pkg-config if available
PKG_CHECK_MODULES([ALLEGRO], [allegro >= 4.0], [have_allegro=yes], [have_allegro=no])
if test "x$have_allegro" != "xyes"; then
  # Fallback to allegro-config
  AC_CHECK_PROG([ALLEGRO_CONFIG], [allegro-config], [allegro-config])
  if test -z "$ALLEGRO_CONFIG"; then
    AC_MSG_ERROR([allegro-config not found. Please install Allegro development libraries.])
  fi
  ALLEGRO_CFLAGS=`$ALLEGRO_CONFIG --cflags`
  ALLEGRO_LIBS=`$ALLEGRO_CONFIG --libs`
fi

# Checks for libraries
AC_CHECK_LIB([jgmod], [JG_Init], [], [AC_MSG_ERROR([jgmod library not found])])
AC_CHECK_LIB([alttf], [ttf_init], [], [AC_MSG_ERROR([alttf library not found])])
AC_CHECK_LIB([lua], [lua_open], [], [AC_MSG_ERROR([lua library not found])])
AC_CHECK_LIB([lualib], [luaopen_base], [], [AC_MSG_ERROR([lualib library not found])])
AC_CHECK_LIB([tolua], [tolua_open], [], [AC_MSG_ERROR([tolua library not found])])
AC_CHECK_LIB([net], [libnet_init], [], [AC_MSG_ERROR([libnet library not found])])

# Checks for header files.
AC_CHECK_HEADERS([allegro.h jgmod.h allegttf.h lua.h lualib.h tolua.h libnet.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT