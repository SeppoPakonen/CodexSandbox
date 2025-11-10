AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = pinball_game

pinball_game_SOURCES = \
    src/main.cpp \
    src/renderer.cpp \
    src/shader.cpp \
    src/camera.cpp \
    src/input.cpp \
    src/ball.cpp \
    src/paddle.cpp \
    src/game.cpp

pinball_game_CXXFLAGS = $(GL_CFLAGS) $(GLFW_CFLAGS) $(GLM_CFLAGS) $(GLEW_CFLAGS)
pinball_game_LDADD = $(GL_LIBS) $(GLFW_LIBS) $(GLM_LIBS) $(GLEW_LIBS)

# Copy shaders to build directory
install-data-local:
	$(MKDIR_P) $(DESTDIR)$(datadir)/shaders
	cp -r shaders/* $(DESTDIR)$(datadir)/shaders/

# Copy shaders to the build directory for runtime
all-local:
	$(MKDIR_P) build/shaders
	cp -r shaders/* build/shaders/