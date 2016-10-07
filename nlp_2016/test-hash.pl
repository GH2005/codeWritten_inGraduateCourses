#!/usr/bin/perl
# File: test-hash.pl

sub four { return 'sub4' }
sub fourth { return 'sub4th' }

%p = (one => first, -two => second);
$p{-three} = third;
$p{four} = fourth;
$p{four2} = 'fourth';

for my $k (keys %p) { print "$k => $p{$k}\n" }
