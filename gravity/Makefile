# Gravity Strike -- Modern Makefile
# Created from original source with improvements for modern Linux systems

.PHONY: all clean install uninstall dist check-deps build-legacy help

# Configuration
PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin
DATADIR = $(PREFIX)/share/gravity

# Build flags
CXX = g++
CC = gcc
CFLAGS += -O2 -Wall -Wextra -std=c11
CXXFLAGS += -O2 -Wall -Wextra -std=c++11
LDFLAGS = -ljgmod -lalttf -llua -llualib -ltolua -lnet `allegro-config --libs`

# Source directories
SRCDIR = src
OBJDIR = obj
SOURCEDIR = $(SRCDIR)

# Find all source files
CPP_SOURCES := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/menu/*.cpp $(SRCDIR)/enemies/*.cpp $(SRCDIR)/weapons/*.cpp $(SRCDIR)/contrib/*.cpp)
C_SOURCES := $(wildcard $(SRCDIR)/*.c $(SRCDIR)/contrib/*.c)

# Generate object files
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(CPP_SOURCES)) \
           $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(C_SOURCES))

# Create directory for objects
OBJECT_DIRS := $(sort $(dir $(OBJECTS)))

# Target executable
TARGET = gslnx

all: check-deps $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

# Create object directories
$(OBJDIR):
	@mkdir -p $(OBJECT_DIRS)

# Compile C++ files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile C files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning build files..."
	@rm -rf $(OBJDIR)
	@rm -f $(TARGET)
	@echo "Clean complete."

install: $(TARGET)
	@echo "Installing Gravity Strike..."
	@mkdir -p $(BINDIR) $(DATADIR)
	@cp $(TARGET) $(BINDIR)/
	@cp -r dat/ $(DATADIR)/
	@cp -r levels/ $(DATADIR)/
	@cp -r ships/ $(DATADIR)/
	@cp -r hiscores/ $(DATADIR)/
	@chmod +x $(BINDIR)/$(TARGET)
	@echo "Installation complete. Run: $(BINDIR)/$(TARGET)"

uninstall:
	@echo "Uninstalling Gravity Strike..."
	@rm -f $(BINDIR)/$(TARGET)
	@rm -rf $(DATADIR)
	@echo "Uninstallation complete."

# Create distribution archive
dist:
	@echo "Creating distribution archive..."
	@tar -czf gravity-strike-$(shell date +%Y%m%d).tar.gz \
		--exclude='.git' \
		--exclude='*.tar.gz' \
		--exclude='*.o' \
		--exclude='obj' \
		--exclude='gslnx' \
		$(SRCDIR) dat/ levels/ ships/ hiscores/ doc/ fixunix.sh gssrc_*.txt keyboard.dat

# Build the original way for compatibility
build-legacy:
	@echo "Building using legacy Makefile..."
	@$(MAKE) -C $(SRCDIR)

# Target for checking dependencies
check-deps:
	@echo "Checking dependencies..."
	@which g++ > /dev/null || (echo "ERROR: g++ compiler not found" && exit 1)
	@which gcc > /dev/null || (echo "ERROR: gcc compiler not found" && exit 1)
	@which allegro-config > /dev/null || (echo "ERROR: Allegro development libraries not found. Install liballegro4-dev or allegro-devel." && exit 1)
	@which lua > /dev/null || (echo "ERROR: Lua interpreter not found. Install liblua5.3-dev or lua-devel." && exit 1)
	@echo "All required dependencies found."

help:
	@echo "Gravity Strike Makefile targets:"
	@echo "  all         - Build the game (default)"
	@echo "  clean       - Remove build files"
	@echo "  install     - Install the game to system"
	@echo "  uninstall   - Remove the game from system"
	@echo "  dist        - Create distribution archive"
	@echo "  build-legacy - Build using the original Makefile"
	@echo "  check-deps  - Verify build dependencies"
	@echo "  help        - Show this help message"