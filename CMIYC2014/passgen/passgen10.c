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
 * name...: passgen10.c
 * author.: atom
 * use....: one input base words, rotate consonants a->e, e->i, etc
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

    int i;

    for (i = 0; i < len; i++)
    {
      char c = out[i];

      if      (c == 'a') c = 'e';
      else if (c == 'e') c = 'i';
      else if (c == 'i') c = 'o';
      else if (c == 'o') c = 'u';
      else if (c == 'u') c = 'a';

      out[i] = c;
    }

    // printf ("%-20s -> %s\n", buf, out);
    printf ("%s\n", out);
  }

  return 0;
}
