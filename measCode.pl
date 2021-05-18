#!/usr/bin/perl

$detail = 1;

$otdlFiles = `find Materials/Trees -name *.otdl`;
$otdlFiles =~ s/\n/ /g;
$testFiles = `find tests -name \'*.pl\'`;
$testFiles .= `find tests -name \'*.oldf\'`;
$testFiles .= `find tests -name \'*.lctl\'`;
$testFiles =~ s/\S+\.out\.oldf//g;
$testFiles =~ s/\n/ /g;

open(FILE, "wc unixTime.cpp src/*.cpp include/*.h *.pl 812/*.oldf src/*.glsl CMakeLists.txt web/*.js web/views/*js README.md docs/*.md $testFiles Materials/Trees/Regions/*.json co2-scenarios.json $otdlFiles|")
|| die("Couldn't run wc.\n");

open(OUT, "|sort -n -r |open -a XCode -f") || die("Couldn't open XCode.\n") if $detail;

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
close(OUT) if $detail;
print "Total: $total\n";
