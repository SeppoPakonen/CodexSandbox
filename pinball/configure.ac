AC_INIT([pinball_game], [1.0.0], [dev@pinball.local])
AM_INIT_AUTOMAKE

AC_PROG_CXX
AC_PROG_CC

# Check for required libraries
PKG_CHECK_MODULES([GL], [gl])
PKG_CHECK_MODULES([GLFW], [glfw3])
PKG_CHECK_MODULES([GLM], [glm])
PKG_CHECK_MODULES([GLEW], [glew])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT