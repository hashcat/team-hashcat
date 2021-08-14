#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64
#define __MSVCRT_VERSION__ 0x0700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include "utils.c"

/**
 * name...: passgen12.c
 * author.: atom
 * use....: one input base words, simply mirror in the middle: password -> wordpass
 */

int main (int argc, char *argv[])
{
  if (argc != 1)
  {
    fprintf (stderr, "usage: %s < words\n", argv[0]);

    return (-1);
  }

  char out[BUFSIZ];
  char buf[BUFSIZ];

  int len;

  while ((len = fgetl (stdin, sizeof (buf), buf)) != -1)
  {
    if (len == 0) continue;

    strcpy (out, buf);

    int half = len / 2;

    int offl = 0;
    int offr = half;

    if (len & 1)
    {
      offr++;

      out[offr] = buf[offr];
    }

    memcpy (out + offl, buf + offr, half);
    memcpy (out + offr, buf + offl, half);

    // printf ("%-20s -> %s\n", buf, out);
    printf ("%s\n", out);
  }

  return 0;
}
