#!/usr/bin/perl -w

# Test script to insert the components of a basic house via the menu system, then 
# put a couple of shade trees next to it and simulate them growing.

require './testSupport.pl';
$resourceDir = 'tests/basic-smoke';
$testDir = 'tests/house';
$testStub = 'test7';

my($loopLimit, $simLimit) = processArgs(@ARGV);

foreach my $i (1..$loopLimit)
 {
  system("rm -rf $testDir/$testStub.out.oldf");
  system("rm -rf $testDir/*.tmp $testDir/*.diff");

  openOutput("$testDir/$testStub.log");
  startPermaplan("-A -d $resourceDir/test1.oldf -D $testDir/$testStub.out.oldf -g 5.0");
  $winWidth = 1000 unless defined $winWidth;
  $winHeight = 800 unless defined $winHeight;
  resizeWindow($winWidth, $winHeight);
  moveWindow(100, 100);
  setCameraFrontVector(0,0,-1);
  my $cameraZ = getCameraHeight();
#  foreach $i (0..$boxCount-1)
#   {
#    @{$positions[$i]} = ($xmin + rand($xmax-$xmin), $ymin + rand($ymax-$ymin), $cameraZ);
#    setCameraPosition(@{$positions[$i]});
#    performDoubleclick(500,400);
#    insertBlockButton();
#    enterBlockParameters(3);
#   }
  printPortMessage();
  simulatePermaplan();
  simulateUntil($simLimit);
  sanityCheckHTTPPages();
#  checkObjectSearchCount('Box', $boxCount);
  stopPermaplan();
  checkLogForErrors();
  checkOutput("$testDir/$testStub.log", "$testDir $testStub (trial $i)");
 }

