#!/usr/bin/perl -w

# This test walks the "All Tree Selector" menus and picks a random type of
# tree to insert and then simulates its growth.

require './testSupport.pl';
$dir = 'tests/grow-one';

my($loopLimit, $simLimit) = processArgs(@ARGV);

foreach my $i (1..$loopLimit)
 {
  system("rm -rf $dir/test5.out.oldf");
  openOutput("$dir/test5.log");
  startPermaplan("-A -d $dir/test5.oldf -D $dir/test5.out.oldf -g 5.0");
  printPortMessage();
  $winWidth = 1000 unless defined $winWidth;
  $winHeight = 800 unless defined $winHeight;
  resizeWindow($winWidth, $winHeight);
  setCameraFrontVector(0,0,-1);
  my $cameraZ = getCameraHeight();
  setCameraPosition(50,50,$cameraZ);
  performDoubleclick($winWidth/2,$winHeight/2);
  insertTreeButton();
  sleep(1);
  my $response = getCurrentMenuOptions();
  checkOptionPresent($response, 'All Tree Selector');
  sendTreeSelection('All Tree Selector');
  sleep(1);
  while(($response = getCurrentMenuOptions()))
   {
    last if $response eq "OK\n";
    #print $response."\n";
    my $select = pickRandomMenuOption($response);
    #print $select."\n";
    sendAllTreeSelection($select);
    sleep(1);
   }
  sleep(1);
  
  setCameraPosition(-50, -50, $cameraZ-50);
  setCameraFrontVector(1,1,-1);
  sleep(1);
  setCameraUpVector(1,1,1);
  simulatePermaplan();
  simulateUntil($simLimit);
  
  sanityCheckHTTPPages();
  stopPermaplan();
  checkLogForErrors();
  checkOutput("$dir/test5.log", "$dir test5 (trial $i)");
 }
