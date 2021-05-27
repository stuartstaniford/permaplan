#!/usr/bin/perl -w

require './testSupport.pl';
$resourceDir = 'tests/basic-smoke';
$testDir = 'tests/add-blocks';

my($loopLimit, $simLimit) = processArgs(@ARGV);

foreach my $i (1..$loopLimit)
 {
  system("rm -rf $testDir/test3.out.oldf");
  system("rm -rf $testDir/*.tmp $testDir/*.diff");
  my $oldfJSON = decode_json(getJSONFileNoComments("$resourceDir/test1.oldf"));
  my($xmin, $xmax, $ymin, $ymax) = extractRangeFromArcs($oldfJSON);

  openOutput("$testDir/test3.log");
  startPermaplan("-A -d $resourceDir/test1.oldf -D $testDir/test3.out.oldf -g 5.0");
  resizeWindow(1000,800);
  setCameraFrontVector(0,0,-1);
  my $cameraZ = getCameraHeight();
  foreach $i (0..6)
   {
    @{$positions[$i]} = ($xmin + rand($xmax-$xmin), $ymin + rand($ymax-$ymin), $cameraZ);
    setCameraPosition(@{$positions[$i]});
    sleep(1);
   }
  sanityCheckHTTPPages();
  stopPermaplan();
  checkLogForErrors();
  checkOutput("$testDir/test3.log", "$testDir test3 (trial $i)");
 }

