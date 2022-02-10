#!/usr/bin/perl -w

# A basic smoke test that we can read in an OLDF file with a few trees, 
# correctly serve some basic pages, and then write out a consistent OLDF 
# file.  Useful for finding startup and shutdown bugs if run repeatedly.

require './testSupport.pl';
$dir = 'tests/basic-smoke';

my($loopLimit, $simLimit) = processArgs(@ARGV);

checkPermaserv();

foreach my $i (1..$loopLimit)
 {
  system("rm -rf $dir/test1.out.oldf");
  system("rm -rf $dir/*.tmp $dir/*.diff");
  openOutput("$dir/test1.log");
  startPermaplan("-A -d $dir/test1.oldf -D $dir/test1.out.oldf -g 5.0");
  $winWidth = 400 unless defined $winWidth;
  $winHeight = 300 unless defined $winHeight;
  resizeWindow($winWidth, $winHeight);
  sanityCheckHTTPPages();
  stopPermaplan();
  checkLogForErrors();
  compareOLDF("$dir/test1.oldf", "$dir/test1.out.oldf");
  checkOutput("$dir/test1.log", "$dir test1 (trial $i)");
 }
