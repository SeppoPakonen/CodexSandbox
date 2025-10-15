#!/bin/bash

# Setup script for Gravity Strike

echo "Setting up Gravity Strike build environment..."

# Run fixunix.sh to ensure proper line endings
echo "Converting line endings with fixunix.sh..."
sh ./fixunix.sh

# Show help for building
echo ""
echo "Setup complete! You can now build Gravity Strike."
echo ""
echo "To build the game:"
echo "  make all"
echo ""
echo "For help with other options:"
echo "  make help"
echo ""
echo "To install system-wide:"
echo "  make install"
echo ""
echo "Note: You'll need the required dependencies installed (see README.md)."