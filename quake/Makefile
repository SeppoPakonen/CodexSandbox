# Makefile for Quake-inspired game

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -Iinclude
LDFLAGS = 
LIBS = -lm

# Source directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Find all source files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/quake_game

# Default target
all: $(TARGET)

# Create directories
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Build the executable
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) $(LIBS)
	@echo "Built $@ successfully!"

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Rebuild
rebuild: clean all

# Copy shaders directory to build
install-shaders:
	mkdir -p $(BINDIR)/shaders
	cp -r shaders/* $(BINDIR)/shaders/

.PHONY: all clean rebuild install-shaders