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
# Function to start the simulation running in permaplan.

sub simulatePermaplan
{
  my $response = $http->get("http://127.0.0.1:$port/menu/simulate/start/");
  if(length $response->{content} && $response->{content} eq "OK\n")
   {
    return 1;
   }
  else
   {
    return 0;
   }
}


#===========================================================================
# Function extract all tables from an HTML page expressed as an array of
# scalars, one line per array element.  Returns a reference to an array.
# Each element of the array is a reference to an object.  The keys in
# the object are the header labels from the table, and the values are
# references to arrays of all the column values under that column header.
#XX Does not handle the full generality of HTML, but instead relies on 
#XX (common) formatting conventions in the permaplan pages.

sub extractTablesFromHTML
{
  my($arrayRef) = @_;
  @tables = ();
  my $i = 0;
  my $state = 0;
  
  foreach my $line (@$arrayRef) # Loop over the lines in the HTML page
   {
    if($state == 0) # looking for <table...
     {
      if(/\<table/)
       {
        $state = 1;
        #XX Note that if <tr> follows <table> on same line we miss it.
       }
      next;
     }
    if($state == 1) # looking for <tr><th>...
     {
      next;
     }
   }
  return \@tables;
}


#===========================================================================
# Function to stop permaplan 

sub stopPermaplan
{
  $http->get("http://127.0.0.1:$port/quit/");
  sleep(2); # Hack to give it some time to really be closed
}


#===========================================================================
# Function to check any HTTP::Tiny response to make sure the response code
# is correct, and the response is not empty.

sub sanityCheckHeader
{
  my($response, $path) = @_;  

  unless($response->{status} == 200)
   {
    print OUT "Bad response code $response->{status} ($response->{reason})".
                  " from $path.\n";
    $outLines++;
   }  
  unless(length $response->{content})
   {
    print OUT "Empty response from $path.\n";
    $outLines++;
   }
}


#===========================================================================
# Function to sanity check that the root page is present and looks correct.

sub sanityCheckRoot
{
  my $response = $http->get("http://127.0.0.1:$port/");
  sanityCheckHeader($response, '/');
  
}


#===========================================================================
# Function to sanity check that the quadtree page is present and looks correct.

sub sanityCheckQuad
{
  my $response = $http->get("http://127.0.0.1:$port/quad/");
  sanityCheckHeader($response, '/quad/');
}


#===========================================================================
# Function to sanity check that the land surface page is present and looks correct.

sub sanityCheckLand
{
  my $response = $http->get("http://127.0.0.1:$port/land/");
  sanityCheckHeader($response, '/land/');
}


#===========================================================================
# Function to sanity check that the plants page is present and looks correct.

sub sanityCheckPlants
{
  my $response = $http->get("http://127.0.0.1:$port/plants/");
  sanityCheckHeader($response, '/plants/');
}


#===========================================================================
# Function to sanity check that the camera page is present and looks correct.

sub sanityCheckCamera
{
  my $response = $http->get("http://127.0.0.1:$port/camera/");
  sanityCheckHeader($response, '/camera/');
}


#===========================================================================
# Function to sanity check that the stbuf page is present and looks correct.

sub sanityCheckStbuf
{
  my $response = $http->get("http://127.0.0.1:$port/stbuf/");
  sanityCheckHeader($response, '/stbuf/');
}


#===========================================================================
# Function to sanity check that the otbuf page is present and looks correct.

sub sanityCheckOtbuf
{
  my $response = $http->get("http://127.0.0.1:$port/otbuf/");
  sanityCheckHeader($response, '/otbuf/');
}


#===========================================================================
# Function to sanity check that the memtrack page is present and looks correct.

sub sanityCheckMemtrack
{
  my $response = $http->get("http://127.0.0.1:$port/memtrack/");
  sanityCheckHeader($response, '/memtrack/');
}


#===========================================================================
# Function to sanity check that the taskqueues page is present and looks correct.

sub sanityCheckTaskQueues
{
  my $response = $http->get("http://127.0.0.1:$port/taskqueues/");
  sanityCheckHeader($response, '/taskqueues/');
}


#===========================================================================
# Function to sanity check that the skysamples page is present and looks correct.

