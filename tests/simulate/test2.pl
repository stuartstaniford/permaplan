#!/usr/bin/perl -w

require 'testSupport.pl';
$resourceDir = 'tests/basic-smoke';
$testDir = 'tests/simulate';

system("rm -rf $testDir/test2.out.oldf");
system("rm -rf $testDir/*.tmp $testDir/*.diff");
openOutput("$testDir/test2.log");
startPermaplan("-A -d $resourceDir/test1.oldf -D $testDir/test2.out.oldf -g 5.0");
simulatePermaplan();
sleep(10);
sanityCheckHTTPPages();
stopPermaplan();
checkLogForErrors();
compareOLDF("$resourceDir/test1.oldf", "$testDir/test2.out.oldf");
if(!checkOutput("$testDir/test2.log"))
 {
  print "$testDir test2 failed.\n";
 }
else
 {
  print "$testDir test2 passed.\n"; 
 }

