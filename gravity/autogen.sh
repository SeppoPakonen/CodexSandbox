#!/bin/sh
# Script to generate the configure script using autotools

set -e

echo "Running autoreconf to generate configure script..."
autoreconf --install --verbose

echo "Autotools setup complete. You can now run:"
echo "  ./configure"
echo "  make"
echo "  make install"