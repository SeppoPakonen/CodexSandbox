#!/bin/bash

# Build:
umk uppsrc,$HOME/ai-upp/uppsrc "${1}" $HOME/.config/u++/theide/CLANG.bm -sdH1 +USEMALLOC,DEBUG_FULL "bin/${1}"

# Run ide:
echo "Executable compiled: bin/${1}"
