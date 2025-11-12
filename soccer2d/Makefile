# Makefile for Soccer2D Game
# Using binutils and gcc

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./include
LDFLAGS = -lm

# For OpenGL renderer
OPENGL_CFLAGS = -DUSE_OPENGL -I/usr/include/GL
OPENGL_LIBS = -lGL -lGLU -lglfw -lGLEW

# Source files
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET = soccer2d
TARGET_SHADER = soccer2d_shader

# Default target - terminal renderer
all: $(TARGET)

# Create object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link terminal version
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Shader version with OpenGL
$(TARGET_SHADER): CFLAGS += $(OPENGL_CFLAGS)
$(TARGET_SHADER): LDFLAGS += $(OPENGL_LIBS)
$(TARGET_SHADER): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET_SHADER) $(LDFLAGS)

# Build both versions
both: $(TARGET) $(TARGET_SHADER)

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(TARGET_SHADER)

# Install dependencies (for Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y build-essential libglfw3-dev libglew-dev

# Install dependencies (for Fedora/RHEL)
install-deps-fedora:
	sudo dnf install gcc make glfw-devel glew-devel

# Install dependencies (for Arch Linux)
install-deps-arch:
	sudo pacman -S gcc make glfw-x11 glew

# Help target
help:
	@echo "Available targets:"
	@echo "  all            - Build terminal version (default)"
	@echo "  $(TARGET_SHADER)   - Build OpenGL shader version"
	@echo "  both           - Build both versions"
	@echo "  clean          - Remove build artifacts"
	@echo "  install-deps   - Install dependencies (Ubuntu/Debian)"
	@echo "  install-deps-fedora - Install dependencies (Fedora/RHEL)"
	@echo "  install-deps-arch - Install dependencies (Arch Linux)"
	@echo ""
	@echo "Run with --shader to use the OpenGL renderer:"
	@echo "  ./$(TARGET) --shader"

.PHONY: all clean install-deps install-deps-fedora install-deps-arch help both