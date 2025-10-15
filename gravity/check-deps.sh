#!/bin/bash

# Script to check build dependencies for Gravity Strike

echo "Checking build dependencies for Gravity Strike..."

# Check for C++ compiler
if command -v g++ &> /dev/null; then
    echo "✓ g++ compiler: found"
else
    echo "✗ g++ compiler: not found"
    MISSING_DEPS=1
fi

# Check for C compiler
if command -v gcc &> /dev/null; then
    echo "✓ gcc compiler: found"
else
    echo "✗ gcc compiler: not found"
    MISSING_DEPS=1
fi

# Check for Allegro (either via allegro-config or pkg-config for Allegro 4)
if command -v allegro-config &> /dev/null; then
    echo "✓ Allegro library: found (allegro-config)"
elif pkg-config allegro4-config --exists 2>/dev/null; then
    echo "✓ Allegro library: found (pkg-config allegro4-config)"
elif [ -f "/usr/lib64/liballeg-4.2.so" ] || [ -f "/usr/lib/liballeg-4.2.so" ] || [ -f "/usr/lib64/liballeg.so" ] && ldconfig -p | grep -q "liballeg"; then
    echo "✓ Allegro library: found (system library)"
else
    echo "✗ Allegro library: not found (required - Allegro 4.x needed)"
    MISSING_DEPS=1
fi

# Check for Lua
if command -v lua &> /dev/null; then
    echo "✓ Lua interpreter: found"
else
    echo "✗ Lua interpreter: not found"
    MISSING_DEPS=1
fi

# Check for pkg-config
if command -v pkg-config &> /dev/null; then
    echo "✓ pkg-config: found"
else
    echo "✗ pkg-config: not found"
    MISSING_DEPS=1
fi

# Check for specific libraries using pkg-config or other methods
if pkg-config --exists allegro 2>/dev/null; then
    echo "✓ allegro pkg-config: found"
else
    echo "✗ allegro pkg-config: not found"
fi

if pkg-config --exists lua 2>/dev/null; then
    echo "✓ lua pkg-config: found"
else
    echo "✗ lua pkg-config: not found"
fi

if [ "$MISSING_DEPS" = "1" ]; then
    echo ""
    echo "Some required dependencies are missing!"
    echo ""
    echo "On Ubuntu/Debian systems, install with:"
    echo "  sudo apt-get install build-essential liballegro4-dev liballegro-ttf-dev"
    echo "  sudo apt-get install liblua5.3-dev libtolua++5.1-dev libjgmod-dev"
    echo "  sudo apt-get install libnet1-dev"
    echo ""
    echo "On Fedora/RHEL systems, install with:"
    echo "  sudo dnf install gcc gcc-c++ allegro-devel allegro-ttf-devel"
    echo "  sudo dnf install lua-devel tolua++-devel jgmod-devel libnet-devel"
    echo ""
    echo "After installing dependencies, try building again:"
    echo "  make all"
    exit 1
else
    echo ""
    echo "All required dependencies found! You can build the game with:"
    echo "  make all"
fi