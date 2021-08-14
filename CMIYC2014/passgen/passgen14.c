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
 * name...: passgen14.c
 * author.: atom
 * use....: one input base words, classic caesar algorithm
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
    if (len < 5) continue;

    int i;

    for (i = 0; i < len; i++)
    {
      out[i] = 'a' + ((buf[i] - 'a' + len) % 26);
    }

    out[i] = 0;

    char *out_off = out + (out[0] & 3) + 1;

    // printf ("%-20s -> %s\n", buf, out);
    printf ("%s\n", out_off);
  }

  return 0;
}
