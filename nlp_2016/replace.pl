#!/usr/bin/perl
# replace.pl

use warnings;
use strict;

while (<>) {
	s/book/book/ig;
	print;
}
