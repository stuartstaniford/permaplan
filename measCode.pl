#!/usr/bin/perl

$detail = 0;

open(FILE, "wc src/*.cpp include/*.h *.pl 812/*.json src/*.glsl CMakeLists.txt web/*.js web/views/*js docs/*.md tests/*.oldf|")
|| die("Couldn't run wc.\n");

open(OUT, "|open -a XCode -f") || die("Couldn't open XCode.\n") if $detail;

$total = 0;

while(<FILE>)
{
  next if /imgui/;
  next if /stb_/;
  next if /imconfig/;
  next if /total/;
  @vals = split;
  $total += $vals[0];
  print OUT "$vals[0]\t$vals[3]\n" if $detail;
}

$makeCount = `grep -n 'DO NOT DELETE' Makefile`;
@syms = split(/\:/, $makeCount);
print OUT "$syms[0]\tMakefile\n" if $detail;
$total += $syms[0];

print OUT "Total: $total\n" if $detail;
print "Total: $total\n";
