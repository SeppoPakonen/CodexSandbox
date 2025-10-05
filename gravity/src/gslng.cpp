/*
 *   GRAVITY STRIKE -- gslng.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "gslng.h"
#include "gserror.h"
#include <string.h>

gsLanguage::gsLanguage()
{
  for (int n=0; n < MAX_STRINGS; n++)
    strings[n] = NULL;
}

gsLanguage::~gsLanguage()
{
  for (int n=0; n < MAX_STRINGS; n++)
    if (strings[n]) delete[](strings[n]);
}

void gsLanguage::set_string(TEXT_STRINGS num, char *s)
{
  if (num >= 0 && num < MAX_STRINGS)
  {
    if (strings[num] != NULL) delete[](strings[num]);

    strings[num] = new char[strlen(s)+1];
    strcpy(strings[num], s);
  }
}

char *gsLanguage::get_string(TEXT_STRINGS num)
{
  if (num >= 0 && num < MAX_STRINGS && strings[num] != NULL)
    return strings[num];
  else
    return "n/a";
}

