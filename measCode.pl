#!/usr/bin/perl

$detail = 0;

$otdlFiles = `find Materials/Trees -name *.otdl`;
$otdlFiles =~ s/\n/ /g;
$regionFiles = `find Materials/Trees/Regions -name '*.json'`;
$regionFiles =~ s/\n/ /g;
$testFiles = `find tests -name \'*.pl\'`;
$testFiles .= `find tests -name \'*.oldf\'`;
$testFiles .= `find tests -name \'*.lctl\'`;
$testFiles =~ s/\S+\.out\.oldf//g;
$testFiles =~ s/\n/ /g;

open(FILE, "wc unixTime.cpp src/*.cpp include/*.h permaserv/*.cpp compilePermaserv openSrc *.pl 812/*.oldf src/*.glsl CMakeLists.txt web/*.js scripts/*.js css/*.css web/views/*js README.md docs/*.md $testFiles Materials/*.json $regionFiles co2-scenarios.json $otdlFiles|")
|| die("Couldn't run wc.\n");


open(OUT, "|sort -n -r |open -a XCode -f") || die("Couldn't open XCode.\n") if $detail;

$total = 0;

while(<FILE>)
{
  next if /imgui/;
  next if /stb_/;
  next if /imconfig/;
  next if /total/;
  next if /RTree.h/;
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
