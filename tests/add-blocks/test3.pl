#!/usr/bin/perl -w

require './testSupport.pl';
$resourceDir = 'tests/basic-smoke';
$testDir = 'tests/add-blocks';

my $loopLimit = processArgs(@ARGV);

foreach my $i (1..$loopLimit)
 {
  system("rm -rf $testDir/test3.out.oldf");
  system("rm -rf $testDir/*.tmp $testDir/*.diff");
  openOutput("$testDir/test3.log");
  startPermaplan("-A -d $resourceDir/test1.oldf -D $testDir/test3.out.oldf -g 5.0");
  foreach $i (0..99)
   {
    print "$i, ";
   }
  sanityCheckHTTPPages();
  stopPermaplan();
  checkLogForErrors();
  checkOutput("$testDir/test3.log", "$testDir test3 (trial $i)");
 }

