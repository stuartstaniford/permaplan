#!/usr/bin/perl -w

# This test inserts a variety of trees in a row and then simulates them.  The
# trees are specified in a fixed array in this test (to which new types of tree
# should be added).

require './testSupport.pl';
$dir = 'tests/tree-array';

my @treeMenuChoices = (
                       ['Eastern North America', 'Pinus (Pines)', 'Eastern White Pine'],
                       ['Eastern North America', 'Pinus (Pines)', 'Red Pine'],
                       ['Eastern North America', 'Pinus (Pines)', 'Scotch Pine'],
                       ['Eastern North America', 'Tsuga (Hemlocks)', 'Eastern Hemlock'],
                       ['Eastern North America', 'Acer (Maples)', 'Red Maple'],
                       ['Eastern North America', 'Fagus (Beeches)', 'American Beech'],
                       ['Eastern North America', 'Quercus (Oaks)', 'Northern Red Oak'],          
                       );

my($loopLimit, $simLimit) = processArgs(@ARGV);

checkPermaserv();

foreach my $i (1..$loopLimit)
 {
  system("rm -rf $dir/test6.out.oldf");
  openOutput("$dir/test6.log");
  startPermaplan("-A -d tests/grow-one/test5.oldf -D $dir/test6.out.oldf -g 5.0");
  printPortMessage();
  $winWidth = 1000 unless defined $winWidth;
  $winHeight = 800 unless defined $winHeight;
  resizeWindow($winWidth, $winHeight);
  my $treeCount = 0;
  for($i = 250; $i > -275; $i-= 50)
   {
    for($j = 250; $j > -275; $j-= 50)
     { 
       placeTree($i, $j, 1000, 800, @{$treeMenuChoices[$treeCount++]});    
       last if $treeCount >= scalar(@treeMenuChoices);
     }
    last if $treeCount >= scalar(@treeMenuChoices);
  }
  my $cameraZ = getCameraHeight();
  setCameraPosition(115, -120, $cameraZ-120);
  setCameraFrontVector(0.64, 0.74,-0.20);
  setCameraUpVector(0.2, 0.10, 0.98);
  simulatePermaplan();
  simulateUntil($simLimit);
  
  sanityCheckHTTPPages();
  stopPermaplan();
  checkLogForErrors();
  checkOutput("$dir/test6.log", "$dir test6 (trial $i)");
 }
