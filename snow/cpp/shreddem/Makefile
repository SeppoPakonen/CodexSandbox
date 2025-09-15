# Copyright (c) 1998 Slingshot Game Technology
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

# Makefile for shreddem terrain preprocessing file.

all : shreddem

# CPP=cl.exe
# CPP_PROJ=/nologo /MLd /Gm /GR /GX /Zi /Od /I ".." /D "WIN32" /D "_DEBUG" /D\
# "_CONSOLE" /D "_MBCS" /D "HAVE_ISO_HEADERS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\"\
# /Fd"$(INTDIR)\\" /FD /c 

OBJS= \
	utm.o \
	dem.o \
	grid.o \
	shreddem.o

INCS= -I fips123/f123inc
CFLAGS= -g $(INCS)

%.o : %.cpp
	g++ -c $(CFLAGS) $<


LIBS= fips123/fips123.a

shreddem : $(OBJS)
	g++ -o shreddem $(OBJS) $(LIBS) -g

clean :
	rm $(OBJS)
	rm shreddem

