#
# $Id: common.mk,v 1.1 2003/01/20 16:34:24 tulrich Exp $
# common makefile rules and variables

SHELL	= /bin/sh
RANLIB  = ranlib

# Compiler and flags
CC	   = gcc
CFLAGS	   = -O2 -ansi
LIBS	   = -lm 
FIPSLIB = fips123.a
INSTALL = $(INSTALL)

# Define .c .h as valid filename suffixes for old make utilities
.SUFFIXES: .c .h

# please note that the following implicit rule breaks most old make utilities
%.o : %.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $<



# Define repository object files for cfront-based C++ compilers
REPOSITORY_OBJ_FILES = 



# common production rules

.PHONY: all utl app depend clean realclean install

all: $(OBJS) ../$(FIPSLIB)
 
utl: 
	$(FILES)

app:
	$(FILES)

depend: $(SRCS)
	makedepend $(DEFS) $(INCLUDES) $(SRCS)

clean    :  

realclean: clean

install :
