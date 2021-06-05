#!/usr/bin/perl -w


require './testSupport.pl';
$dir = 'tests/grow-one';

my($loopLimit, $simLimit) = processArgs(@ARGV);

foreach my $i (1..$loopLimit)
 {
  system("rm -rf $dir/test5.out.oldf");
  openOutput("$dir/test5.log");
  startPermaplan("-A -d $dir/test5.oldf -D $dir/test5.out.oldf -g 5.0");
  resizeWindow(1000, 800);
  setCameraFrontVector(0,0,-1);
  my $cameraZ = getCameraHeight();
  setCameraPosition(50,50,$cameraZ);
  performDoubleclick(500,400);
  insertTreeButton();
  my $response = getCurrentMenuOptions();
  checkOptionPresent($response, 'All Tree Selector');
  
  #sanityCheckHTTPPages();
  #stopPermaplan();
  #checkLogForErrors();
  #checkOutput("$dir/test5.log", "$dir test5 (trial $i)");
 }
