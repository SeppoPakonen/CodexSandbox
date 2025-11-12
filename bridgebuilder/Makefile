CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
BINUTILS_PREFIX = 
STRIP = $(BINUTILS_PREFIX)strip
OBJCOPY = $(BINUTILS_PREFIX)objcopy

# Find required libraries
LIBS = -lglfw -lGL -lGLEW -lm

# Include directories
INCLUDES = -Iinclude

# Source files
SOURCES = src/main.c src/physics.c src/renderer.c src/input.c src/bridge_builder.c src/shader_loader.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Target executable
TARGET = BridgeBuilder

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBS)
	
# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile all sources to objects
$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SOURCES)

# Strip symbols from executable using binutils
strip: $(TARGET)
	$(STRIP) --strip-unneeded $(TARGET)

# Alternative binary with only essential symbols
release: $(TARGET) strip

# Create assembly listing using binutils
asm: $(TARGET)
	objdump -d $(TARGET) > $(TARGET).asm

# Create symbol table using binutils
symbols: $(TARGET)
	nm -n $(TARGET) > $(TARGET).sym

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET) $(TARGET).asm $(TARGET).sym

# Rebuild everything
rebuild: clean all

.PHONY: all clean strip release asm symbols