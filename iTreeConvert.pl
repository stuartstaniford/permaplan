#!/usr/bin/perl

$iTreeFile = "web/itree-SpeciesList_28-10-2020-11-10-57-AM.txt";
$outFile = "web/trees.json";

open(FILE, $iTreeFile) || die("Couldn't open $iTreeFile.\n");

open(OUT, ">$outFile") || die("Couldn't open $outFile.\n");

$firstRow = <FILE>;
$firstRow =~ s/[\r\n]//g;

@names = split('\t', $firstRow);

print OUT "\[\n";
$firstLine = 1;
while(<FILE>)
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
  @vals = split('\t', $_);
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
