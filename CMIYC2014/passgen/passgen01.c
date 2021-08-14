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
 * name...: passgen01.c
 * author.: atom
 * use....: one input base words, appends first 4 byte of cryptographical weak digest encoded with KL charset
 */

#define MANGLE_LENGTH 4

static void fix_charset (uint8_t *s, int len)
{
  static uint8_t korelogic_charset[95] =
  {
    // a-z
    0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
    0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
    0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
    0x79, 0x7a,

    // A-Z
    0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
    0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
    0x59, 0x5a,

    // 0-9
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39,

    // Symbols
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
    0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x5b,
    0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x7b, 0x7c, 0x7d,
    0x7e
  };

  int i;

  for (i = 0; i < len; i++)
  {
    const uint8_t idx = s[i] % sizeof (korelogic_charset);

    s[i] = korelogic_charset[idx];
  }

  s[i] = 0;
}

static void crap_hash (char *buf, const int len, unsigned char *md)
{
  int s = len * 11;

  int i;

  for (i = 0; i < len; i++)
  {
    const char c = buf[i];

    s += i * 13;
    s += c * 17;
  }

  for (i = 0; i < len; i++)
  {
    const char c = buf[i];

    s += i * 19;
    s += c * 23;

    md[i] = s;
  }
}

int main (int argc, char *argv[])
{
  if (argc != 1)
  {
    fprintf (stderr, "usage: %s < words\n", argv[0]);

    return (-1);
  }

  unsigned char md[BUFSIZ];

  char buf[BUFSIZ];

  int len;

  while ((len = fgetl (stdin, sizeof (buf), buf)) != -1)
  {
    if (len == 0) continue;

    crap_hash (buf, len, md);

    fix_charset (md, MANGLE_LENGTH);

    // printf ("%-20s -> %s%s\n", buf, buf, md);
    printf ("%s%s\n", buf, md);
  }

  return 0;
}
