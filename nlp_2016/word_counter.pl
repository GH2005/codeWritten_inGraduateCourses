#!/usr/bin/perl
# word_counter.pl written by Yang Wang, B00711141

use strict;
use warnings;

my @text = <>;
my $text_comb = join("", @text);

my %freq = ();
foreach my $wd ($text_comb =~ /\w+/g) {
	$freq{lc $wd}++;
}

my $count = 0;
my @most_common;
my @hapax;

foreach my $wd (sort {$freq{$b}<=>$freq{$a}} (keys %freq)) {
	if ($count < 10) { push(@most_common, $wd); $count++; }
	if ($freq{$wd} == 1) { push(@hapax, $wd); }
}

print "10 most common words are: ", join(",", @most_common), "\n\n", "The number of hapax legomena is ", scalar(@hapax), "\n";
