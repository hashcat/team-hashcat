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
 * name...: passgen13.c
 * author.: atom
 * use....: one input base words, us keyboard vertical replace, 1 becomes q, q becomes a, a becomes z, etc
 */

char sbox[256] =
{
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
  'p', 'q', 'w', 'e', 'r', 't', 'y', 'u',
  'i', 'o',   0,   0,   0,   0,   0,   0,
    0, 'Z', '5', '3', 'C', 'D', 'V', 'B',
  'N', 'K', 'M', '8', '9', '7', '6', 'L',
  '0', 'A', 'F', 'X', 'G', 'J', '4', 'S',
  '2', 'H', '1',   0,   0,   0,   0,   0,
    0, 'z', '5', '3', 'c', 'd', 'v', 'b',
  'n', 'k', 'm', '8', '9', '7', '6', 'l',
  '0', 'a', 'f', 'x', 'g', 'j', '4', 's',
  '2', 'h', '1',   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
};

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
      uint8_t idx = out[i];

      out[i] = sbox[idx];
    }

    // printf ("%-20s -> %s\n", buf, out);
    printf ("%s\n", out);
  }

  return 0;
}
