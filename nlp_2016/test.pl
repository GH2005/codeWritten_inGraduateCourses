#!/usr/bin/perl
use strict;
use warnings;

my @a = (1, 2, 3);
my @b = (4, 5);
foreach (@a, @b) {
	print $_;
}
