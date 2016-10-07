#!/usr/bin/perl
use strict;
use warnings;

sub conc {
	my @strs = @_;
	@strs = sort {$a cmp $b} @strs;
	my $res = join('', @strs);
	return $res;
}

print (&conc('aaa', 'ccc'));
print "\n";
print (&conc('ccc', 'aaa'));
print "\n";
print (&conc("abc\n\n", 26, "25.3"));
