#!/usr/bin/perl -w

# A test that takes the same input OLDF file as test1.pl and then creates some additional
# windows and verifies that they appear correctly.

require './testSupport.pl';
$resourceDir = 'tests/grow-one';
$testDir = 'tests/windows';

unshift(@ARGV, "-y", "1905");
my($loopLimit, $simLimit) = processArgs(@ARGV);

foreach my $i (1..$loopLimit)
 {
  system("rm -rf $testDir/test8.out.oldf");
  system("rm -rf $testDir/*.tmp $testDir/*.diff");
  openOutput("$testDir/test8.log");
  startPermaplan("-A -d $resourceDir/test5.oldf -D $testDir/test8.out.oldf -g 5.0");
  $winWidth = 1000 unless defined $winWidth;
  $winHeight = 800 unless defined $winHeight;
  resizeWindow($winWidth, $winHeight);
  printPortMessage();
  simulatePermaplan();
  simulateUntil($simLimit);
  # do something to create some extra windows
  my @windowList = getOpenWindowList();
  my @desiredResult = ('Landscape Simulation');
  &compareArrays(\@desiredResult, \@windowList);
  sanityCheckHTTPPages();
  stopPermaplan();
  checkLogForErrors();
  checkOutput("$testDir/test8.log", "$testDir test8 (trial $i)");
 }
