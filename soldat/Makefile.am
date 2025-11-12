# Makefile.am for Soldat
AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

# Define the binary directory
bin_PROGRAMS = soldat_client soldat_server

# Client sources
soldat_client_SOURCES = \
	src/main.cpp \
	src/client_main.cpp

# Server sources
soldat_server_SOURCES = \
	src/server_main.cpp

# Include directories
soldat_client_CPPFLAGS = \
	-I$(top_srcdir)/soldat/stdsrc \
	-I$(top_srcdir)/soldat/pascal/client \
	-I$(top_srcdir)/soldat/pascal/shared \
	@SDL2_CFLAGS@ \
	@OPENGL_CFLAGS@ \
	@GLU_CFLAGS@ \
	@GLFW_CFLAGS@ \
	@GLEW_CFLAGS@ \
	@PHYSFS_CFLAGS@

soldat_server_CPPFLAGS = \
	-I$(top_srcdir)/soldat/stdsrc \
	-I$(top_srcdir)/soldat/pascal/server \
	-I$(top_srcdir)/soldat/pascal/shared \
	@SDL2_CFLAGS@ \
	@OPENGL_CFLAGS@ \
	@GLU_CFLAGS@ \
	@GLFW_CFLAGS@ \
	@GLEW_CFLAGS@ \
	@PHYSFS_CFLAGS@

# Link libraries
soldat_client_LDADD = @SDL2_LIBS@ @OPENGL_LIBS@ @GLU_LIBS@ @GLFW_LIBS@ @GLEW_LIBS@ @PHYSFS_LIBS@
soldat_server_LDADD = @SDL2_LIBS@ @OPENGL_LIBS@ @GLU_LIBS@ @GLFW_LIBS@ @GLEW_LIBS@ @PHYSFS_LIBS@

# Add headers to the distribution
dist_pkginclude_HEADERS = \
	soldat/stdsrc/Constants.h \
	soldat/stdsrc/Util.h \
	soldat/stdsrc/Vector.h \
	soldat/stdsrc/Calc.h \
	soldat/stdsrc/Console.h \
	soldat/stdsrc/Cvar.h \
	soldat/stdsrc/Command.h \
	soldat/stdsrc/LogFile.h \
	soldat/stdsrc/Version.h \
	soldat/stdsrc/SharedConfig.h \
	soldat/stdsrc/Demo.h \
	soldat/stdsrc/AI.h \
	soldat/stdsrc/Anims.h \
	soldat/stdsrc/AutoUpdater.h \
	soldat/stdsrc/Game.h \
	soldat/stdsrc/MapFile.h \
	soldat/stdsrc/Parts.h \
	soldat/stdsrc/PolyMap.h \
	soldat/stdsrc/TraceLog.h \
	soldat/stdsrc/Waypoints.h \
	soldat/stdsrc/Weapons.h

# Create directories if they don't exist
BUILT_SOURCES = setup_dirs
setup_dirs:
	if [ ! -d "$(top_builddir)/bin" ]; then mkdir -p "$(top_builddir)/bin"; fi
	if [ ! -d "$(top_builddir)/assets" ]; then mkdir -p "$(top_builddir)/assets"; fi
	if [ ! -d "$(top_builddir)/configs" ]; then mkdir -p "$(top_builddir)/configs"; fi
	if [ ! -d "$(top_builddir)/maps" ]; then mkdir -p "$(top_builddir)/maps"; fi

CLEANFILES = setup_dirs