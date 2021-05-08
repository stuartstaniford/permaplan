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

# File level state variables

$http = undef;
$port = $portBase;

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

  print "Permaplan is up.\n";
  return $port;
}


#===========================================================================
# Function to stop permaplan 

sub stopPermaplan
{
  $http->get("http://127.0.0.1:$port/quit/");
}


#===========================================================================

1; # for the benefit of require 'testSupport.pl'

#===========================================================================
