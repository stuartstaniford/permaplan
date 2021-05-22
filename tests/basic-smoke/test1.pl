#!/usr/bin/perl -w


require './testSupport.pl';
$dir = 'tests/basic-smoke';

my $loopLimit = processArgs(@ARGV);

foreach my $i (1..$loopLimit)
 {
  system("rm -rf $dir/test1.out.oldf");
  system("rm -rf $dir/*.tmp $dir/*.diff");
  openOutput("$dir/test1.log");
  startPermaplan("-A -d $dir/test1.oldf -D $dir/test1.out.oldf -g 5.0");
  sanityCheckHTTPPages();
  stopPermaplan();
  checkLogForErrors();
  compareOLDF("$dir/test1.oldf", "$dir/test1.out.oldf");
  if(!checkOutput("$dir/test1.log"))
   {
    print "$dir test1 failed (trial $i).\n";
   }
  else
   {
    print "$dir test1 passed (trial $i).\n"; 
   }
 }
