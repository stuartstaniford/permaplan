#!/usr/bin/perl -w

require './testSupport.pl';
$resourceDir = 'tests/basic-smoke';
$testDir = 'tests/hammer';

my $loopLimit = processArgs(@ARGV);

system("rm -rf $testDir/test4.out.oldf");
system("rm -rf $testDir/*.tmp $testDir/*.diff");
openOutput("$testDir/test4.log");
startPermaplan("-A -d $resourceDir/test1.oldf -D $testDir/test4.out.oldf -g 5.0");
simulatePermaplan();
stressTestHTTPPages($loopLimit);
stopPermaplan();
checkLogForErrors();
checkOutput("$testDir/test4.log", "$testDir test4");

