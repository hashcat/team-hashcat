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
#include <openssl/md5.h>

/**
 * name...: passgen07.c
 * author.: atom
 * use....: same as atom-5 but encoded with charset limited to a-z only
 */

#define MANGLE_LENGTH 4

static void fix_charset (uint8_t *s, int len)
{
  static uint8_t korelogic_charset[26] =
  {
    // a-z
    0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
    0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
    0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
    0x79, 0x7a
  };

  int i;

  for (i = 0; i < len; i++)
  {
    const uint8_t idx = s[i] % sizeof (korelogic_charset);

    s[i] = korelogic_charset[idx];
  }

  s[i] = 0;
}

int main (int argc, char *argv[])
{
  if (argc != 1)
  {
    fprintf (stderr, "usage: %s < words\n", argv[0]);

    return (-1);
  }

  MD5_CTX ctx;

  unsigned char md[MD5_DIGEST_LENGTH];

  char out[BUFSIZ];
  char buf[BUFSIZ];

  int len;

  while ((len = fgetl (stdin, sizeof (buf), buf)) != -1)
  {
    if (len == 0) continue;

    MD5_Init   (&ctx);
    MD5_Update (&ctx, buf, len);
    MD5_Final  (md, &ctx);

    fix_charset (md, MANGLE_LENGTH);

    char *ptr = out;

    int i;

    for (i = 0; i < len; i++)
    {
      const char c = buf[i];
      const char t = md[i];

      if (i < MANGLE_LENGTH)
      {
        *ptr++ = c;
        *ptr++ = t;
      }
      else
      {
        *ptr++ = c;
      }
    }

    *ptr++ = 0;

    // printf ("%-20s -> %s\n", buf, out);
    printf ("%s\n", out);
  }

  return 0;
}
