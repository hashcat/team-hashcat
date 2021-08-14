#!/usr/bin/env perl

use strict;
use warnings;


if (scalar @ARGV != 2)
{
  die "$0 templates.txt config.table\n";
}

my ($templates, $table) = @ARGV;

my $db_templates = load_templates ($templates);

my $db_table = load_table ($table);


for my $template (@{$db_templates})
{
  my $db_template = parse_template ($template);

  my $total_cnt = get_total_cnt ($db_template, $db_table);

  for (my $i = 0; $i < $total_cnt; $i++)
  {
    my $idx = $i;

    my $out = "";

    for my $key (@{$db_template})
    {
      if (exists $db_table->{$key})
      {
        my $key_cnt = scalar @{$db_table->{$key}};

        my $replace = $db_table->{$key}->[$idx % $key_cnt];

        $out .= $replace;

        $idx /= $key_cnt;
      }
      else
      {
        $out .= $key;
      }
    }

    print $out, "\n";
  }
}

sub parse_template
{
  my $template = shift;

  my @db_template = split "", $template;

  return \@db_template;
}

sub get_total_cnt
{
  my $db_template = shift;
  my $db_table    = shift;

  my $cnt = 1;

  for my $key (@{$db_template})
  {
    if (exists $db_table->{$key})
    {
      $cnt *= scalar @{$db_table->{$key}};
    }
  }

  return $cnt;
}

sub load_templates
{
  my $templates = shift;

  my $db_templates = [];

  open (IN, $templates) or die ("$templates: $!\n");

  while (my $line = <IN>)
  {
    $line =~ s/\n$//;
    $line =~ s/\r$//;

    push (@{$db_templates}, $line);
  }

  close (IN);

  return $db_templates;
}

sub load_table
{
  my $table = shift;

  my $db_table = {};

  open (IN, $table) or die ("$table: $!\n");

  while (my $line = <IN>)
  {
    $line =~ s/\n$//;
    $line =~ s/\r$//;

    my ($from, $to) = split "\t", $line;

    push (@{$db_table->{$from}}, $to);
  }

  close (IN);

  return $db_table;
}
