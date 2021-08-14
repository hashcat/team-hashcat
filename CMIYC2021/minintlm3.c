#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64
#define __MSVCRT_VERSION__ 0x0700

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <iconv.h>
#include <openssl/md4.h>

/**
 * Name........: minintlm3.c
 * Autor.......: Jens Steube <jens.steube@gmail.com>
 * License.....: MIT
 * Compile.....: gcc -o minintlm3 minintlm3.c -O2 -Wall -lcrypto
 */

typedef uint8_t  u8;
typedef uint32_t u32;
typedef uint64_t u64;

typedef struct
{
  u32 w0;
  u32 w1;
  u32 w2;
  u32 w3;

  int cracked;

} hash_t;


typedef struct
{
  char *buf;

  int len;

} word_t;

static int cmp (const void *m1, const void *m2)
{
  hash_t *mi1 = (hash_t *) m1;
  hash_t *mi2 = (hash_t *) m2;

  if (mi1->w0 > mi2->w0) return  1;
  if (mi1->w0 < mi2->w0) return -1;
  if (mi1->w1 > mi2->w1) return  1;
  if (mi1->w1 < mi2->w1) return -1;
  if (mi1->w2 > mi2->w2) return  1;
  if (mi1->w2 < mi2->w2) return -1;
  if (mi1->w3 > mi2->w3) return  1;
  if (mi1->w3 < mi2->w3) return -1;

  return 0;
}

static size_t super_chop (char *s, size_t len)
{
  char *p = s + len - 1;

  while (len)
  {
    if (*p != '\n') break;

    *p-- = 0;

    len--;
  }

  while (len)
  {
    if (*p != '\r') break;

    *p-- = 0;

    len--;
  }

  return len;
}

static int fgetl (FILE *stream, size_t sz, char *buf)
{
  if (feof (stream)) return -1;

  char *s = fgets (buf, sz, stream);

  if (s == NULL) return -1;

  size_t len = strlen (s);

  len = super_chop (s, len);

  return len;
}

#define COMBS       5
#define MAX_HASHES  100000
#define MAX_WORDS   128

int main (int argc, char *argv[])
{
  // check command line

  if (argc != 3)
  {
    fprintf (stderr, "usage: %s hashlist outfile < infile\n", argv[0]);

    return (-1);
  }

  // iconv for md4 to ntlm

  iconv_t conv = iconv_open ("UTF-16LE", "UTF-8");

  if (conv == (iconv_t) -1)
  {
    perror ("iconv_open");

    return 1;
  }

  // outfile

  FILE *fd2 = fopen (argv[2], "w");

  if (fd2 == NULL)
  {
    perror ("fopen2");

    return 1;
  }

  // hashlist

  FILE *fd = fopen (argv[1], "r");

  if (fd == NULL)
  {
    perror ("fopen1");

    return 1;
  }

  // load hashlist

  hash_t *hashes_buf = (hash_t *) calloc (MAX_HASHES, sizeof (hash_t));

  int hashes_cnt = 0;

  while (!feof (fd))
  {
    u32 w0;
    u32 w1;
    u32 w2;
    u32 w3;

    const int nread = fscanf (fd, "%08x%08x%08x%08x\n", &w0, &w1, &w2, &w3);

    if (nread != 4) continue;

    // also store values byte swapped so we don't need to swap them in inner loop

    hashes_buf[hashes_cnt].w0 = __builtin_bswap32 (w0);
    hashes_buf[hashes_cnt].w1 = __builtin_bswap32 (w1);
    hashes_buf[hashes_cnt].w2 = __builtin_bswap32 (w2);
    hashes_buf[hashes_cnt].w3 = __builtin_bswap32 (w3);

    hashes_buf[hashes_cnt].cracked = 0;

    hashes_cnt++;
  }

  fclose (fd);

  // sort hashes, make sure input hashlist is already unique otherwise problems

  qsort (hashes_buf, hashes_cnt, sizeof (hash_t), cmp);

  int line_len;

  char line_buf[BUFSIZ];

  while ((line_len = fgetl (stdin, BUFSIZ, line_buf)) != -1)
  {
    if (line_len == 0) continue;

    // password candidate is ready, but this is ntlm, so convert it from 8 bit format to utf16le

    char iconv_buf[BUFSIZ*2];

    char *inx = line_buf;
    char *outx = iconv_buf;

    size_t inbytes = line_len;
    size_t outbytes = sizeof (iconv_buf);

    iconv (conv, &inx, &inbytes, &outx, &outbytes);

    // now hash it using md4

    MD4_CTX c;

    MD4_Init (&c);

    MD4_Update (&c, iconv_buf, sizeof (iconv_buf) - outbytes);

    u32 md[4];

    MD4_Final ((unsigned char *) md, &c);

    // search the hash in the database

    hash_t s;

    s.w0 = md[0];
    s.w1 = md[1];
    s.w2 = md[2];
    s.w3 = md[3];

    hash_t *hptr = (hash_t *) bsearch (&s, hashes_buf, hashes_cnt, sizeof (hash_t), cmp);

    if (hptr == NULL) continue;

    // hash was found, show it to the user

    if (hptr->cracked == 1) continue;

    hptr->cracked = 1;

    fprintf (fd2, "%08x%08x%08x%08x:%s\n",
      __builtin_bswap32 (md[0]),
      __builtin_bswap32 (md[1]),
      __builtin_bswap32 (md[2]),
      __builtin_bswap32 (md[3]),
      line_buf);

    fflush (fd2);
  }


  iconv_close (conv);

  fclose (fd2);

  return (0);
}
