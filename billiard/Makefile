# Makefile for Billiard Simulation
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -Isrc -I/usr/include
LDFLAGS = -lm

# For OpenGL rendering
OPENGL_LIBS = -lglfw -lGLEW -lGL -lGLU

# Source files
SOURCES = src/main.c src/billiard.c src/raster_renderer.c src/opengl_renderer.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = billiard

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS) $(OPENGL_LIBS)

# Compile object files
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Install dependencies (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y libglfw3-dev libglew-dev libgl1-mesa-dev libglu1-mesa-dev

# Install dependencies (macOS with Homebrew)
install-deps-mac:
	brew install glfw glew

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Run with software rendering
run-software: $(TARGET)
	./$(TARGET) --software

# Run with OpenGL rendering
run-opengl: $(TARGET)
	./$(TARGET) --opengl

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

.PHONY: all clean run run-software run-opengl install-deps install-deps-mac debug