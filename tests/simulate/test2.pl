#!/usr/bin/perl -w

# A test that takes the same input OLDF file as test1.pl and then simulates it
# for a while to make sure simulation is superficially functional.

require './testSupport.pl';
$resourceDir = 'tests/basic-smoke';
$testDir = 'tests/simulate';

my($loopLimit, $simLimit) = processArgs(@ARGV);

foreach my $i (1..$loopLimit)
 {
  system("rm -rf $testDir/test2.out.oldf");
  system("rm -rf $testDir/*.tmp $testDir/*.diff");
  openOutput("$testDir/test2.log");
  startPermaplan("-A -d $resourceDir/test1.oldf -D $testDir/test2.out.oldf -g 5.0");
  resizeWindow(400, 300);
  printPortMessage();
  simulatePermaplan();
  while(1)
   {
    my $year = getPermaplanYear();
    # print "$year\n";
    last if $year >= $simLimit;
    sleep(1);
   }
  sanityCheckHTTPPages();
  stopPermaplan();
  checkLogForErrors();
  compareOLDF("$resourceDir/test1.oldf", "$testDir/test2.out.oldf");
  checkOutput("$testDir/test2.log", "$testDir test2 (trial $i)");
 }
