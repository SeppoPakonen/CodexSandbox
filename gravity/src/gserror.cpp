/*
 *   GRAVITY STRIKE -- gserror.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include <stdio.h>
#include "gserror.h"
#include "gsglob.h"
#include "time.h"

extern int debuglevel;

gsError::gsError()
{
  strcpy(globals->logfile_name, "gs.log");

  loglevel = debuglevel;

  open_log();
  get_timestamp(timestamp);
  fprintf(logfile, "\n%s\tstarting log...\n", timestamp);
  close_log();
}

gsError::~gsError()
{
  open_log();
  get_timestamp(timestamp);
  fprintf(logfile, "%s\t...end of log.\n", timestamp);
  close_log();
}

void gsError::open_log()
{
  logfile = fopen(globals->logfile_name, "at");
}

void gsError::close_log()
{
  fclose(logfile);
}

void gsError::get_timestamp(char *buffer)
{
  time_t zeit = time(NULL);
  struct tm *kzeit = localtime(&zeit);
  char tstr[21];

  strftime(tstr, 20, "%H:%M:%S", kzeit);
  strcpy(buffer, tstr);
}

void gsError::log(int fac, char *l1, char *l2, long n)
{
  open_log();
  if (fac <= loglevel)
  {
    get_timestamp(timestamp);
    fprintf(logfile, "%s\t%s:\t%s (%ld)\n", timestamp, l1, l2, n);
  }
  close_log();
}

void gsError::log(int fac, char *l1, char *l2)
{
  open_log();
  if (fac <= loglevel)
  {
    get_timestamp(timestamp);
    fprintf(logfile, "%s\t%s:\t'%s'\n", timestamp, l1, l2);
  }
  close_log();
}

void gsError::log(int fac, char *l1, char *l2, char *l3)
{
  open_log();
  if (fac <= loglevel)
  {
    get_timestamp(timestamp);
    fprintf(logfile, "%s\t%s:\t%s (%s)\n", timestamp, l1, l2, l3);
  }
  close_log();
}

void gsError::log(int fac, char *l1, char *l2, char *l3, long l4)
{
  open_log();
  if (fac <= loglevel)
  {
    get_timestamp(timestamp);
    fprintf(logfile, "%s\t%s:\t%s (%s, %ld)\n", timestamp, l1, l2, l3, l4);
  }
  close_log();
}

void gsError::log(int fac, char *l1, float l2, float l3)
{
  open_log();
  if (fac <= loglevel)
  {
    get_timestamp(timestamp);
    fprintf(logfile, "%s\t%s:\t%.6f,%.6f\n", timestamp, l1, l2, l3);
  }
  close_log();
}

void gsError::log(int fac, char *l)
{
  open_log();
  if (fac <= loglevel)
  {
    get_timestamp(timestamp);
    fprintf(logfile, "%s\tdefault:\t%s\n", timestamp, l);
  }
  close_log();
}

void gsError::er_exit(char *t, int n)
{
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  printf("%s (%d)\n", t, n);
  allegro_exit();
  exit(n);
}

