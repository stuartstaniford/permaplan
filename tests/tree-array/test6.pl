#!/usr/bin/perl -w

# This test inserts a variety of trees in a row and then simulates them.  The
# trees are specified in a fixed array in this test (to which new types of tree
# should be added).

require './testSupport.pl';
$dir = 'tests/tree-array';

my @treeMenuChoices = ('Eastern North America', 'Pinus (Pines)', 'Eastern White Pine');

my($loopLimit, $simLimit) = processArgs(@ARGV);

foreach my $i (1..$loopLimit)
 {
  system("rm -rf $dir/test6.out.oldf");
  openOutput("$dir/test6.log");
  startPermaplan("-A -d tests/grow-one/test5.oldf -D $dir/test6.out.oldf -g 5.0");
  printPortMessage();
  resizeWindow(1000, 800);
  placeTree(50, 50, 1000, 800, @treeMenuChoices);  
  setCameraPosition(-50, -50, $cameraZ-50);
  setCameraFrontVector(1,1,-1);
  sleep(1);
  setCameraUpVector(1,1,1);
  simulatePermaplan();
  simulateUntil($simLimit);
  
  sanityCheckHTTPPages();
  stopPermaplan();
  checkLogForErrors();
  checkOutput("$dir/test6.log", "$dir test6 (trial $i)");
 }
