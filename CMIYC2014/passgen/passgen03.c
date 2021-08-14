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
 * name...: passgen03.c
 * author.: atom
 * use....: one input base words, append sum of telephone countrycodes matching 2-char iso-3166 countrycode, encode with KL charset
 */

static int get_cc_from_ss (char *ss)
{
  /* element of periodic system of elements */
  typedef struct
  {
    const char *iso3166;
    int cc;

  } t_3166cc;

  static t_3166cc table[] =
  {
    { "ax", 248 },
    { "af", 4 },
    { "al", 8 },
    { "dz", 12 },
    { "as", 16 },
    { "ad", 20 },
    { "ao", 24 },
    { "ai", 660 },
    { "aq", 10 },
    { "ag", 28 },
    { "ar", 32 },
    { "am", 51 },
    { "aw", 533 },
    { "au", 36 },
    { "at", 40 },
    { "az", 31 },
    { "bs", 44 },
    { "bh", 48 },
    { "bd", 50 },
    { "bb", 52 },
    { "by", 112 },
    { "be", 56 },
    { "bz", 84 },
    { "bj", 204 },
    { "bm", 60 },
    { "bt", 64 },
    { "bo", 68 },
    { "ba", 70 },
    { "bw", 72 },
    { "bv", 74 },
    { "br", 76 },
    { "io", 86 },
    { "bn", 96 },
    { "bg", 100 },
    { "bf", 854 },
    { "bi", 108 },
    { "kh", 116 },
    { "cm", 120 },
    { "ca", 124 },
    { "cv", 132 },
    { "ky", 136 },
    { "cf", 140 },
    { "td", 148 },
    { "cl", 152 },
    { "cn", 156 },
    { "cx", 162 },
    { "cc", 166 },
    { "co", 170 },
    { "km", 174 },
    { "cd", 180 },
    { "cg", 178 },
    { "ck", 184 },
    { "cr", 188 },
    { "ci", 384 },
    { "hr", 191 },
    { "cu", 192 },
    { "cy", 196 },
    { "cz", 203 },
    { "dk", 208 },
    { "dj", 262 },
    { "dm", 212 },
    { "do", 214 },
    { "ec", 218 },
    { "eg", 818 },
    { "sv", 222 },
    { "gq", 226 },
    { "er", 232 },
    { "ee", 233 },
    { "et", 231 },
    { "fk", 238 },
    { "fo", 234 },
    { "fj", 242 },
    { "fi", 246 },
    { "fr", 250 },
    { "gf", 254 },
    { "pf", 258 },
    { "tf", 260 },
    { "ga", 266 },
    { "gm", 270 },
    { "ge", 268 },
    { "de", 276 },
    { "gh", 288 },
    { "gi", 292 },
    { "gr", 300 },
    { "gl", 304 },
    { "gd", 308 },
    { "gp", 312 },
    { "gu", 316 },
    { "gt", 320 },
    { "gn", 324 },
    { "gw", 624 },
    { "gy", 328 },
    { "ht", 332 },
    { "hm", 334 },
    { "hn", 340 },
    { "hk", 344 },
    { "hu", 348 },
    { "is", 352 },
    { "in", 356 },
    { "id", 360 },
    { "ir", 364 },
    { "iq", 368 },
    { "ie", 372 },
    { "il", 376 },
    { "it", 380 },
    { "jm", 388 },
    { "jp", 392 },
    { "jo", 400 },
    { "kz", 398 },
    { "ke", 404 },
    { "ki", 296 },
    { "kp", 408 },
    { "kr", 410 },
    { "kw", 414 },
    { "kg", 417 },
    { "la", 418 },
    { "lv", 428 },
    { "lb", 422 },
    { "ls", 426 },
    { "lr", 430 },
    { "ly", 434 },
    { "li", 438 },
    { "lt", 440 },
    { "lu", 442 },
    { "mo", 446 },
    { "mk", 807 },
    { "mg", 450 },
    { "mw", 454 },
    { "my", 458 },
    { "mv", 462 },
    { "ml", 466 },
    { "mt", 470 },
    { "mh", 584 },
    { "mq", 474 },
    { "mr", 478 },
    { "mu", 480 },
    { "yt", 175 },
    { "mx", 484 },
    { "fm", 583 },
    { "md", 498 },
    { "mc", 492 },
    { "mn", 496 },
    { "ms", 500 },
    { "ma", 504 },
    { "mz", 508 },
    { "mm", 104 },
    { "na", 516 },
    { "nr", 520 },
    { "np", 524 },
    { "nl", 528 },
    { "an", 530 },
    { "nc", 540 },
    { "nz", 554 },
    { "ni", 558 },
    { "ne", 562 },
    { "ng", 566 },
    { "nu", 570 },
    { "nf", 574 },
    { "mp", 580 },
    { "no", 578 },
    { "om", 512 },
    { "pk", 586 },
    { "pw", 585 },
    { "ps", 275 },
    { "pa", 591 },
    { "pg", 598 },
    { "py", 600 },
    { "pe", 604 },
    { "ph", 608 },
    { "pn", 612 },
    { "pl", 616 },
    { "pt", 620 },
    { "pr", 630 },
    { "qa", 634 },
    { "re", 638 },
    { "ro", 642 },
    { "ru", 643 },
    { "rw", 646 },
    { "sh", 654 },
    { "kn", 659 },
    { "lc", 662 },
    { "pm", 666 },
    { "vc", 670 },
    { "ws", 882 },
    { "sm", 674 },
    { "st", 678 },
    { "sa", 682 },
    { "sn", 686 },
    { "cs", 891 },
    { "sc", 690 },
    { "sl", 694 },
    { "sg", 702 },
    { "sk", 703 },
    { "si", 705 },
    { "sb", 90 },
    { "so", 706 },
    { "za", 710 },
    { "gs", 239 },
    { "es", 724 },
    { "lk", 144 },
    { "sd", 736 },
    { "sr", 740 },
    { "sj", 744 },
    { "sz", 748 },
    { "se", 752 },
    { "ch", 756 },
    { "sy", 760 },
    { "tw", 158 },
    { "tj", 762 },
    { "tz", 834 },
    { "th", 764 },
    { "tl", 626 },
    { "tg", 768 },
    { "tk", 772 },
    { "to", 776 },
    { "tt", 780 },
    { "tn", 788 },
    { "tr", 792 },
    { "tm", 795 },
    { "tc", 796 },
    { "tv", 798 },
    { "ug", 800 },
    { "ua", 804 },
    { "ae", 784 },
    { "gb", 826 },
    { "us", 840 },
    { "um", 581 },
    { "uy", 858 },
    { "uz", 860 },
    { "vu", 548 },
    { "va", 336 },
    { "ve", 862 },
    { "vn", 704 },
    { "vg", 92 },
    { "vi", 850 },
    { "wf", 876 },
    { "eh", 732 },
    { "ye", 887 },
    { "zm", 894 },
    { "zw", 716 },
    { NULL, 0 }
  };

  t_3166cc *t;

  for (t = table; t->iso3166 != NULL; t++)
  {
    if (memcmp (ss, t->iso3166, 2) == 0)
    {
      return t->cc;
    }
  }

  return 0;
}

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

int main (int argc, char *argv[])
{
  if (argc != 1)
  {
    fprintf (stderr, "usage: %s < words\n", argv[0]);

    return (-1);
  }

  char out[BUFSIZ]; memset (out, 0, sizeof (out));
  char buf[BUFSIZ];

  int len;

  while ((len = fgetl (stdin, sizeof (buf), buf)) != -1)
  {
    if (len == 0) continue;

    strcpy (out, buf);

    int i;

    for (i = 0; i < len - 1; i++)
    {
      int cc = get_cc_from_ss (out + i);

      int *v = (int *) out + i;

      *v ^= cc;
    }

    fix_charset ((uint8_t *) out, 4);

    // printf ("%-20s -> %s%s\n", buf, buf, out);
    printf ("%s%s\n", buf, out);
  }

  return 0;
}
