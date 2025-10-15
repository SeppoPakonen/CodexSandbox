#!/bin/bash

# Script to package Gravity Strike for distribution
# Creates a tarball with all necessary files

set -e

PROJECT_NAME="gravity-strike"
VERSION=$(date +%Y%m%d)
PACKAGE_NAME="${PROJECT_NAME}-${VERSION}"
TARBALL_NAME="${PACKAGE_NAME}.tar.gz"

echo "Creating distribution package: ${TARBALL_NAME}"

# Create temporary directory for packaging
TEMP_DIR=$(mktemp -d)

# Copy all necessary files to temp directory
cp -r src dat levels ships hiscores doc fixunix.sh gssrc_en.txt gssrc_de.txt keyboard.dat Makefile configure.ac autogen.sh README.md $TEMP_DIR/ 2>/dev/null || true

# Create the package
cd $TEMP_DIR
tar -czf "../${TARBALL_NAME}" --exclude='.git' --exclude-vcs-ignores .
cd ..

# Cleanup
rm -rf $TEMP_DIR

echo "Package created: ${TARBALL_NAME}"
echo "Size: $(ls -lh ${TARBALL_NAME} | awk '{print $5}')"

echo ""
echo "To build from the package:"
echo "  tar -xzf ${TARBALL_NAME}"
echo "  cd ${PACKAGE_NAME}"
echo "  ./autogen.sh" 
echo "  ./configure"
echo "  make"