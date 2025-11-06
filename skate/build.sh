#!/bin/bash

# Compile using U++ umk (ultimate++ make)
umk uppsrc,$HOME/ai-upp/uppsrc Bruce ~/.config/u++/theide/CLANG.bm -Sdbs +EMU,SDL,HAVE_DIRECT_OPENGL,DEBUG_FULL ~/bin/bruce

echo Run main executable
echo ~/bin/bruce

