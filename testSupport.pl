# Copyright Staniford Systems.  All Rights Reserved.  May 2021 -
# This file provides routines used by the automated testing framework, that
# are required in more than one test script - get for starting and
# communicating with permaplan, and parsing the logs.

#===========================================================================
# Modules needed

use HTTP::Tiny;
use HTML::TreeBuilder;

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
# Print the port and url

sub printPortMessage
{
  print "Port is $port, accessible at http://127.0.0.1:$port/\n";
}

  
#===========================================================================
# Function to parse one of the HTML pages and make sure the basic stuff is
# there - that there's a head, a body, a title, at least one H1.  Returns
# the HTML::Tree object for further interrogation specific to each kind of
# page.

sub sanityCheckOuterPage
{
  my($response, $path) = @_;
  my $tree =   HTML::TreeBuilder->new();
  $tree->parse_content($response->{content});

  my @headTags = $tree->look_down(_tag => "head");
  if(scalar(@headTags) != 1)
   {
    print OUT "$path has no head (or more than one head).\n";
    $outLines++;
   }
  my @titles = $headTags[0]->look_down(_tag => "title");
  if(scalar(@titles) != 1)
   {
    print OUT "$path has no title in head (or more than one title).\n";
    $outLines++;
   }

  my @bodyTags = $tree->look_down(_tag => "body");
  if(scalar(@bodyTags) != 1)
   {
    print OUT "$path has no body (or more than one body).\n";
    $outLines++;
   }
  my @headers = $bodyTags[0]->look_down(_tag => "h1");
  if(scalar(@headers) < 1)
   {
    print OUT "$path has no <h1> in body.\n";
    $outLines++;
   }

  my @footers = $bodyTags[0]->look_down(_tag => "span", 
                                        name => "copyright");
  if(scalar(@headers) !=1)
   {
    print OUT "$path has no footer span in body.\n";
    $outLines++;
   }

  return $tree;
}


#===========================================================================
# Function to sanity check that the root page is present and looks correct.

sub sanityCheckRoot
{
  my $response = $http->get("http://127.0.0.1:$port/");
  sanityCheckHeader($response, '/');
  my $tree = sanityCheckOuterPage($response, '/');
  #$tree->dump();
}


#===========================================================================
# Function to sanity check that the quadtree page is present and looks correct.

sub sanityCheckQuad
{
  my $response = $http->get("http://127.0.0.1:$port/quad/");
  sanityCheckHeader($response, '/quad/');
  my $tree = sanityCheckOuterPage($response, '/quad/');
}


#===========================================================================
# Function to sanity check that the land surface page is present and looks correct.

sub sanityCheckLand
{
  my $response = $http->get("http://127.0.0.1:$port/land/");
  sanityCheckHeader($response, '/land/');
  my $tree = sanityCheckOuterPage($response, '/land/');
}


#===========================================================================
# Function to sanity check that the plants page is present and looks correct.

sub sanityCheckPlants
{
  my $response = $http->get("http://127.0.0.1:$port/plants/");
  sanityCheckHeader($response, '/plants/');
  my $tree = sanityCheckOuterPage($response, '/plants/');
}


#===========================================================================
# Function to sanity check that the camera page is present and looks correct.

sub sanityCheckCamera
{
  my $response = $http->get("http://127.0.0.1:$port/camera/");
  sanityCheckHeader($response, '/camera/');
  my $tree = sanityCheckOuterPage($response, '/camera/');
}


#===========================================================================
# Function to sanity check that the stbuf page is present and looks correct.

sub sanityCheckStbuf
{
  my $response = $http->get("http://127.0.0.1:$port/stbuf/");
  sanityCheckHeader($response, '/stbuf/');
  my $tree = sanityCheckOuterPage($response, '/stbuf/');
}


#===========================================================================
# Function to sanity check that the otbuf page is present and looks correct.

sub sanityCheckOtbuf
{
  my $response = $http->get("http://127.0.0.1:$port/otbuf/");
  sanityCheckHeader($response, '/otbuf/');
  my $tree = sanityCheckOuterPage($response, '/otbuf/');
}


#===========================================================================
# Function to sanity check that the memtrack page is present and looks correct.

sub sanityCheckMemtrack
{
  my $response = $http->get("http://127.0.0.1:$port/memtrack/");
  sanityCheckHeader($response, '/memtrack/');
  my $tree = sanityCheckOuterPage($response, '/memtrack/');
}


#===========================================================================
# Function to sanity check that the taskqueues page is present and looks correct.

sub sanityCheckTaskQueues
{
  my $response = $http->get("http://127.0.0.1:$port/taskqueues/");
  sanityCheckHeader($response, '/taskqueues/');
  my $tree = sanityCheckOuterPage($response, '/taskqueues/');
}


#===========================================================================
# Function to sanity check that the skysamples page is present and looks correct.

