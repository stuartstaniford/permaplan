#!/usr/bin/perl
system("tests/basic-smoke/test1.pl -L 2");
system("tests/simulate/test2.pl  -y 1920");
system("tests/add-blocks/test3.pl");
system("tests/hammer/test4.pl -L 25");
system("tests/grow-one/test5.pl -L 3 -y 1910");
system("tests/tree-array/test6.pl -y 1930");
# system("tests/house/test7.pl -y 1930");
system("tests/windows/test8.pl");
