#!/usr/bin/perl -w

require 'testSupport.pl';

openOutput('tests/test1.log');
startPermaplan('-A -d tests/test1.oldf -D tests/test1.out.oldf -g 5.0');
stopPermaplan();
checkLogForErrors();
checkOutput('tests/test1.log');
