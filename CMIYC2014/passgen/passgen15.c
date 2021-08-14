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
 * name...: passgen15.c
 * author.: atom
 * use....: two input base words, use first 3 char from each and join, then apply rule "TX $Y $Z" where X=[012345], Y=?d, Z=?s
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

    char *word1 = strtok (buf,  "\t");
    char *word2 = strtok (NULL, "\t");

    if (word1 == NULL) continue;
    if (word2 == NULL) continue;

    const int len1 = strlen (word1);
    const int len2 = strlen (word2);

    if (len1 < 3) continue;
    if (len2 < 3) continue;

    out[0] = word1[0];
    out[1] = word1[1];
    out[2] = word1[2];
    out[3] = word2[0];
    out[4] = word2[1];
    out[5] = word2[2];

    MD5_CTX ctx;

    unsigned char md[MD5_DIGEST_LENGTH];

    MD5_Init   (&ctx);
    MD5_Update (&ctx, out, 6);
    MD5_Final  (md, &ctx);

    static uint8_t digits[] =
    {
      0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
      0x38, 0x39,
    };

    static uint8_t symbols[] =
    {
      0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
      0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
      0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x5b,
      0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x7b, 0x7c, 0x7d,
      0x7e
    };

    out[6] = digits[md[0] % sizeof (digits)];
    out[7] = symbols[md[1] % sizeof (symbols)];

    const uint up_idx = md[0] % 6;

    out[up_idx] = toupper (out[up_idx]);

    // printf ("%-20s %-20s -> %s\n", word1, word2, out);
    printf ("%s\n", out);
  }

  return 0;
}
