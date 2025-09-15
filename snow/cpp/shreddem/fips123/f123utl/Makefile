 
INCLUDES = -I../f123inc

# define salient files for this directory

FILES = attrdump		\
	attrinfo		\
	linearc			\
	linedump		\
	polyarc			\
	ptarc			\
	sdtsdump		\
	show173			\
	show8211		\
	sum173			\
	wrddf


# define source code, header, and object file names

SRCS = $(addsuffix .c,  $(FILES) )
OBJS = $(addsuffix .o,  $(FILES) )


# include common variables and production rules

include ../common.mk


# the default test PDF dataset


#
# local production rules
#

# implicit rule for building test binaries

%.o : %.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

# general static pattern rule to build one or more test files

$(FILES) : % : %.o
	$(CC) $(INCLUDES) $< ../$(FIPSLIB) $(LIBS) -o $@

.PHONY: all
all	: $(FILES)

clean:
	rm -f $(FILES)
	rm -f *.o

install:
	$(INSTALL) $(FILES) $(bindir)
