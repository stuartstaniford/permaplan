#!/usr/bin/perl

open(FILE, "wc src/*.cpp include/*.h *.pl 812/*.json src/*.glsl CMakeLists.txt|");

$total = 0;

while(<FILE>)
{
  next if /imgui/;
  next if /stb_image/;
  next if /imconfig/;
  next if /total/;
  @vals = split;
  $total += $vals[0];
}

print "$total\n";