#!/usr/bin/perl
# a3q4.pl, "time string" extraction, by Yang Wang, B00711141
use strict;
use warnings;

# get and preprocess the input
my @input = <>;
my $input = join "", @input;

# get all "time string"s, each normalized and printed
while ($input =~ /(?<!(\d|:))(\d|1[0-2]|0\d)(:[0-5]\d){0,2}\s*(am|pm)/gi) {
	my $item = ($& =~ s/(\d|1[0-2]|0\d)(:[0-5]\d)?(:[0-5]\d)?\s*(am|pm)/join("", $1, defined($2)?$2:":00", lc($4))/rie);
	print "$item\n";
}
