#!/bin/bash

# Compile using U++ umk (ultimate++ make)
umk uppsrc,$HOME/ai-upp/uppsrc Bruce CLANG -Sdb +EMU,SDL,HAVE_DIRECT_OPENGL ~/bin/bruce

# Run main executable
~/bin/bruce

