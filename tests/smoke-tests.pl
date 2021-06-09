#!/usr/bin/perl
system("tests/basic-smoke/test1.pl -L 2");
system("tests/simulate/test2.pl  -y 1920");
system("tests/add-blocks/test3.pl");
system("tests/hammer/test4.pl -L 25");
system("tests/grow-one/test5.pl -y 1920");
