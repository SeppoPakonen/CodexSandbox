#!/bin/bash

# Gravity Strike Build Script
# This script builds the project and prevents make clean from removing it
# Usage: ./build.sh [options]

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if we're in the right directory
if [ ! -f "Makefile" ] && [ ! -f "configure" ]; then
    print_error "Makefile or configure script not found in current directory!"
    exit 1
fi

# Function to check dependencies
check_dependencies() {
    print_status "Checking dependencies..."
    
    # Check for required tools
    local deps=("g++" "gcc" "make")
    for dep in "${deps[@]}"; do
        if ! command -v "$dep" &> /dev/null; then
            print_error "$dep is not installed. Please install build essentials."
            exit 1
        fi
    done
    
    # Check for Allegro 5
    if ! command -v allegro5-config &> /dev/null && ! pkg-config allegro-5 --exists 2>/dev/null; then
        print_warning "Allegro 5 development libraries not found via standard tools."
        print_status "Checking for alternative Allegro configuration..."
        if ! command -v allegro-config &> /dev/null; then
            print_error "Allegro 5 development libraries not found. Please install them."
            print_error "On Debian/Ubuntu: sudo apt-get install liballegro5-dev liballegro-image5-dev"
            print_error "On Fedora/RHEL: sudo dnf install allegro5-devel"
            exit 1
        fi
    fi
    
    # Check for Lua
    if ! command -v lua &> /dev/null; then
        print_error "Lua is not installed. Please install Lua development libraries."
        exit 1
    fi
    
    print_success "All dependencies found!"
}

# Function to build the project
build_project() {
    print_status "Building Gravity Strike..."
    
    # If Makefile exists, use it
    if [ -f "Makefile" ]; then
        make all
        print_success "Build completed successfully!"
    else
        print_error "Makefile not found!"
        exit 1
    fi
}

# Function to clean temporary files (autotools related)
clean_temp_files() {
    print_status "Cleaning temporary autotools files..."
    
    # Remove autotools generated files (but not build.sh)
    local temp_dirs=(".deps" "autom4te.cache" ".libs" "_build" "build-aux")
    local temp_files=(".DS_Store" "*.la" "*.lo" "libtool" "config.log" "config.status" "Makefile.in" "aclocal.m4")
    
    for dir in "${temp_dirs[@]}"; do
        if [ -d "$dir" ]; then
            rm -rf "$dir"
            print_status "Removed directory: $dir"
        fi
    done
    
    for file in "${temp_files[@]}"; do
        for f in $file; do
            if [ -e "$f" ]; then
                rm -f "$f"
                print_status "Removed file: $f"
            fi
        done
    done
    
    # Clean any additional autotools generated files
    find . -name "Makefile.in" -type f -delete
    find . -name "*.tar.gz" -type f -delete
    
    print_success "Temporary files cleaned!"
}

# Function to display usage
show_help() {
    echo "Gravity Strike Build Script"
    echo ""
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  build        - Build the project (default)"
    echo "  clean        - Clean temporary autotools files (does not remove build.sh)"
    echo "  deps         - Check dependencies only"
    echo "  install      - Build and install the project"
    echo "  help         - Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0           - Build the project"
    echo "  $0 build     - Build the project"
    echo "  $0 clean     - Clean temporary files"
    echo "  $0 deps      - Check dependencies"
}

# Main script logic
case "${1:-build}" in
    build)
        check_dependencies
        build_project
        ;;
    clean)
        clean_temp_files
        ;;
    deps)
        check_dependencies
        ;;
    install)
        check_dependencies
        build_project
        make install
        ;;
    help|--help|-h)
        show_help
        ;;
    *)
        print_error "Unknown option: $1"
        show_help
        exit 1
        ;;
esac
