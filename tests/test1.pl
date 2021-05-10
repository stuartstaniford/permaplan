#!/usr/bin/perl -w

require 'testSupport.pl';

startPermaplan('-A -d tests/test1.oldf -D tests/test1.out.oldf -g 5.0');
stopPermaplan();
checkLogForErrors();
