#!/usr/bin/perl

use Text::CSV_XS;

$rawTreeFile = "ForestServiceUrbanDatabase/Data/TS3_Raw_tree_data.csv";
$coefficientFile = "ForestServiceUrbanDatabase/Data/TS6_Growth_coefficients.csv";
$outFile = "ForestServiceUrbanDatabase/json/trees.json";

open(RAWFILE, $rawTreeFile) || die("Couldn't open $rawTreeFile.\n");
open(COEFFILE, $coefficientFile) || die("Couldn't open $coefficientFile.\n");

open(OUT, ">$outFile") || die("Couldn't open $outFile.\n");

$firstRow = <RAWFILE>;
$firstRow =~ s/[\r\n]//g;

@names = split('\,', $firstRow);

print OUT "\[\n";
$firstLine = 1;
while(<RAWFILE>)
{
  if($firstLine)
   {
    $firstLine = 0;
   }
  else
   {
    print OUT ",\n"; # terminate the previous line
   }
  print OUT "\{\n";
  s/[\r\n]//g;
  @vals = split('\,', $_);
  $firstVal = 1;
  foreach $i (0.. $#names)
   {
    if(defined $vals[$i] && $vals[$i])
     {
      if($firstVal)
       {
        $firstVal = 0;
       }
      else
       {
        print OUT ",\n"; # terminate the previous line
       }
      print OUT "\'$names[$i]\'\:\t\'$vals[$i]\'";
     }
   }
  print OUT "\n\}";
}
print OUT "\n]\n";

close(RAWFILE);
close(COEFFILE);
close(OUT);
