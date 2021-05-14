#!/usr/bin/perl -w

require 'testSupport.pl';

system('rm -rf tests/test1.out.oldf');
startPermaplan('-A -d tests/test1.oldf -D tests/test1.out.oldf -g 5.0');
stopPermaplan();
checkLogForErrors();
openOutput('tests/test1.log');
compareOLDF('tests/test1.oldf', 'tests/test1.out.oldf');
if(!checkOutput('tests/test1.log'))
 {
  print "test1 failed.\n";
 }
else
 {
  print "test1 passed.\n"; 
 }
