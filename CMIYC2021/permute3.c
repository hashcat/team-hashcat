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
#include <stdlib.h>
#include <string.h>

/**
 * Name........: permute3
 * Autor.......: Jens Steube <jens.steube@gmail.com>
 * License.....: MIT
 * Credits.....: This program is using the awesome "Countdown QuickPerm Algorithm" developed by Phillip Paul Fuchs
 * Compile.....: gcc -O2 -Wall -o permute3 permute3.c
 */

#define OUT_SIZE   8192

typedef struct out
{
  FILE *fp;

  char  buf[OUT_SIZE];
  int   len;

} out_t;

static void out_flush (out_t *out)
{
  if (out->len == 0) return;

  fwrite (out->buf, 1, out->len, out->fp);

  out->len = 0;
}

static void out_push (out_t *out, const char *pw_buf, const int pw_len)
{
  char *ptr = out->buf + out->len;

  memcpy (ptr, pw_buf, pw_len);

  #if defined (_WIN)

  ptr[pw_len + 0] = '\r';
  ptr[pw_len + 1] = '\n';

  out->len += pw_len + 2;

  #else

  ptr[pw_len] = '\n';

  out->len += pw_len + 1;

  #endif

  if (out->len >= OUT_SIZE - 300)
  {
    out_flush (out);
  }
}

int super_chop (char *s, const int len_orig)
{
  int len = len_orig;

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

int fgetl (FILE *fd, const size_t sz, char *buf)
{
  if (feof (fd)) return -1;

  char *s = fgets (buf, sz - 1, fd);

  if (s == NULL) return -1;

  const int len = (const int) strlen (s);

  return super_chop (s, len);
}

typedef struct
{
  char *buf;

  int len;

} word_t;


size_t next_permutation (word_t *word, int *p, int k)
{
  p[k]--;

  int j = k % 2 * p[k];

  char *tmp = word[j].buf;

  word[j].buf = word[k].buf;

  word[k].buf = tmp;

  int tmp2 = word[j].len;

  word[j].len = word[k].len;

  word[k].len = tmp2;

  for (k = 1; p[k] == 0; k++) p[k] = k;

  return k;
}

void print_word (out_t *out, word_t *word_buf, const int word_cnt)
{
  if (word_cnt == 0) return;

  char out_buf[BUFSIZ];

  int out_len = 0;

  int i;

  for (i =0; i<word_cnt - 1; i++)
  {
    memcpy (out_buf + out_len, word_buf[i].buf, word_buf[i].len);

    out_len += word_buf[i].len;

    out_buf[out_len] = ' ';

    out_len += 1;
  }

  memcpy (out_buf + out_len, word_buf[i].buf, word_buf[i].len);

  out_len += word_buf[i].len;

  out_buf[out_len] = 0;

  out_push (out, out_buf, out_len);
}

void do_permutations (out_t *out, word_t *word_buf, const int word_cnt)
{
  /* init permutation */

  int p[BUFSIZ];

  int k;

  for (k = 0; k < word_cnt + 1; k++) p[k] = k;

  k = 1;

  /* run permutation */

  print_word (out, word_buf, word_cnt);

  while ((k = next_permutation (word_buf, p, k)) != word_cnt) print_word (out, word_buf, word_cnt);

  print_word (out, word_buf, word_cnt);

  out_flush (out);
}

int main (int argc, char *argv[])
{
  if (argc != 1)
  {
    fprintf (stderr, "usage: %s < infile > outfile\n", argv[0]);

    return (-1);
  }

  #ifdef _WINDOWS
  _setmode (_fileno (stdin), _O_BINARY);
  #endif

  out_t out;

  out.fp  = stdout;
  out.len = 0;

  char line_buf[BUFSIZ];

  word_t word_buf[BUFSIZ];

  for (int i=0;i<BUFSIZ; i++)
  {
    word_buf[i].buf = (char *) malloc (BUFSIZ);

    word_buf[i].len = 0;
  }

  int word_cnt;

  int line_len;

  while ((line_len = fgetl (stdin, BUFSIZ, line_buf)) != -1)
  {
    if (line_len == 0) continue;

    word_cnt = 0;

    word_buf[word_cnt].len = 0;

    for (int i = 0; i < line_len; i++)
    {
      if (line_buf[i] != ' ')
      {
        word_buf[word_cnt].buf[word_buf[word_cnt].len] = line_buf[i];

        word_buf[word_cnt].len++;
      }
      else
      {
        word_cnt++;

        word_buf[word_cnt].len = 0;
      }
    }

    word_cnt++;

    do_permutations (&out, word_buf, word_cnt);
  }

  return (0);
}
