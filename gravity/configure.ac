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
PKG_CHECK_MODULES([ALLEGRO], [allegro-5.0 >= 5.0], [have_allegro=yes], [have_allegro=no])
if test "x$have_allegro" != "xyes"; then
  # Fallback to allegro-config for Allegro 5
  AC_CHECK_PROG([ALLEGRO_CONFIG], [allegro-config], [allegro-config])
  if test -n "$ALLEGRO_CONFIG"; then
    ALLEGRO_CFLAGS=`$ALLEGRO_CONFIG --cflags`
    ALLEGRO_LIBS=`$ALLEGRO_CONFIG --libs`
  else
    # Try allegro5-config
    AC_CHECK_PROG([ALLEGRO5_CONFIG], [allegro5-config], [allegro5-config])
    if test -n "$ALLEGRO5_CONFIG"; then
      ALLEGRO_CFLAGS=`$ALLEGRO5_CONFIG --cflags`
      ALLEGRO_LIBS=`$ALLEGRO5_CONFIG --libs`
    else
      AC_MSG_ERROR([allegro-config or allegro5-config not found. Please install Allegro 5 development libraries.])
    fi
  fi
fi

# Check for Allegro 5 addons
PKG_CHECK_MODULES([ALLEGRO_PRIMITIVES], [allegro_primitives-5.0 >= 5.0], [have_allegro_primitives=yes], [have_allegro_primitives=no])
if test "x$have_allegro_primitives" = "xyes"; then
  ALLEGRO_CFLAGS="$ALLEGRO_CFLAGS $ALLEGRO_PRIMITIVES_CFLAGS"
  ALLEGRO_LIBS="$ALLEGRO_LIBS $ALLEGRO_PRIMITIVES_LIBS"
else
  AC_CHECK_LIB([allegro_primitives-5.0], [allegro_primitives_init], [], [AC_MSG_ERROR([allegro_primitives library not found. Please install Allegro 5 primitives addon.])])
  ALLEGRO_LIBS="$ALLEGRO_LIBS -lallegro_primitives"
fi

PKG_CHECK_MODULES([ALLEGRO_IMAGE], [allegro_image-5.0 >= 5.0], [have_allegro_image=yes], [have_allegro_image=no])
if test "x$have_allegro_image" = "xyes"; then
  ALLEGRO_CFLAGS="$ALLEGRO_CFLAGS $ALLEGRO_IMAGE_CFLAGS"
  ALLEGRO_LIBS="$ALLEGRO_LIBS $ALLEGRO_IMAGE_LIBS"
else
  AC_CHECK_LIB([allegro_image-5.0], [allegro_image_init], [], [AC_MSG_ERROR([allegro_image library not found. Please install Allegro 5 image addon.])])
  ALLEGRO_LIBS="$ALLEGRO_LIBS -lallegro_image"
fi

PKG_CHECK_MODULES([ALLEGRO_TTF], [allegro_ttf-5.0 >= 5.0], [have_allegro_ttf=yes], [have_allegro_ttf=no])
if test "x$have_allegro_ttf" = "xyes"; then
  ALLEGRO_CFLAGS="$ALLEGRO_CFLAGS $ALLEGRO_TTF_CFLAGS"
  ALLEGRO_LIBS="$ALLEGRO_LIBS $ALLEGRO_TTF_LIBS"
else
  AC_CHECK_LIB([allegro_ttf-5.0], [allegro_ttf_init], [], [AC_MSG_ERROR([allegro_ttf library not found. Please install Allegro 5 ttf addon.])])
  ALLEGRO_LIBS="$ALLEGRO_LIBS -lallegro_ttf"
fi

PKG_CHECK_MODULES([ALLEGRO_AUDIO], [allegro_audio-5.0 >= 5.0], [have_allegro_audio=yes], [have_allegro_audio=no])
if test "x$have_allegro_audio" = "xyes"; then
  ALLEGRO_CFLAGS="$ALLEGRO_CFLAGS $ALLEGRO_AUDIO_CFLAGS"
  ALLEGRO_LIBS="$ALLEGRO_LIBS $ALLEGRO_AUDIO_LIBS"
else
  AC_CHECK_LIB([allegro_audio-5.0], [allegro_audio_init], [], [AC_MSG_ERROR([allegro_audio library not found. Please install Allegro 5 audio addon.])])
  ALLEGRO_LIBS="$ALLEGRO_LIBS -lallegro_audio"
fi

PKG_CHECK_MODULES([ALLEGRO_ACODEC], [allegro_acodec-5.0 >= 5.0], [have_allegro_acodec=yes], [have_allegro_acodec=no])
if test "x$have_allegro_acodec" = "xyes"; then
  ALLEGRO_CFLAGS="$ALLEGRO_CFLAGS $ALLEGRO_ACODEC_CFLAGS"
  ALLEGRO_LIBS="$ALLEGRO_LIBS $ALLEGRO_ACODEC_LIBS"
else
  AC_CHECK_LIB([allegro_acodec-5.0], [allegro_acodec_init], [], [AC_MSG_ERROR([allegro_acodec library not found. Please install Allegro 5 acodec addon.])])
  ALLEGRO_LIBS="$ALLEGRO_LIBS -lallegro_acodec"
fi

# Checks for libraries
AC_CHECK_LIB([jgmod], [JG_Init], [], [AC_MSG_ERROR([jgmod library not found])])
AC_CHECK_LIB([lua], [lua_open], [], [AC_MSG_ERROR([lua library not found])])
AC_CHECK_LIB([lualib], [luaopen_base], [], [AC_MSG_ERROR([lualib library not found])])
AC_CHECK_LIB([tolua], [tolua_open], [], [AC_MSG_ERROR([tolua library not found])])
AC_CHECK_LIB([net], [libnet_init], [], [AC_MSG_ERROR([libnet library not found])])

# Checks for header files.
AC_CHECK_HEADERS([allegro5.h allegro5/allegro.h jgmod.h lua.h lualib.h tolua.h libnet.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT