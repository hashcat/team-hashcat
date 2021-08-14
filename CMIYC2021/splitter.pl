#!/usr/bin/perl

use strict;
use warnings;

my $min = 5;

while (1)
{
  open (IN, $ARGV[0]) or die;

  while (my $line = <IN>)
  {
    chomp $line;

    my @words = split " ", $line;

    next if scalar @words < $min;

    my $lower_limit = 0;
    my $upper_limit = scalar @words;

    my @out;

    for (my $i = 0; $i < $min; $i++)
    {
      my $random_number = int(rand($upper_limit-$lower_limit)) + $lower_limit;

      push (@out, $words[$random_number]);
    }

    printf ("%s\n", join (" ", @out));
  }

  close (IN);
}
