CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -D_POSIX_C_SOURCE=200809L
LIBS = -lm

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/citygame

# Create directories if they don't exist
$(shell mkdir -p $(OBJDIR) $(BINDIR))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBS)
	@echo "Linking complete!"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@
	@echo "Compiled "$<" successfully!"

clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Cleanup complete!"

install-deps-ubuntu:
	sudo apt-get update
	sudo apt-get install libsdl2-dev

install-deps-centos:
	sudo yum install SDL2-devel

run: $(TARGET)
	./$(TARGET)