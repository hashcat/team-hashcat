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
 * name...: passgen04.c
 * author.: atom
 * use....: one input base words, permutes the word x times. x = product of all ascii values of the chars in the word
 */

static uint32_t calc_N (const int len)
{
  uint32_t r = len;

  int i;

  for (i = 1; i < len; i++)
  {
    const uint m = len - i;

    uint64_t r64 = (uint64_t) r * m;

    if (r64 > 0x7fffffff) break;

    r *= m;
  }

  return r;
}

static int next_permutation (char *word, int *p, int k)
{
  p[k]--;

  int j = k % 2 * p[k];

  char tmp = word[j];

  word[j] = word[k];

  word[k] = tmp;

  for (k = 1; p[k] == 0; k++) p[k] = k;

  return k;
}

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

    /* init permutation */

    int p[BUFSIZ];

    int k;

    for (k = 0; k < len + 1; k++) p[k] = k;

    k = 1;

    /* permute as often as md5 digest is */

    uint32_t N = 1;

    uint32_t i;

    for (i = 0; i < (uint32_t) len; i++)
    {
      N *= buf[i];
    }

    N %= calc_N (len);

    for (i = 0; i < N; i++)
    {
      k = next_permutation (out, p, k);
    }

    // printf ("%-20s -> %d, %s\n", buf, N, out);
    printf ("%s\n", out);
  }

  return 0;
}
