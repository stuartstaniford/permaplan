#!/usr/bin/perl

# See https://metacpan.org/pod/Text::CSV_XS
# for usage information on the Text::CSV_XS module
use Text::CSV_XS;
$csv = Text::CSV_XS->new ({ binary => 1, auto_diag => 1 });

$rawTreeFile = "ForestServiceUrbanDatabase/Data/TS3_Raw_tree_data.csv";
$coefficientFile = "ForestServiceUrbanDatabase/Data/TS6_Growth_coefficients.csv";
$outFile = "ForestServiceUrbanDatabase/json/trees.json";

open(RAWFILE, "<:encoding(utf8)", $rawTreeFile) || die("Couldn't open $rawTreeFile.\n");
open(COEFFILE, $coefficientFile) || die("Couldn't open $coefficientFile.\n");

open(OUT, ">$outFile") || die("Couldn't open $outFile.\n");

$firstRow = $csv->getline(RAWFILE);

@names = @$firstRow;

print OUT "\[\n";
$firstLine = 1;
my $row;
while($row = $csv->getline(RAWFILE))
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
  @vals = @$row;
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
