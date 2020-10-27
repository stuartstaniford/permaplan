#!/usr/bin/perl

open(FILE, "wc src/*.cpp include/*.h *.pl 812/*.json src/*.glsl CMakeLists.txt web/*.js web/views/*js|");

$total = 0;

while(<FILE>)
{
  next if /imgui/;
  next if /stb_image/;
  next if /imconfig/;
  next if /total/;
  @vals = split;
  $total += $vals[0];
  print "$vals[0]\t$vals[3]\n";
}

print "Total: $total\n";
