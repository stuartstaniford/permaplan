#!/usr/bin/perl -w

require './testSupport.pl';
$resourceDir = 'tests/basic-smoke';
$testDir = 'tests/hammer';

my($loopLimit, $simLimit) = processArgs(@ARGV);
$loopLimit = 100 if $loopLimit == 1;
system("rm -rf $testDir/test4.out.oldf");
system("rm -rf $testDir/*.tmp $testDir/*.diff");
openOutput("$testDir/test4.log");
startPermaplan("-A -d $resourceDir/test1.oldf -D $testDir/test4.out.oldf -g 5.0");
resizeWindow(400, 300);
simulatePermaplan();
stressTestHTTPPages($loopLimit);
stopPermaplan();
checkLogForErrors();
checkOutput("$testDir/test4.log", "$testDir test4");

