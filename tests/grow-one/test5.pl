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
  getCurrentMenuOptions();
  #sanityCheckHTTPPages();
  #stopPermaplan();
  #checkLogForErrors();
  #compareOLDF("$dir/test1.oldf", "$dir/test1.out.oldf");
  #checkOutput("$dir/test1.log", "$dir test1 (trial $i)");
 }
