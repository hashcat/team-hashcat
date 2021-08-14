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
 * name...: passgen16.c
 * author.: atom
 * use....: two input base words, filter in both char by char, then apply rule "TX ^Y $Y" where X=[1234567], Y=?d
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

    if (len1 != 7) continue;
    if (len2 != 7) continue;

    out[1] = word1[0];
    out[2] = word2[0];
    out[3] = word1[1];
    out[4] = word2[1];
    out[5] = word1[2];
    out[6] = word2[2];
    out[7] = word2[3];

    MD5_CTX ctx;

    unsigned char md[MD5_DIGEST_LENGTH];

    MD5_Init   (&ctx);
    MD5_Update (&ctx, out, 7);
    MD5_Final  (md, &ctx);

    static uint8_t digits[] =
    {
      0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
      0x38, 0x39,
    };

    const uint up_idx = md[0] % 7;

    out[up_idx] = toupper (out[up_idx]);

    out[0] = digits[md[1] % sizeof (digits)];
    out[8] = digits[md[2] % sizeof (digits)];

    // printf ("%-20s %-20s -> %s\n", word1, word2, out);
    printf ("%s\n", out);
  }

  return 0;
}
