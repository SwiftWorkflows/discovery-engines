#!/usr/bin/env perl

use Digest::MD5 qw(md5);

# print "$ARGV[0]\n";
$h = md5($ARGV[0]);
# print "$h\n";
print unpack('L', $h);
print "\n";
