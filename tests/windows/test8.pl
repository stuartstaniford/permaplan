#!/usr/bin/perl -w

# A test that takes the same input OLDF file as test1.pl and then creates some additional
# windows and verifies that they appear correctly.

require './testSupport.pl';
$resourceDir = 'tests/basic-smoke';
$testDir = 'tests/windows';

my($loopLimit, $simLimit) = processArgs(@ARGV);

foreach my $i (1..$loopLimit)
 {
  system("rm -rf $testDir/test8.out.oldf");
  system("rm -rf $testDir/*.tmp $testDir/*.diff");
  openOutput("$testDir/test8.log");
  startPermaplan("-A -d $resourceDir/test1.oldf -D $testDir/test8.out.oldf -g 5.0");
  $winWidth = 400 unless defined $winWidth;
  $winHeight = 300 unless defined $winHeight;
  resizeWindow($winWidth, $winHeight);
  printPortMessage();
  simulatePermaplan();
  simulateUntil($simLimit);
  sanityCheckHTTPPages();
  stopPermaplan();
  checkLogForErrors();
  checkOutput("$testDir/test8.log", "$testDir test8 (trial $i)");
 }