sub sanityCheckSkySamples
{
  my $response = $http->get("http://127.0.0.1:$port/skysamples/");
  sanityCheckHeader($response, '/skysamples/');
}


#===========================================================================
# This function walks through all the currently known info pages in the 
# permaplan HTTP interface, and tests that they all produce vaguely sane 
# results - ie it's basically smoke testing.  API pages that actually affect
# the core program state are not tested here.

sub sanityCheckHTTPPages
{
  sanityCheckRoot();
  sanityCheckQuad();
  sanityCheckLand();
  sanityCheckPlants();
  sanityCheckCamera();
  #sanityCheckStbuf();
  #sanityCheckOtbuf();
  sanityCheckMemtrack();
  sanityCheckTaskQueues();
  sanityCheckSkySamples();
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
# Variables used across the diff processing functions

$state = 0;
$diffHeaderLine = '';

#===========================================================================
# Function to process a diff chunk header line

sub diffChunkHeader
{
  $diffHeaderLine = $_;
  @lines1 = ();
  @lines2 = ();
}


#===========================================================================
# Function to process a diff chunk header line

use Scalar::Util;

sub diffChunkIsProblem
{
  if($diffHeaderLine =~ /^(\d+)c(\d+)\s*$/)  #23c23
   {
    return 1 if (scalar(@lines1) != 1 || scalar(@lines2) != 1);
    $lines1[0] =~ s/\,//;
    $lines2[0] =~ s/\,//;
    # Case of lines that only have trivial numeric differences
    if(Scalar::Util::looks_like_number($lines1[0]) && 
                              Scalar::Util::looks_like_number($lines1[0]))
     {
      my $ratio = $lines1[0]/$lines2[0];
      return 0 if ($ratio > 0.99999 && $ratio < 1.00001);
     }
   }
  # Case of software and softwareVersion which may not match
  elsif($diffHeaderLine =~ /^(\d+)\,(\d+)c(\d+)\,(\d+)\s*$/)  # 33,34c33,34
   {
    return 1 unless $2 - $1 == $4 - $3;
    return 1 unless scalar(@lines1) == scalar(@lines2);
    #print STDERR "Here for $1,$2 and ".join(" ",@lines1)."\n";
    return 1 unless $2 - $1 + 1 == scalar(@lines1);
    foreach $i (0...$#lines1)
     {
      #print STDERR "Checking $lines1[$i], $lines2[$i]\n";
      return 1 unless $lines1[$i] =~/^\s*\"software\":/ || 
                            $lines1[$i] =~/^\s*\"softwareVersion\":/;
      return 1 unless $lines2[$i] =~/^\s*\"software\":/ || 
                            $lines2[$i] =~/^\s*\"softwareVersion\":/;
     }
    return 0;
   }
  return 1;
}


#===========================================================================
# Function to examine a diff between two jq processed OLDF files and 
# screen out various known or trivial cases, so that only diffs that are
# actually indicative of a problem remain.  These are placed into the
# output file.

sub diffFilter
{
  my($file) = @_;
  $state = 0;
  
  open(FILE, $file) || die("Couldn't open $file.\n");
  
  while(<FILE>)
   {
    if($state == 0)
     {
      diffChunkHeader();
      $state = 1;
     }
    elsif($state == 1)
     {
      if(/^\</)
       {
        s/^\<//;
        push @lines1, $_;
       }
      elsif(/^\-\-\-\s*$/)
       {
        $state = 2;
       }
      else
       {
        print OUT "Diff format failure on $_ in chunk $diffHeaderLine.\n";
       }
     }
    elsif($state == 2)
     {
      if(/^\>/)
       {
        s/^\>//;
        push @lines2, $_;
       }
      else
       {
        if(diffChunkIsProblem())
         {
          print OUT $diffHeaderLine.'<'.join('<', @lines1)."---\n".'>'.
                        join('>', @lines2)."\n";
          $outLines += 2 + scalar(@lines1) + scalar(@lines2);
         }
        diffChunkHeader();
        $state = 1;        
       }
     }    
   }
  
  close(FILE);
}


#===========================================================================

1; # for the benefit of require 'testSupport.pl'

#===========================================================================
