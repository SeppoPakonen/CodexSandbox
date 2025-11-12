CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
CXXFLAGS = -Wall -Wextra -std=c++17 -O2

# Libraries
LIBS = -lglfw -lGLEW -lGL -lm -lGLU

# Directories
SRCDIR = src
INCDIR = include
BUILDDIR = build
SHADERDIR = shaders
TARGETDIR = bin

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.c)
SOURCES += $(wildcard $(SRCDIR)/*.cpp)
SOURCES += $(wildcard $(SRCDIR)/*.cc)

# Object files
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
OBJECTS := $(OBJECTS:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
OBJECTS := $(OBJECTS:$(SRCDIR)/%.cc=$(BUILDDIR)/%.o)

# Target executable
TARGET = $(TARGETDIR)/biker2d

# Create directories
$(shell mkdir -p $(BUILDDIR) $(BUILDDIR)/src $(TARGETDIR))

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Linking..."
	@$(CC) $(OBJECTS) -o $@ $(LIBS)
	@echo "Build complete!"

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cc
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	@echo "Cleaning..."
	@rm -rf $(BUILDDIR) $(TARGETDIR)

run: $(TARGET)
	@echo "Running..."
	@./$(TARGET)

# Copy shaders to bin directory
install-shaders:
	@mkdir -p $(TARGETDIR)/shaders
	@cp -r $(SHADERDIR)/* $(TARGETDIR)/shaders/