sub sanityCheckSkySamples
{
  my $response = $http->get("http://127.0.0.1:$port/skysamples/");
  sanityCheckHeader($response, '/skysamples/');
  my $tree = sanityCheckOuterPage($response, '/skysamples/');
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
# Function to load a file of JSON, strip comments, and put the result in
# a scalar which is returned.

sub getJSONFileNoComments
{
  my($file) = @_;
  my($string) = '';
  
  open(JSON, $file) || die("Couldn't open $file.\n");
  while(<JSON>)
   {
    next if /^\s*\/\//;
    $string .= $_;
   }  
  close(JSON);
  return $string;
}


#===========================================================================
# Needed for JSON operations in functions below.

use JSON::XS;

%ignorePath =
 (
  'Obj{introductoryData}{fileTime}[0]' => 1,
  'Obj{introductoryData}{fileTime}[1]' => 1,
  'Obj{introductoryData}{software}' => 1,
  'Obj{introductoryData}{softwareVersion}' => 1,
 );


#===========================================================================
# Helper function to diffJSON for hash references.

sub diffJSONHashRef
{
  my($ref1, $ref2, $path) = @_;

  my(@keys1) = sort keys %$ref1;
  my(@keys2) = sort keys %$ref2;
  
  unless(join('|', @keys1) eq join('|', @keys2))
   {
    unless($ignorePath{$path})
     {
      print OUT "JSON objects differ at $path: hash has keys "
        .join('|', @keys1)." vs ".join('|', @keys2)."\n";
      $outLines++;
      return 1;
     }
   }
  
  # Now we know both hashes have identical keys
  my $retval = 0;
  foreach $key (@keys1)
   {
    if(ref($ref1->{$key}) eq '' && ref($ref2->{$key}) eq '')
     {
      # two scalars
      $retval |= diffJSONScalar($ref1->{$key}, $ref2->{$key}, $path.'{'.$key.'}');
     }
    else
     {
      $retval |= diffJSON($ref1->{$key}, $ref2->{$key}, $path.'{'.$key.'}');
     } 
   }
  
  return $retval;
}


#===========================================================================
# Helper function to diffJSON for array references.

sub diffJSONArrayRef
{
  my($ref1, $ref2, $path) = @_;

  unless(scalar(@$ref1) eq scalar(@$ref2))
   {
    unless($ignorePath{$path})
     {
      print OUT "JSON arrays differ in size at $path: ".
                                  scalar(@$ref1)." vs ".scalar(@$ref2)."\n";
      $outLines++;
      return 1;
     }
   }
  
  # Now we know both arrays are the same size
  my $retval = 0;
  foreach $index (0..(scalar(@$ref1)-1))
   {
    if(ref($ref1->[$index]) eq '' && ref($ref2->[$index]) eq '')
     {
      # two scalars
      $retval |= diffJSONScalar($ref1->[$index], $ref2->[$index], 
                                                      $path.'['.$index.']');
     }
    else
     {
      $retval |= diffJSON($ref1->[$index], $ref2->[$index],
                                                      $path.'['.$index.']');
     } 
   }
  
  return $retval;
}


#===========================================================================
# Helper function to diffJSON for scalars.

sub diffJSONScalar
{
  my($x, $y, $path) = @_;

  if(Scalar::Util::looks_like_number($x) && Scalar::Util::looks_like_number($y)
                                    && $y > 0)
   {
    my $ratio = $x/$y;
    return 0 if ($ratio > 0.99999 && $ratio < 1.00001);
   }
  elsif($x eq $y)
   {
    return 0;
   }

  return 0 if $ignorePath{$path};
  
  print OUT "JSON objects differ at $path: $x vs $y.\n";
  $outLines++;
  return 1;
}


#===========================================================================
# Function to compare two JSON files, expressed as Perl data structures,
# and determine if they are materially different.

sub diffJSON
{
  my($ref1, $ref2, $path) = @_;

  if(ref $ref1 eq 'HASH' && ref $ref2 eq 'HASH') 
   {
     return diffJSONHashRef($ref1, $ref2, $path);
   }
  elsif(ref $ref1 eq 'ARRAY' && ref $ref2 eq 'ARRAY') 
   {
     return diffJSONArrayRef($ref1, $ref2, $path);
   }
  elsif(ref $ref1 eq '' && ref $ref2 eq '') #scalars 
   {
     return diffJSONScalar($$ref1, $$ref2, $path);
   }
  else
   {
    return 0 if $ignorePath{$path};
    print OUT "JSON objects differ at $path: ".ref($ref1)." vs "
                                                        .ref($ref2)."\n";
    $outLines++;
    return 1;
   }
}


#===========================================================================
# Function to compare two OLDF files.  Returns 1 if they are identical.  If
# they are different, reports the differences in the OUT file and returns 0.


sub compareOLDF
{
  my($firstFile, $secondFile) = @_;
  
  my $firstJson = getJSONFileNoComments($firstFile);
  my $secondJson = getJSONFileNoComments($secondFile);  
  my $firstPerlHash  = decode_json($firstJson);
  my $secondPerlHash  = decode_json($secondJson);

  return diffJSON($firstPerlHash, $secondPerlHash, 'Obj');  
}


#===========================================================================

1; # for the benefit of require 'testSupport.pl'

#===========================================================================
