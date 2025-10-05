/*
 *   GRAVITY STRIKE -- gshiscor.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _GSHISCORE_
#define _GSHISCORE_

class gsHiscore
{
  private:
    char fakestring[255];
    char crc_file_crc[100];

    long int get_full_time(char *val);

    static void save_checksum(const char *fname, int attrib, int param);

  public:
    gsHiscore();
    ~gsHiscore();

    int save_hiscores(char *retscore);
    char *decode_string(char *s, int minus=1);

    void make_crc_file();
    int do_crc_checks(char *dir=NULL);
    void crc_failed_for_level(char *l);
    int check_crc_failed_for_current_level();
};

extern gsHiscore *hiscore;

#endif

