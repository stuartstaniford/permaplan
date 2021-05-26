#!/usr/bin/perl
system("tests/basic-smoke/test1.pl -L 5");
system("tests/simulate/test2.pl");
system("tests/add-blocks/test3.pl");
system("tests/hammer/test4.pl");
