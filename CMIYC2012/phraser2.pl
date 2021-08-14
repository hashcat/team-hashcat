#!/usr/bin/perl

use strict;
use warnings;

my $num = 4;

while (my $line = <>)
{
  chomp ($line);

  my @e = split /\s+/, $line;

  for (my $i = 0; $i < scalar @e - $num; $i++)
  {
    print join (" ", $e[$i + 0], $e[$i + 1], $e[$i + 2], $e[$i + 3]), "\n";

  }
}

