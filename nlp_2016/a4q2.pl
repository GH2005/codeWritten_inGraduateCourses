#!/usr/bin/perl
# a4q2.pl, by Yang Wang, B00711141
use strict;
use warnings;
#
# arguments
#
my $L = shift @ARGV;
my $nm_f1 = shift @ARGV;
my $nm_f2 = shift @ARGV;
#
# read the first line of each file and check for consistency
#
open(my $f1, '<', $nm_f1) or die $!;
open(my $f2, '<', $nm_f2) or die $!;
my $head1 = <$f1>; chomp $head1;
my $head2 = <$f2>; chomp $head2;
my @sp1 = split(",L:", $head1);
my @sp2 = split(",L:", $head2);
die "Incorrect Parameters" if ($sp1[0] ne $sp2[0]);
die "Incorrect Parameters" if ($sp1[1] < $L || $sp2[1] < $L);
#
# read n-grams; compute and output the CNG distance
#
my @ng1 = <$f1>; my @ng2 = <$f2>;
close $f1; close $f2;
chomp @ng1; chomp @ng2;
my $size1 = scalar(@ng1) < $L ? scalar(@ng1) : $L;
my $size2 = scalar(@ng2) < $L ? scalar(@ng2) : $L;

# divide n-grams and their normalized frequencies
my %hash1; my %hash2;
for (my $i = 0; $i < $size1; $i++) {
	my @tmp = split("\t", $ng1[$i]);
	$hash1{$tmp[0]} = $tmp[1];
}
for (my $i = 0; $i < $size2; $i++) {
	my @tmp = split("\t", $ng2[$i]);
	$hash2{$tmp[0]} = $tmp[1];
}

# compute the CNG distance
my %union = map {$_ => 1} keys(%hash1), keys(%hash2);
my @union = keys(%union); # union of domain of two profiles
my $cng_dist = 0;
foreach my $ele (@union) {
	my $v1 = defined($hash1{$ele}) ? $hash1{$ele} : 0;
	my $v2 = defined($hash2{$ele}) ? $hash2{$ele} : 0;
	$cng_dist += (2 * ($v1-$v2) / ($v1+$v2))**2;
}

# print the CNG distance
print "$cng_dist\n";
