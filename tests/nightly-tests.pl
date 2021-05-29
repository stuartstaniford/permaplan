#!/usr/bin/perl
system("tests/basic-smoke/test1.pl -L 50");
system("tests/simulate/test2.pl  -y 2200 -L 5");
system("tests/add-blocks/test3.pl -L 5");
system("tests/hammer/test4.pl -L 500");
