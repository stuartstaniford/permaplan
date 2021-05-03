#!/usr/bin/perl

# Script for tweaking the logging options so that permaplan can be compiled with a specified
# logging set-up for use in automated test scripts where we need to control what the logging
# options are so that we can check correctness based on log output.

# Input file (generally of extension .lctl) contains a series of lines, one symbol per line
# where symbols are either like LOG_FRAME_STARTS (which indicates that LOG_FRAME_STARTS should
# be set in order to compile in those logs), or doLogFrameStarts (indicating that doLogFrameStarts
# should be set to true on startup (which of course will only be of any use if LOG_FRAME_STARTS is
# also set).  Any options not explicitly set in the .lctl file will be set off by this script.


# =====================================================================================
# Read the .lctl file 

($lctlFileName) = @ARGV;
($loghFileName) = "include/Logging.h";
($logcppFileName) = "src/Logging.cpp";

open(LCTL, $lctlFileName) || die("Couldn't open $lctlFileName.\n");

while(<LCTL>)
{
  chomp;
  next if /^\#/; # ignore Perl-style comment lines
  next if /^\//; # ignore C++-style comment lines
  next unless $_; #ignore empty lines
  if(/^LOG\_\w+$/)
   {
    $LOGOptionOn{$_} = 1;
    next;
   }
  if(/^doLog\w+$/)
   {
    $doLogOptionOn{$_} = 1;
   }
  else
   {
    warn("Bad line $_\n");
   }
}

close(LCTL);

#print join(' ', keys %LOGOptionOn)."\n\n";
#print join(' ', keys %doLogOptionOn)."\n\n";


# =====================================================================================
# Process the Logging.h file

open(LOGH, $loghFileName) || die("Couldn't open $loghFileName.\n");
open(OUT, ">include/Logging.out.h") || die("Couldn't open output for $loghFileName.\n");
while(<LOGH>)
{
  #define LOG_OTDL_VALIDITY      // Log validity problems in an OTDL object
  if(/^\/*(\#define\s+)(LOG_\w+)\s(.*)$/)
   {
    if($LOGOptionOn{$2})
     {
      print OUT "$1$2 $3\n";
     }
    else
     {
      print OUT "//$1$2 $3\n";      
     }
   }
  else
   {
    print OUT;
   }
}

close(LOGH);
close(OUT);
system("open -a XCode include/Logging.out.h");


# =====================================================================================
# Process the Logging.cpp file

open(LOGCPP, $logcppFileName) || die("Couldn't open $$logcppFileName.\n");
open(OUT, ">src/Logging.out.cpp") || die("Couldn't open output for $$logcppFileName.\n");
while(<LOGCPP>)
{
  #bool doLogFrameStarts         = false; // Log each frame as it begins
  if(/^(bool\s+)(doLog\w+)(\s+=\s+)(\w+)(\;.*)$/)
   {
    if($doLogOptionOn{$2})
     {
      print OUT "$1$2$3true$5\n";
     }
    else
     {
      print OUT "$1$2$3false$5\n";      
     }
   }
  else
   {
    print OUT;
   }
}

#close(LOGCPP);
close(OUT);
system("open -a XCode src/Logging.out.cpp")


# =====================================================================================
