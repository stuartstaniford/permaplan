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

  $options .= " -p $port";
  unless($debuggerMode)
   {
    system("./permaplan $options &");
   }
  else
   {
    print("Start debugger with: run $options\n");
    print("Hit return when ready:");
    my $response = <STDIN>;
   }
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

sub processArgs
{
  my $loopLimit = 1;
  my $simLimit = 1950;
  my $skip = 0;
  
  foreach $index (0..$#_)
   {
    if($skip)
     {
      $skip = 0;
      next;
     }
    $arg = $_[$index];
    if($arg eq "-h")
     {
      print("\nCommand line options:\n\n");
      print("\t-h\tPrint this message.\n");
      print("\t-D\tSupply command for user to start permaplan in debugger.\n");
      print("\t-L N\tLoop over test N times.\n");
      print("\t-y Y\tSimulate to year Y (if relevant).\n");
      print ("\n");
      exit;
     }
    elsif($arg eq "-D")
     {
      $debuggerMode = 1; 
     }
    elsif($arg eq "-L")
     {
      $loopLimit = $_[$index+1]; 
      $skip = 1;
     }
    elsif($arg eq "-y")
     {
      $simLimit = $_[$index+1]; 
      $skip = 1;
     }
    else
     {
      die("Unknown arg $arg.\n");
     }
   }
  return ($loopLimit, $simLimit);  
}


#===========================================================================
# Function to start the simulation running in permaplan.

sub simulatePermaplan
{
  my $response = $http->get("http://127.0.0.1:$port/menu/simulate/start/");
  sanityCheckHeader($response, '/menu/simulate/start/');  
  return 1 if(length $response->{content} && $response->{content} eq "OK\n");
  return 0;
}


#===========================================================================
# Function to set the position of the camera (in world space).  Takes three 
# args which should be x,y,z of desired position.

sub setCameraPosition
{
  my $url = "http://127.0.0.1:$port/camera/set/pos/".join(':', @_).":";
  my $response = $http->get($url);
  sanityCheckHeader($response, '/camera/set/pos/');  
  return 1 if(length $response->{content} && $response->{content} eq "OK\n");
  return 0;
}


#===========================================================================
# Function to resize the permaplan window.  Takes two args which should be 
# new window width and height.

sub resizeWindow
{
  my $url = "http://127.0.0.1:$port/window/resize/".join(':', @_).":";
  my $response = $http->get($url);
  sanityCheckHeader($response, '/window/resize/');  
  return 1 if(length $response->{content} && $response->{content} eq "OK\n");
  return 0;
}


#===========================================================================
# Function to perform a double-click.  Takes two args which should be the
# x and y window positions of the desired double-click.

sub performDoubleclick
{
  my $url = "http://127.0.0.1:$port/doubleclick/".join(':', @_).":";
  my $response = $http->get($url);
  sanityCheckHeader($response, '/doubleclick/');  
  return 1 if(length $response->{content} && $response->{content} eq "OK\n");
  return 0;
}


#===========================================================================
# Function to simulate clicking the "block" button on the insert menu.

sub insertBlockButton
{
  my $url = "http://127.0.0.1:$port/menu/insert/block/";
  my $response = $http->get($url);
  sanityCheckHeader($response, '/menu/insert/block/');  
  return 1 if(length $response->{content} && $response->{content} eq "OK\n");
  return 0;
}


#===========================================================================
# Function to simulate clicking the "tree" button on the insert menu.

sub insertTreeButton
{
  my $url = "http://127.0.0.1:$port/menu/insert/tree/";
  my $response = $http->get($url);
  sanityCheckHeader($response, '/menu/insert/tree/');  
  return 1 if(length $response->{content} && $response->{content} eq "OK\n");
  return 0;
}


#===========================================================================
# Function to simulate entering parameters and choosing material on block
# entry dialog (XX material choice not implemented yet).

sub enterBlockParameters
{
  my $url = "http://127.0.0.1:$port/menu/enter/block/".join(':', @_).':';
  my $response = $http->get($url);
  sanityCheckHeader($response, '/menu/enter/block/');  
  return 1 if(length $response->{content} && $response->{content} eq "OK\n");
  return 0;
}


#===========================================================================
# Function to move the permaplan window.  Takes two args which should be 
# new window position in screen co-ordinates.
# https://www.glfw.org/docs/3.3/group__window.html#ga1abb6d690e8c88e0c8cd1751356dbca8

sub moveWindow
{
  my $url = "http://127.0.0.1:$port/window/move/".join(':', @_).":";
  my $response = $http->get($url);
  sanityCheckHeader($response, '/window/move/');  
  return 1 if(length $response->{content} && $response->{content} eq "OK\n");
  return 0;
}


#===========================================================================
# Function to find the current height of the camera.

sub getCameraHeight
{
  my $url = "http://127.0.0.1:$port/camera/";
  my $response = $http->get($url);
  sanityCheckHeader($response, '/camera/');  
  my $tree =   HTML::TreeBuilder->new();
  #print $response->{content};
  $tree->parse_content($response->{content});
  my $table = extractTableFromHTML($tree, "camSummary", "/camera");
  my $cameraZ;
  if(defined $table && defined $table->{'contents'} 
    && scalar(@{$table->{'contents'}}) && defined  $table->{'contents'}[0]
    && defined  $table->{'contents'}[0][3] 
    && ref($table->{'contents'}[0][3]) eq '')
   {
    $cameraZ = $table->{'contents'}[0][3];
   }
  else
   {
    print OUT "getCameraHeight couldn't get valid value from /camera/";
    $outLines++;
   }

  return $cameraZ;  
}


#===========================================================================
# Function to do a quadtree search for object of some type and verify that
# there are the right number of them.

sub checkObjectSearchCount
{
  my($objName, $objCount) = @_;
  my $url = "http://127.0.0.1:$port/quadsearch/$objName";
  my $response = $http->get($url);
  sanityCheckHeader($response, "/quadsearch/$objName");  
  my $tree =   HTML::TreeBuilder->new();
  #print $response->{content};
  $tree->parse_content($response->{content});
  my $table = extractTableFromHTML($tree, $objName, "/quadsearch/$objName");
  unless(defined $table && defined $table->{'contents'} 
                        && scalar(@{$table->{'contents'}}) == $objCount)
   {
    print OUT "checkObjectSearchCount couldn't get valid count of $objName.\n";
    $outLines++;
   }
}


#===========================================================================
# Function to set the direction the camera points in.  Takes three args 
# which should form the desired vector.

sub setCameraFrontVector
{
  my $url = "http://127.0.0.1:$port/camera/set/front/".join(':', @_).":";
  my $response = $http->get($url);
  sanityCheckHeader($response, '/camera/set/front/');  
  return 1 if(length $response->{content} && $response->{content} eq "OK\n");
  return 0;
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
# Function to get the simulation year from the main page.

sub getPermaplanYear
{
  my $response = $http->get("http://127.0.0.1:$port/");
  sanityCheckHeader($response, '/');
  my $tree =   HTML::TreeBuilder->new();
  #print $response->{content};
  $tree->parse_content($response->{content});
  my $simSpan = $tree->look_down(_tag => "span", 
                                        name => "simSummary");
  unless(defined $simSpan)
   {
    print OUT "Couldn't not get simulation year from permaplan.\n";
    $outLines++;
    return 10000;
   }
  $year = $simSpan->{_content}[4];
  $year =~ s/\.//g;
  $year =~ s/\s//g;
  return $year;
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
# Function to extract a table from an HTML tree.  Returns a reference to 
# an object.  The object has fields nRows, nColumns, headers (a reference 
# to an array of scalars with the columns headers (<th> elements), and
# contents -  a reference to an array of the rows.  Each element of the 
# content array is a reference to another array which contains the elements 
# of a row.  Will not currently handle anything complex inside the table 
# cells, only a simple label or number.  Anything else will end up as a
# reference to the original HTML::Element.

sub extractTableFromHTML
{
  my($tree, $name, $path) = @_;
  my %hash = ();
  my $i = 0;
  my $state = 0;
  
  # Find the right table, check it's unique, etc.
  my @tables = $tree->look_down(_tag => "table",
                                name => $name);
  if(scalar(@tables) != 1)
   {
    print OUT "$path has ".scalar(@tables)." $name tables instead of 1.\n";
    $outLines++;
   }
  if($tables[0]->{_content}[0]{_tag} ne 'tr')
   {
    print OUT "Table $path:$name doesn't start with <tr> row.\n";
    $outLines++;
   }
  
  # Extract the header row of the table, sanity check it, etc.
  my $headerRow = $tables[0]->{_content}[0]{_content}; #hopefully
  if(ref $headerRow ne 'ARRAY')
   {
    print OUT "Contents of header row are not array.\n";
    $outLines++;
   }
  $hash{'nColumns'} = scalar(@$headerRow);
  if($hash{'nColumns'} < 1 || $hash{'nColumns'} > 100)
   {
    print OUT "Table $path:$name has bad number of columns ".
                                                    "$hash{'nColumns'}.\n";
    $outLines++;
   }
  
  foreach my $element (@$headerRow)
   {
    if($element->{_tag} ne 'th')
     {
      print OUT "No <th> tag in headerRow for $path:$name table: ".
                                                $element->{_tag}.".\n";
      $outLines++;
     }
    if(ref($element->{_content}) ne 'ARRAY')
     {
      print OUT "<th> tag content not array in headerRow for ".
                      "$path:$name table: ".ref($element->{_content}).".\n";
      $outLines++;
     }
    if(scalar(@{$element->{_content}}) != 1)
     {
      print OUT "<th> tag content array bad size in headerRow for ".
                "$path:$name table: ".scalar(@{$element->{_content}}).".\n";
      $outLines++;
     }
    push @{$hash{'headers'}}, $element->{_content}[0];
   }
  
  # loop over the remaining rows of the table, sanity checking and extracting
  foreach my $row (@{$tables[0]->{_content}}
                                [1..scalar(@{$tables[0]->{_content}})-1])
   {
    $hash{'nRows'}++;  
    if($row->{_tag} ne 'tr')
     {
      print OUT "Table $path:$name has a non-<tr> row "
                                                  .$hash{'nRows'}.".\n";
      $outLines++;
     }
    if(scalar(@{$row->{_content}}) != $hash{'nColumns'})
    {
     print OUT "Table $path:$name has a wrong-size row "
                                                  .$hash{'nRows'}.".\n";
     $outLines++;
    }
    foreach my $cell (@{$row->{_content}})
     {
      if($cell->{_tag} ne 'td')
       {
        print OUT "Table $path:$name has a non-<td> element in row "
                                                    .$hash{'nRows'}.".\n";
        $outLines++;
       }
      if(ref($cell->{_content}) ne 'ARRAY')
       {
        print OUT "<td> tag content not array in row "..$hash{'nRows'}.
                    " for $path:$name table: ".ref($cell->{_content}).".\n";
        $outLines++;
       }
      if(scalar(@{$cell->{_content}}) == 1)
       {
        push @{$hash{'contents'}[$hash{'nRows'}-1]}, $cell->{_content}[0];
       }
      else
       {
        push @{$hash{'contents'}[$hash{'nRows'}-1]}, $cell->{_content};        
       }
     }
   }
  
# Debut print statements
#  print join('|', %hash)."\n";
#  print join('|', @{$hash{'headers'}})."\n";
#  foreach $i (0..scalar(@{$hash{'contents'}})-1)
#   {
#    print "$i: ".join('|', @{$hash{'contents'}[$i]})."\n";
#   } 

  # Done, go home
  return \%hash;
}


#===========================================================================
# This function takes an HTML::Element that is believed to be an <a href...> 
# tag and returns both the link and the content.  Does reasonable error
# checking that it really is what we are expecting.

sub extractLinkFromElement
{
  my($element) = @_;
  if(ref($element) ne 'HTML::Element')
   {
    if(ref($element) eq '')
     {
      print OUT "extractLinkFromElement was passed scalar \"$element\"".
                                            " instead of HTML::Element.\n";
     }
    else
     {
      print OUT "extractLinkFromElement was passed ".ref($element).
                                            " instead of HTML::Element.\n";
     }
    $outLines++;
    return (undef, undef);
   } 
  unless(defined $element->{_tag})
   {
    print OUT "extractLinkFromElement was passed a non tag instead of <a>: ".
                                                    $element->{_content}."\n";
    $outLines++;
    return (undef, undef);
   } 
  if($element->{_tag} ne 'a')
   {
    print OUT "extractLinkFromElement was passed a <".$element->{_tag}.
                                            "> tag instead of <a>.\n";
    $outLines++;
    return (undef, undef);
   } 
  unless(defined $element->{href})
   {
    print OUT "extractLinkFromElement anchor has no href attribute.\n";
    $outLines++;
    return (undef, undef);
   } 
  if(ref($element->{_content}) ne 'ARRAY')
   {
    print OUT "extractLinkFromElement anchor content was "
                          .ref($element->{_content})." instead of ARRAY.\n";
    $outLines++;
    return (undef, undef);
   } 
  if(scalar(@{$element->{_content}}) != 1)
   {
    print OUT "extractLinkFromElement anchor content array had "
              .scalar(@{$element->{_content}})." element instead of 1.\n";
    $outLines++;
    return (undef, undef);
   } 
  #print join('|', keys %$element)."\n";
  return ($element->{href}, $element->{_content}[0]);
}


#===========================================================================
# This function does a random walk down the quad tree, possibly terminating
# on the pages of VisualObjects, or possibly terminating on leaf quadtree
# nodes.

sub randomWalkQuadTree
{
  # Find the quad page and initial sanity checking
  my($startPath) = @_;
  my $response = $http->get("http://127.0.0.1:$port/$startPath");
  sanityCheckHeader($response, "/$startPath");
  my $tree = sanityCheckOuterPage($response, "/$startPath");
  
  # Examine the visual object table and pick one if available
  my $voTable = extractTableFromHTML($tree, "VisualObjects", "/$startPath");
  if(defined $voTable && defined $voTable->{'contents'} 
                                          && scalar(@{$voTable->{'contents'}}))
   {
    my $row = $voTable->{'contents'}
                            [int(rand(scalar(@{$voTable->{'contents'}})))];
    my ($link, $content) = extractLinkFromElement($row->[0]);
    return unless defined $link && defined $content;
    #print "Path $startPath;\tlink: $link;\tcontent: $content\n";  
    my $objResponse = $http->get("http://127.0.0.1:$port$link");
    sanityCheckHeader($objResponse, "$link");
    sanityCheckOuterPage($objResponse, "$link");
   }
  
  # Look into our kids, and recurse to a random one if there are any
  my $kidTable = extractTableFromHTML($tree, "kids", "/$startPath");
  if(!defined $kidTable || !defined $kidTable->{'contents'} 
                                    || !scalar(@{$kidTable->{'contents'}}))
   {
    #print "Terminated at path $startPath\n";
    return; # have reached the bottom of the tree.
   }
  my $row = $kidTable->{'contents'}
                          [int(rand(scalar(@{$kidTable->{'contents'}})))];
  my ($link, $content) = extractLinkFromElement($row->[0]);
  return unless defined $link && defined $content;
  #print "Path $startPath;\tlink: $link;\tcontent: $content\n";
  randomWalkQuadTree($startPath.$link);
}


#===========================================================================
# This function is for stress-testing the HTTP interface.  It does a range
# of requests for info pages in a tight loop to put maximal strain on
# the Permaplan HTTP interface (well, maximal strain a slow Perl script can
# manage anyway..

sub stressTestHTTPPages
{
  my($loops) = @_;
  
  foreach my $i (1..$loops)
   {
    sanityCheckHTTPPages();
    randomWalkQuadTree("quad/");
   }
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
  my($fileName, $testDescription) = @_;
  close(OUT);
  if($outLines)
   {
    system("open -a Xcode $fileName");
    print "$testDescription failed.\n";
    return 0;
   }
  print "$testDescription passed.\n";
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
# Function to extract the range 

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
                                    && abs($y) > 0)
   {
    my $ratio = $x/$y;
    return 0 if ($ratio > 0.99999 && $ratio < 1.00001);
   }
  elsif($x eq $y)
   {
    return 0;
   }

  return 0 if $ignorePath{$path};
  
  print OUT "JSON scalars differ at $path: $x vs $y.\n";
  $outLines++;
  return 1;
}

#===========================================================================
# Function to extract a rectangular region from the Arcs element in an 
# OLDF file.  Useful for automated geographical operations being in the
# right location.

sub extractRangeFromArcs
{
  my($oldfRef) = @_;
  
  my($xmin, $xmax, $ymin, $ymax) = ("+inf", "-inf", "+inf", "-inf");
  
  foreach $arc (@{$oldfRef->{"boundaries"}{"arcs"}})
   {
    $xmin = $arc->[0] if $arc->[0] < $xmin; 
    $xmax = $arc->[0] if $arc->[0] > $xmax; 
    $ymin = $arc->[1] if $arc->[1] < $ymin; 
    $ymax = $arc->[1] if $arc->[1] > $ymax; 
   }
  #print "xmin: $xmin, xmax: $xmax, ymin: $ymin, ymax: $ymax\n";

  return $xmin, $xmax, $ymin, $ymax;
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
