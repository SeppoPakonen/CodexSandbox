# Gravity Strike -- Linux/Unix Makefile
# Jens Hassler, October 2002
# Updated for modern systems

# Flags
CXX = g++
CC = gcc
CXXFLAGS ?= -O2 -Wall -std=c++11
CFLAGS ?= -O2 -Wall -std=c11
LDFLAGS = -ljgmod -lalttf -llua -llualib -ltolua -lnet `allegro-config --libs`

# Object directory (create if it doesn't exist)
OBJDIR = obj
SRCDIR = .

# Find all source files recursively
SRCS_CPP = $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/menu/*.cpp $(SRCDIR)/enemies/*.cpp $(SRCDIR)/weapons/*.cpp $(SRCDIR)/contrib/*.cpp)
SRCS_C = $(wildcard $(SRCDIR)/*.c $(SRCDIR)/contrib/*.c)
CSRCS = mappyal.c contrib/md5.c

# Generate object files
OBJS_CPP = $(SRCS_CPP:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
OBJS_C = $(SRCS_C:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
COBJS = $(CSRCS:%.c=$(OBJDIR)/%.o)

# Combined object files
OBJS = $(OBJS_CPP) $(OBJS_C) $(COBJS)

GSEXE = ../gslnx

RM = /bin/rm -f


# Make sure object directory exists
$(OBJDIR):
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OBJDIR)/menu $(OBJDIR)/enemies $(OBJDIR)/weapons $(OBJDIR)/contrib

$(GSEXE): $(OBJDIR) $(OBJS)
	@echo "Linking $(GSEXE)..."
	$(CXX) $(OBJS) -o $(GSEXE) $(LDFLAGS)
	@echo "Build complete: $(GSEXE)"

# Pattern rules for compiling C++ files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Pattern rules for compiling C files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Specific rule for the main executable dependencies
$(OBJDIR)/gsmain.o: $(SRCDIR)/gsmain.cpp | $(OBJDIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Cleaning build files..."
	$(RM) -r $(OBJDIR)
	@echo "Clean complete."

# Install target
install: $(GSEXE)
	cp $(GSEXE) /usr/local/bin/

.PHONY: clean install