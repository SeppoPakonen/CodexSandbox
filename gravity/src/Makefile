# Gravity Strike -- Linux/Unix Makefile
# Jens Hassler, October 2002

# Flags
CC = gcc
LD = g++
CFLAGS = -o2 -Wall
LDFLAGS = -ljgmod -lalttf -llua -llualib -ltolua -lnet `allegro-config --libs`

OBJDIR = obj
SRCS = $(wildcard *.cpp) $(wildcard menu/*.cpp) $(wildcard menu/controls/*.cpp) $(wildcard enemies/*.cpp) $(wildcard weapons/*.cpp) $(wildcard contrib/*.cpp)
CSRCS = mappyal.c contrib/md5.c
OBJS = $(addprefix obj/, $(addsuffix .o, $(basename $(SRCS))))
COBJS = $(addprefix obj/, $(addsuffix .o, $(basename $(CSRCS))))

GSEXE = ../gslnx

RM = /bin/rm -f


$(GSEXE): $(OBJS) $(COBJS)
	$(LD) $(OBJS) $(COBJS) -o $(GSEXE) $(LDFLAGS)

$(OBJS): $(subst .o,.cpp, $(subst $(OBJDIR)/,,$@))
	$(CC) $(CFLAGS) -o $@ -c $(subst .o,.cpp, $(subst $(OBJDIR)/,,$@))

$(COBJS): $(subst .o,.c, $(subst $(OBJDIR)/,,$@))
	$(CC) $(CFLAGS) -o $@ -c $(subst .o,.c, $(subst $(OBJDIR)/,,$@))

clean:
	$(RM) $(OBJS) $(COBJS)
