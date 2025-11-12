# Makefile for FEZ Game
CC = gcc
CFLAGS = -std=c99 -Wall -O2 -Iinclude -DGL_GLEXT_LEGACY
LDFLAGS = -lGL -lGLU -lSDL2 -lSDL2main -lm
SOURCES = src/main.c src/math_world.c src/software_renderer.c src/shader_renderer.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = build/fez_game

# Use binutils tools for additional processing if needed
OBJCOPY = objcopy
OBJDUMP = objdump
NM = nm
SIZE = size

# Default target
all: $(TARGET)

# Create build directory and compile
$(TARGET): $(OBJECTS) | build
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Build complete! Executable: $(TARGET)"
	@echo "Binary size information:"
	@$(SIZE) $(TARGET)

# Create build directory
build:
	@mkdir -p build

# Compile source files to objects
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Additional target to run objdump for verification
inspect: $(TARGET)
	@echo "Symbol table:"
	@$(NM) -D $(TARGET) | grep -E "(render|update|main|world|cast)" | head -15
	@echo "Disassembly of main function:"
	@$(OBJDUMP) -d $(TARGET) | grep -A 20 "main$$"

# Clean build artifacts
clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)

# Run the game
run: $(TARGET)
	./$(TARGET)

# Show size information
size: $(TARGET)
	@$(SIZE) $(TARGET)

# Phony targets
.PHONY: all clean run size inspect