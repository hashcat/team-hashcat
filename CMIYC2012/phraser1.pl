#!/usr/bin/perl

use strict;
use warnings;

my $min_len = 8;
my $max_len = 34;

while (my $line = <>)
{
  chomp ($line);

  my $len = length $line;

  for (my $i = 0; $i < $len; $i++)
  {
    for (my $j = $len - $i; $j; $j--)
    {
      next if ($j < $min_len);
      next if ($j > $max_len);

      my $str = substr ($line, $i, $j);

      print $str, "\n";
    }
  }
}

