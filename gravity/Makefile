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

# Try to determine Allegro 5 configuration method
ifeq ($(shell which allegro5-config 2>/dev/null),)
    ifneq ($(shell pkg-config allegro5 --libs 2>/dev/null),)
        # Try the Allegro 5 specific pkg-config
        ALLEGRO_LIBS := $(shell pkg-config allegro5 --libs)
        ALLEGRO_CFLAGS := $(shell pkg-config allegro5 --cflags)
    else
        # Try standard allegro-config for Allegro 5 if nothing else works
        ifneq ($(shell which allegro-config 2>/dev/null),)
            ALLEGRO_LIBS := $(shell allegro-config --libs)
            ALLEGRO_CFLAGS := $(shell allegro-config --cflags)
        else
            # Default fallback (this will likely fail but at least the error is cleaner)
            ALLEGRO_LIBS := -lallegro -lallegro_image -lallegro_primitives -lallegro_ttf -lallegro_audio -lallegro_acodec
        endif
    endif
else
    # allegro5-config is available (typical on Debian/Ubuntu)
    ALLEGRO_LIBS := $(shell allegro5-config --libs)
    ALLEGRO_CFLAGS := $(shell allegro5-config --cflags)
endif

LDFLAGS = -ljgmod -lalttf -llua -llualib -ltolua -lnet $(ALLEGRO_LIBS)
CXXFLAGS += $(ALLEGRO_CFLAGS) -I/usr/include/lua5.4
CFLAGS += $(ALLEGRO_CFLAGS) -I/usr/include/lua5.4

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
	@if ! which allegro5-config > /dev/null && ! pkg-config allegro5 --exists 2>/dev/null && ! ldconfig -p 2>/dev/null | grep -q "liballegro.so.5"; then \
		if pkg-config allegro --exists 2>/dev/null; then \
			echo "NOTE: Allegro 4 found but Gravity Strike requires Allegro 5.x for the updated version"; \
			echo "Please install Allegro 5 development libraries:"; \
			echo "  Debian/Ubuntu: sudo apt-get install liballegro5-dev liballegro-image5-dev liballegro-ttf5-dev liballegro-audio5-dev liballegro-acodec5-dev"; \
			echo "  Fedora/RHEL: sudo dnf install allegro5-devel allegro5-image-devel allegro5-ttf-devel allegro5-audio-devel allegro5-acodec-devel"; \
			exit 1; \
		else \
			echo "ERROR: Allegro 5 development libraries not found."; \
			echo "Install liballegro5-dev (Debian/Ubuntu) or allegro5-devel (Fedora/RHEL)"; \
			exit 1; \
		fi \
	fi
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