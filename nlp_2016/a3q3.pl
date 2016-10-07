#!/usr/bin/perl
# a3q3.pl, by Yang Wang, B00711141
use warnings;
use strict;
#
# parameters
#
my $type = shift @ARGV;
my $n = shift @ARGV;
my $L = shift @ARGV;
my $nm_fout = shift @ARGV;
my @fins = @ARGV;
open(my $fout, '>', $nm_fout);
printf $fout "type:%s,n:%d,L:%d\n", $type, $n, $L;
#
# invoke Text::Ngrams
#
use lib '.';
use Text::Ngrams;

my $ng = Text::Ngrams->new( windowsize => $n, type => $type );
$ng->process_files(@fins);
my $out = $ng->to_string( orderby => 'frequency', onlyfirst => $L,
	normalize => 1, spartan => 1 );
#
# output and clean
#
print $fout $out;
close $fout;
