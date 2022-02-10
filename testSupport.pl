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
$servPortFile   = "permaserv-port.txt";
$servPort       = 0;

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
$winWidth   = undef;
$winHeight  = undef;
$outLines = 0;


#===========================================================================
# Function to check if permaserv is available and up-to-date, and start or
# restart it if necessary.

sub checkPermaserv
{
  if(-f $servPortFile)
   {
    open(PORT, $servPortFile) || die("Couldn't read $servPortFile.\n");
    $servPort = <PORT>;
    close(PORT);   
   }

}


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
  $http = HTTP::Tiny->new(('timeout' => 3));
  
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
      print("\t-H N\tSet window height to N pixels.\n");
      print("\t-D\tSupply command for user to start permaplan in debugger.\n");
      print("\t-L N\tLoop over test N times.\n");
      print("\t-W N\tSet window width to N pixels.\n");
      print("\t-y Y\tSimulate to year Y (if relevant).\n");
      print ("\n");
      exit;
     }
    elsif($arg eq "-D")
     {
      $debuggerMode = 1; 
     }
    elsif($arg eq "-H")
     {
      $winHeight = $_[$index+1]; 
      $skip = 1;
     }
    elsif($arg eq "-L")
     {
      $loopLimit = $_[$index+1]; 
      $skip = 1;
     }
    elsif($arg eq "-W")
     {
      $winWidth = $_[$index+1]; 
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
# Function to wait until simulation has reached a certain year (make sure
# you've started it and not done anything to pause it before calling this).

sub simulateUntil
{
  my($limYear) = @_;

  while(1)
   {
    my $year = getPermaplanYear();
    # print "$year\n";
    last if $year >= $limYear;
    sleep(1);
   }
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
# Function to simulate clicking arbitrary button on the insert menu.

sub insertAnyButton
{
  my ($button) = @_;
  my $url = "http://127.0.0.1:$port/menu/insert/$button/";
  my $response = $http->get($url);
  sanityCheckHeader($response, "/menu/insert/$button/");  
  return 1 if(length $response->{content} && $response->{content} eq "OK\n");
  return 0;
}


#===========================================================================
# Function to simulate clicking the "block" button on the insert menu.

sub insertBlockButton
{
  return insertAnyButton('block');
}


#===========================================================================
# Function to simulate clicking the "gable" button on the insert menu.

sub insertGableButton
{
  return insertAnyButton('gable');
}


#===========================================================================
# Function to simulate clicking the "shed" button on the insert menu.

sub insertShedButton
{
  return insertAnyButton('shed');
}


#===========================================================================
# Function to simulate clicking the "tree" button on the insert menu.

sub insertTreeButton
{
  return insertAnyButton('tree');
}


#===========================================================================
# Function to select from the tree genus selector menu.

sub sendTreeSelection
{
  my($selection) = @_;
  $selection =~ s/ /\_/g;
  my $url = "http://127.0.0.1:$port/menu/select/genus/$selection";
  my $response = $http->get($url);
  sanityCheckHeader($response, "/select/genus/$selection/");  
  return 1 if(length $response->{content} && $response->{content} eq "OK\n");
  return 0;
}


#===========================================================================
# Function to select from the json file based all tree selection menus.

sub sendAllTreeSelection
{
  my($selection) = @_;
  $selection =~ s/ /\_/g;
  my $url = "http://127.0.0.1:$port/menu/select/alltree/$selection";
  my $response = $http->get($url);
  sanityCheckHeader($response, "/select/alltree/$selection/");  
  return 1 if(length $response->{content} && $response->{content} eq "OK\n");
  return 0;
}


#===========================================================================
# Function to obtain the current menu options displayed on the screen.

sub getCurrentMenuOptions
{
  my $url = "http://127.0.0.1:$port/menu/options/";
  my $response = $http->get($url);
  sanityCheckHeader($response, '/menu/options/');  
  #print $response->{content};
  return $response->{content} if(length $response->{content});
  return 0;  
}


#===========================================================================
# Function to check that an expected option is present in a response to 
# getCurrentMenuOptions.

sub checkOptionPresent
{
  my($response, $checkOption) = @_;
  my(@options) = split("\n", $response);
  foreach $option (@options)
   {
    return 1 if $option eq $checkOption;
   }
  print OUT "checkOptionPresent couldn't find $checkOption in "
                                            .join('|', @options).".\n";
  $outLines++;
  return 0;  
}


#===========================================================================
# Function to pick a random option in from the response to 
# getCurrentMenuOptions.  Useful for stress-testing and ensuring that all
# presented menu options are actually implemented.

sub pickRandomMenuOption
{
  my($response, $checkOption) = @_;
  my(@options) = split("\n", $response);
  unless(scalar(@options))
   {
    print OUT "pickRandomMenuOption called on empty menu.\n";
    $outLines++;
   }
  return $options[int(rand(scalar(@options)))];  
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
# Function to simulate entering parameters and choosing material on gable
# entry dialog.

sub enterGableParameters
{
  my($height, $length, $width, $roofAngle, $overhang) = @_;
  my $url = "http://127.0.0.1:$port/menu/enter/gable/".
                            "$height:$length:$width:$roofAngle:$overhang:";
  print STDERR "URL: $url\n";
  my $response = $http->get($url);
  sanityCheckHeader($response, '/menu/enter/gable/');  
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
# Function to get the list of currently open windows.

sub getOpenWindowList
{
  my @windowList = ();
  my $url = "http://127.0.0.1:$port/window/list/";
  my $response = $http->get($url);
  sanityCheckHeader($response, '/window/list/');  
  my $tree =   HTML::TreeBuilder->new();
  #print $response->{content};
  $tree->parse_content($response->{content});
  my $table = extractTableFromHTML($tree, "windowList", "/window/list");
  my $rows;
  if(defined $table && defined $table->{'contents'} 
                                  && ($rows = scalar(@{$table->{'contents'}})))
   {
    foreach $row (0..$rows-1)
     {
      if(defined  $table->{'contents'}[$row] 
                            && defined $table->{'contents'}[$row][1] 
                            && defined $table->{'contents'}[$row][1]->{'_content'}
                            && defined $table->{'contents'}[$row][1]->{'_content'}[0]
                        ) 
       {
        push(@windowList, $table->{'contents'}[$row][1]->{'_content'}[0]);
       }
      else
       {
        print OUT "getOpenWindowList: Bad row $row.\n";
        $outLines++;
       }
     }
   }
  else
   {
    print OUT "getOpenWindowList couldn't get valid value from /window/list/\n";
    $outLines++;
   }

  return @windowList;  
}


#===========================================================================
# Function to compare a test array to a desired result array.

sub compareArrays
{
  my($testArrayRef, $desiredArrayRef) = @_;
  if(scalar(@$testArrayRef) != scalar(@$desiredArrayRef))
   {
    print OUT "Test array is size ".scalar(@$testArrayRef)." instead of ".
                                              scalar(@$desiredArrayRef)."\n";
    $outLines++;
    return;
   }
  for(my $i=0; $i<scalar(@$desiredArrayRef)-1; $i++)
   {
    if($testArrayRef->[$i] ne $desiredArrayRef->[$i])
     {
      print OUT "Test array does not match at position $i: ".
                  "$testArrayRef->[$i] instead of $desiredArrayRef->[$i]\n";
      $outLines++;
     }
   }
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
    print OUT "getCameraHeight couldn't get valid value from /camera/\n";
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
# Function to set the up direction (top of frame) for the camera.  Takes 
# three args which should form the desired vector.

sub setCameraUpVector
{
  my $url = "http://127.0.0.1:$port/camera/set/up/".join(':', @_).":";
  my $response = $http->get($url);
  sanityCheckHeader($response, '/camera/set/up/');  
  return 1 if(length $response->{content} && $response->{content} eq "OK\n");
  return 0;
}


#===========================================================================
# Function to insert a tree of specified kind at a specified location via
# the all tree menu, then move camera to focus on it.

sub insertSpecifiedTree
{
  my($x, $y, $winWidth, $winHeight, $treeNameArrayRef) = @_;
  setCameraFrontVector(0,0,-1);
  my $cameraZ = getCameraHeight();
  setCameraPosition($x, $y, $cameraZ);
  performDoubleclick($winWidth/2,$winHeight/2);
  insertTreeButton();
  sleep(1);
  my $response = getCurrentMenuOptions();
  checkOptionPresent($response, 'All Tree Selector');
  sendTreeSelection('All Tree Selector');
  sleep(1);
  foreach $component (@$treeNameArrayRef)
   {
    $response = getCurrentMenuOptions();
    #print $response."\n";
    checkOptionPresent($response, $component);
    sendAllTreeSelection($component);
    sleep(1);
   }
  setCameraPosition($x-20, $y-20, $cameraZ-170);
  setCameraFrontVector(1,1,-1);
  sleep(1);
  setCameraUpVector(1,1,1);
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
# Function to place a tree at a specified location.  Uses the All Tree
# Selector menu to find the right kind of tree.

sub placeTree
{
  my($x,$y, $winW, $winH, @menuSpecs) = @_;  
  setCameraFrontVector(0, 0, -1);
  my $cameraZ = getCameraHeight();
  setCameraPosition($x, $y, $cameraZ);
  performDoubleclick($winW/2,$winH/2);
  insertTreeButton();
  sleep(1);
  my $response = getCurrentMenuOptions();
  checkOptionPresent($response, 'All Tree Selector');
  sendTreeSelection('All Tree Selector');
  sleep(1);
  my $r = 0;
  while(($response = getCurrentMenuOptions()))
   {
    last if $response eq "OK\n";
    #print $response."\n";
    checkOptionPresent($response, $menuSpecs[$r]);
    sendAllTreeSelection($menuSpecs[$r]);
    $r++;
    sleep(1);
   }
  sleep(1);
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
    $closeMessageCount++ if /LogCloseDown: Orderly exit from GLFWApplication::overLoop/;
    $closeMessageCount++ if /LogCloseDown: HttpDebug server shutting down normally/;
    $closeMessageCount++ if /LogWindowOperations: Terminating window system altogether/;
    $closeMessageCount++ if /Deallocating Landscape Simulation window/;
   }
  unless($closeMessageCount == 4)
   {
    $logIsGood = 0;
    print OUT "Only $closeMessageCount/4 closedown messages in log.\n";
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
