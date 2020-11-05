#!/usr/bin/perl

open(FILE, "wc Makefile src/*.cpp include/*.h *.pl 812/*.json src/*.glsl CMakeLists.txt web/*.js web/views/*js|")
|| die("Couldn't run wc.\n");

open(OUT, "|open -a XCode -f") || die("Couldn't open XCode.\n");

$total = 0;

while(<FILE>)
{
  next if /imgui/;
  next if /stb_image/;
  next if /imconfig/;
  next if /total/;
  @vals = split;
  $total += $vals[0];
  print OUT "$vals[0]\t$vals[3]\n";
}

print OUT "Total: $total\n";
print "Total: $total\n";
