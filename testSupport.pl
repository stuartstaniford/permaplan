# Copyright Staniford Systems.  All Rights Reserved.  May 2021 -
# This file provides routines used by the automated testing framework, that
# are required in more than one test script - get for starting and
# communicating with permaplan, and parsing the logs.

#===========================================================================
# Modules needed

use HTTP::Tiny;

#===========================================================================
# File level constants/parameters

$portFileName   = "portPermaplan.txt";
$portBase       = 2080;
$portRangeSize  = 10;
$logFileName    = "permaplan.log";

@errorLogTypes = (
                  "LogOLDFValidity",
                  "LogOTDLValidity",
                  "LogAtlasAnomalies",
                  "LogTreeErrors",
                  "LogRequestErrors",
                  "LogResponseErrors",
                  "LogTriangleBufErrs"
                  );

# File level state variables

$http     = undef;
$port     = $portBase;
$outLines = 0;


#===========================================================================
# Function to reliably start permaplan and wait till it's definitely up
# and responding to it's HTTP interface.  Typically takes four or five
# seconds.

sub startPermaplan
{
  my($options) = @_;
  if(-f $portFileName)
   {
    open(PORT, $portFileName) || die("Couldn't read $portFileName.\n");
    $port = <PORT>;
    close(PORT);   
   }
  my $nextPort = ($port-$portBase+1)%$portRangeSize + $portBase;
  open(PORT, ">$portFileName") || die("Couldn't write $portFileName.\n");
  print PORT $nextPort;
  close(PORT);

  system("./permaplan $options -p $port &");
  $http = HTTP::Tiny->new();

  while(1)
   {
    my $response = $http->get("http://127.0.0.1:$port/alive/");
    if(length $response->{content} && $response->{content} eq "OK\n")
     {
      last;
     }
    sleep(1);
   }

  return $port;
}


#===========================================================================
# Function to stop permaplan 

sub stopPermaplan
{
  $http->get("http://127.0.0.1:$port/quit/");
  sleep(2); # Hack to give it some time to really be closed
}


#===========================================================================
# Function to open the output file for use by later checking functions 

sub openOutput
{
  my($fileName) = @_;
  open(OUT, ">$fileName") || die("Couldn't open $fileName.\n");
}


#===========================================================================
# Function to check and close the output file 

sub checkOutput
{
  my($fileName) = @_;
  close(OUT);
  if($outLines)
   {
    system("open -a Xcode $fileName");
    return 0;
   }
  return 1;
}


#===========================================================================
# Function to check the permaplan log for obvious errors

sub checkLogForErrors
{
  my $logIsGood         = 1;
  my $closeMessageCount = 0;
  
  open(LOG, $logFileName) || die("Couldn't open $logFileName.\n");
  while(<LOG>)
   {
    foreach $errType (@errorLogTypes)
     {
      if(/^$errType/)
       {
        $logIsGood = 0;
        print OUT;
        $outLines++;
       }
     }
    $closeMessageCount++ if /LogCloseDown: Orderly exit from window loop/;
    $closeMessageCount++ if /LogCloseDown: HttpDebug server shutting down normally/;
   }
  unless($closeMessageCount == 2)
   {
    $logIsGood = 0;
    print OUT "Only $closeMessageCount/2 closedown messages in log.\n";
    $outLines++;
   }
  
  close(LOG);
  return $logIsGood;
}


#===========================================================================
# Function to compare two OLDF files.  Returns 1 if they are identical.  If
# they are different, reports the differences in the OUT file and returns 0.

sub compareOLDF
{
  my($firstFile, $secondFile) = @_;
  
  system("grep -v '//' $firstFile |jq -S > $firstFile.tmp");
  system("grep -v '//' $secondFile |jq -S > $secondFile.tmp");
  system("diff $firstFile.tmp $secondFile.tmp > $firstFile.diff");
  #system("rm -rf $firstFile.tmp $secondFile.tmp");
  diffFilter("$firstFile.diff");
}


#===========================================================================
# Function to examine a diff between two jq processed OLDF files and 
# screen out various known or trivial cases, so that only diffs that are
# actually indicative of a problem remain.  These are placed into the
# output file.

sub diffFilter
{
  my($file) = @_;
  my($state) = 0;
  
  open(FILE, $file) || die("Couldn't open $file.\n");
  
  while(<FILE>)
   {
    print OUT;
    $outLines++;
   }
  
  close(FILE);
}


#===========================================================================

1; # for the benefit of require 'testSupport.pl'

#===========================================================================
