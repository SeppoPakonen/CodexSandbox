# CaveFly Game Makefile
# Uses binutils for compilation

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = -lglfw -lGL -lGLU -lm
TARGET = cavefly
SOURCES = src/main.c src/physics.c
OBJECTS = $(SOURCES:.c=.o)
BINDIR = bin
SRCDIR = src

# Default target
all: $(BINDIR)/$(TARGET)

# Create bin directory
$(BINDIR):
	mkdir -p $(BINDIR)

# Link the executable
$(BINDIR)/$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Alternative build method without intermediate objects
build: $(BINDIR)
	$(CC) $(CFLAGS) $(SOURCES) -o $(BINDIR)/$(TARGET) $(LDFLAGS)

# Clean build artifacts
clean:
	rm -f $(OBJECTS)
	rm -rf $(BINDIR)

# Install dependencies (for Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev build-essential

# Run the game
run: $(BINDIR)/$(TARGET)
	./$(BINDIR)/$(TARGET)

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: $(BINDIR)/$(TARGET)

# Phony targets
.PHONY: all build clean install-deps run debug