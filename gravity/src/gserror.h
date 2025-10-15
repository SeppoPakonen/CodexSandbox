/*
 *   GRAVITY STRIKE -- gserror.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include <stdio.h>
#include "allegro5_wrapper.h"

#ifndef _GSERROR_
#define _GSERROR_

class gsError
{
  private:
    FILE         *logfile;
    int          loglevel;
    char         timestamp[30];

    void         get_timestamp(char *);

  public:
    gsError();
    ~gsError();

    void open_log();
    void close_log();

    // first int = facility: 0=error, 1=warn, 2=info
    void log(int, char *, char *, long);
    void log(int, char *, char *, char *);
    void log(int, char *, char *, char *, long);
    void log(int, char *, char *);
    void log(int, char *, float, float=0);
    void log(int, char *);

    void er_exit(char *, int n);
};

extern gsError *errors;

#endif

