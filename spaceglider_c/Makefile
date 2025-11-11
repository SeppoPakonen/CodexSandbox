# Makefile for Space Glider C Implementation
# Using binutils for building

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -I./include
LDFLAGS = -lglfw -lGL -lGLU -lm
TARGET = spaceglider
SRCDIR = src
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/game_logic.c $(SRCDIR)/physics.c $(SRCDIR)/input.c \
          $(SRCDIR)/renderer_old.c $(SRCDIR)/renderer_modern.c
OBJECTS = $(SOURCES:.c=.o)
BINDIR = build

# Use binutils programs explicitly
AS = as
LD = ld
OBJCOPY = objcopy
OBJDUMP = objdump
STRIP = strip
NM = nm

# Default target
.PHONY: all clean debug run run-modern

all: $(BINDIR)/$(TARGET)

# Create build directory
$(BINDIR):
	mkdir -p $(BINDIR)

# Link the final executable
$(BINDIR)/$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Linking complete: $@"

# Compile source files
$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled: $< -> $@"

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: $(BINDIR)/$(TARGET)_debug

$(BINDIR)/$(TARGET)_debug: $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Debug build complete: $@"

# Run the game with old school renderer
run: $(BINDIR)/$(TARGET)
	@echo "Running with old school renderer..."
	./$(BINDIR)/$(TARGET)

# Run the game with modern renderer
run-modern: $(BINDIR)/$(TARGET)
	@echo "Running with modern renderer..."
	./$(BINDIR)/$(TARGET) --modern

# Clean build artifacts
clean:
	rm -f $(OBJECTS)
	rm -rf $(BINDIR)
	@echo "Build artifacts cleaned"

# Display symbol information (using binutils)
symbols: $(BINDIR)/$(TARGET)
	@echo "Symbol table:"
	$(NM) $(BINDIR)/$(TARGET)

# Display assembly (using binutils)
disassemble: $(BINDIR)/$(TARGET)
	@echo "Disassembly:"
	$(OBJDUMP) -d $(BINDIR)/$(TARGET)

# Strip symbols to reduce binary size (using binutils)
strip: $(BINDIR)/$(TARGET)
	$(STRIP) $(BINDIR)/$(TARGET)
	@echo "Symbols stripped from binary"

# Create a minimal static binary using binutils
static: LDFLAGS = -static -lglfw -lGL -lGLU -lglut -lm
static: CFLAGS += -static
static: $(BINDIR)/$(TARGET)_static

$(BINDIR)/$(TARGET)_static: $(OBJECTS) | $(BINDIR)
	$(CC) -static $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Static build complete: $@"

# Install dependencies (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev build-essential

# Print build info
info:
	@echo "Compiler: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "LDFLAGS: $(LDFLAGS)"
	@echo "Sources: $(SOURCES)"
	@echo "Binutils tools: $(AS), $(LD), $(OBJCOPY), $(OBJDUMP), $(STRIP), $(NM)"

help:
	@echo "Available targets:"
	@echo "  all           - Build the game (default)"
	@echo "  debug         - Build with debug symbols"
	@echo "  run           - Run with old school renderer"
	@echo "  run-modern    - Run with modern renderer"
	@echo "  clean         - Remove build artifacts"
	@echo "  install-deps  - Install required dependencies"
	@echo "  symbols       - Show symbol table"
	@echo "  disassemble   - Show disassembly"
	@echo "  strip         - Strip symbols from binary"
	@echo "  static        - Create static binary"
	@echo "  info          - Show build information"
	@echo "  help          - Show this help"