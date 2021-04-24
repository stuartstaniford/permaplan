#!/usr/bin/perl

$stub = $ARGV[0];

if(-e "include/$stub.h" || -e "src/$stub.cpp")
 {
  die("Files for $stub exist already.\n"); 
 }
open(IN, "include/EmptyClass.h") || die("Couldn't find EmptyClass.h.\n");
open(OUT, ">include/$stub.h") || die("Couldn't open $stub.h.\n");

while(<IN>)
 {
  s/EmptyClass/$stub/g;
  print OUT;  
 }

close(IN);
close(OUT);

open(IN, "src/EmptyClass.cpp") || die("Couldn't find EmptyClass.cpp.\n");
open(OUT, ">src/$stub.cpp") || die("Couldn't open $stub.cpp.\n");

while(<IN>)
 {
  s/EmptyClass/$stub/g;
  print OUT;  
 }

close(IN);
close(OUT);

system("git add include/$stub.h src/$stub.cpp");
