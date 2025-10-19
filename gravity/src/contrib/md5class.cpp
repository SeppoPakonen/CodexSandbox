#include "md5.h"
#include "md5class.h"
#include <stdio.h>
#include <string.h>
#include "allegro5_wrapper.h"

gsMD5::gsMD5()
{
}

gsMD5::~gsMD5()
{
}

char *gsMD5::get_md5(char *buf, int bufsz, char *md5buf)
{
  unsigned char sig[16];

  md5_state_t md5c;
  md5_init(&md5c);
  md5_append(&md5c, (unsigned char*)buf, bufsz);
  md5_finish(&md5c, sig);

  for (int i=0; (unsigned)i < sizeof(sig); i++)
  {
    char tempnum[5];
    sprintf(tempnum, "%02X", sig[i]);
    strcat(md5buf, tempnum);
  }

  return md5buf;
}

char *gsMD5::get_md5_from_file(char *filename, char *md5buf, int textmode)
{
  unsigned char sig[16];
  unsigned char buffer[16384];
  FILE *DAT;
  strcpy(md5buf, "");

  md5_state_t md5c;
  md5_init(&md5c);

  if (!exists(filename)) return "";

  if (textmode)
    DAT = fopen(filename, "rt");
  else
    DAT = fopen(filename, "rb");

  if (!DAT) return "";

  int i;
  while ((i = fread(buffer, 1, sizeof(buffer), DAT)) > 0)
  {
    md5_append(&md5c, buffer, (unsigned)i);
  }

  fclose(DAT);

  md5_finish(&md5c, sig);

  strcpy(md5buf, "");
  for (i=0; (unsigned)i < sizeof(sig); i++)
  {
    char tempnum[5];
    sprintf(tempnum, "%02X", sig[i]);
    strcat(md5buf, tempnum);
  }

  return md5buf;
}


