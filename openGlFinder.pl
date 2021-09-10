#!/usr/bin/perl
#
# A utility script to find all openGL calls, GLFW calls, GLEW calls, and create
# a summary report for use in understanding how the codebase depends on OpenGL.

my @files = split("\n", `ls include/*.h src/*.cpp src/*.c`);

foreach $file (@files)
 {
  open(FILE, $file)|| die("Couldn't open $file.n");
  my %openGLTokensFound = ();
  my %openGLFWTokensFound = ();
  my %openGLEWTokensFound = ();
  my %otherTokensFound = ();
  
  while(<FILE>)
   {
    my @tokens = split /[\s\(\)\+\-\.\:\/\,\{\}\[\]\;\*\<\>\=\@\"\&]+/;
    foreach $token (@tokens)
     {
      next unless $token =~ /^gl/;
      next if $token =~ /^global/;
      next if $token =~ /^glm_/;
      if($token =~ /^glfw[A-Z]/)
       {
        $openGLFWTokensFound{$token}++;
       }
      elsif($token =~ /^glew[A-Z]/)
       {
        $openGLEWTokensFound{$token}++;
       }
      elsif($token =~ /^gl[A-Z]/)
       {
        $openGLTokensFound{$token}++;
       }
      else
       {
        $otherTokensFound{$token}++;
       }  
     }
   }
  close(FILE);
  my $total = scalar(keys %openGLFWTokensFound) + scalar(keys %openGLEWTokensFound)
              + scalar(keys %openGLTokensFound) + scalar(keys %otherTokensFound);
  next unless $total > 0;
  print "$file\n";
  print "\tGLFW tokens:\n";
  print "\tGLEW tokens:\n";
  print "\tGL tokens\n";
  print "\tOther tokens:\n";
  print "\n";
 }
