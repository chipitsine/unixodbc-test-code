#!/usr/bin/perl -w

#
# tst.pl
#
# PURPOSE:
#
#  This script is used to execute tests. The tests may be organized in a hierarchy of
#  test suites and tests.
#
# INPUTS:
#
#  command-line   - 0-n variables in "Key=Value" format
#  index.ini      - processes this when sent to a directory (starts in current dir)
#  <tests>        - these are specified in index.ini files
#
# OUTPUTS:
#
#  A variety of output formats are supported. The format used is indicated by the value 
#  stored in the ResultFormat variable and can be one of the following values;
# 
#     - 1 = RESULTS_FORMAT_NORMAL
#     - 2 = RESULTS_FORMAT_XML
#     - 3 = RESULTS_FORMAT_HTML
#     - 4 = RESULTS_FORMAT_TAP
#     - 5 = RESULTS_FORMAT_INTERACTIVE
#
#  The ResultFormat variable may be set like any other variable;
# 
#     - command-line ie "ResultFormat=4" as command-line arg to tst.pl
#     - index.ini ie "ResultFormat=4" in "[config]" section
# 
# AUTHOR:
#
#   Peter Harvey <pharvey@mysql.com
#
# LICENSE:
# 
#  GPL, LGPL or BSD (your choice)
# 

#
# TO DO:
#
#  1. Allow vars to be passed to this script on the command-line. Good to prevent pwd from
#     being embedded in an ini file.
#  2. Allow for a Depends parameter containing a list of other tests. The idea is that if any
#     of the listed tests failed this test will not be executed - it will be [SKIP  ].
#     

#
# We use standard perl except for IniFiles.
# - always use strict (just because)
# - use Cwd to help keep track of current dir (means using its ver of some funcs)
# - use IniFiles to provide read/write of ini files (this can be found at cpan)
#
use strict;  
use Cwd 'getcwd', 'chdir', 'realpath';
use Config::IniFiles;

$| = 1;                               # Make IO unbuffered

#  \brief   Some "constants".
#
my $RESULTS_FORMAT_NORMAL      = 1;
my $RESULTS_FORMAT_XML         = 2;
my $RESULTS_FORMAT_HTML        = 3;
my $RESULTS_FORMAT_TAP         = 4;
my $RESULTS_FORMAT_INTERACTIVE = 5;

#  \brief   Scoped vars. 
#
#           For each ini we have a hash of key/value pairs read. We 
#           store these hashs in arrayScope so we can resolve as needed.
#
my @arrayScope;

#  \brief   The number of tests and the test we are on.
# 
#           This is mostly useful for TAP output.
# 
my $bDryRun = 1;
my $nTests  = 0;
my $nTest   = 0;

#  \brief   Prints a result of a test in human readable form.
#
#           Scans back along the variable scope to get the "Name" variable at each level
#           and uses it to print a 'path' to name the test.
#
#           The implication is that "Name" becomes a special case variable and should be
#           set in a 'config' section at the top of each ini and not be used for anything
#           else.
#
#  \param   Result 0|1.
#
#  \sa      subPrintResultXML
#           subPrintResultHTML
#           subPrintResultTAP
#
sub subPrintResultNormal 
{
   my $nResult       = $_[0];
   my $stringResult  = "[FAILED]";
   my $stringPath    = "";
   my $nLast         = $#arrayScope;
   my $n;

   if ( $nResult eq 0 ) 
   {
      $stringResult = "[OK    ]";
   }

   for ( $n = $nLast; $n >= 0; $n-- ) 
   {
      $stringPath = $arrayScope[$n]{Name} . "/" . $stringPath;
   }

   print( sprintf( "%-70s %s\n", $stringPath, $stringResult ) );
}

#  \brief   Prints a result of a test in XML form.
#
#           Scans back along the variable scope to get the "Name" variable at each level
#           and uses it to print a 'path' to name the test.
#
#           The implication is that "Name" becomes a special case variable and should be
#           set in a 'config' section at the top of each ini and not be used for anything
#           else.
#
#  \param   Result string.
#
#  \sa      subPrintResultNormal
#           subPrintResultHTML
#           subPrintResultTAP
#
sub subPrintResultXML
{
   my $nResult       = $_[0];
   my $stringResult  = "FAILED";
   my $nLast         = $#arrayScope;

   if ( $nResult eq 0 ) 
   {
      $stringResult = "OK";
   }

   print( sprintf( "<Result Name=%s>%s</Result>\n", $arrayScope[$nLast]{Name}, $stringResult ) );
}

#  \brief   Prints a result of a test in HTML form.
#
#           Scans back along the variable scope to get the "Name" variable at each level
#           and uses it to print a 'path' to name the test.
#
#           The implication is that "Name" becomes a special case variable and should be
#           set in a 'config' section at the top of each ini and not be used for anything
#           else.
#
#  \param   Result string.
#
#  \sa      subPrintResultNormal
#           subPrintResultXML
#           subPrintResultTAP
#
sub subPrintResultHTML
{
   my $nResult       = $_[0];
   my $stringResult  = "FAILED";
   my $stringPath    = "";
   my $nLast         = $#arrayScope;
   my $n;

   if ( $nResult eq 0 ) 
   {
      $stringResult = "OK";
   }

   for ( $n = $nLast; $n >= 0; $n-- ) 
   {
      $stringPath = $arrayScope[$n]{Name} . "/" . $stringPath;
   }

   print( sprintf( "%-70s <B>%s</B>\n", $stringPath, $stringResult ) );
}

#  \brief   Prints a result of a test in TAP form.
#
#           Scans back along the variable scope to get the "Name" variable at each level
#           and uses it to print a 'path' to name the test as the description.
#
#           The implication is that "Name" becomes a special case variable and should be
#           set in a 'config' section at the top of each ini and not be used for anything
#           else.
#
#           TAP output is pretty basic;
# 
#           <result> <index> <name> [directive]
#
#           result      Test result which is "ok" or "not ok".
#           index       Test index.
#           name        Test name or description. We use full name-path to test.
#           directive   Post-test directive which is after a '#' and can be "TODO" or "SKIP".
# 
#  \param   Result string.
#
#  \sa      subPrintResultNormal
#           subPrintResultXML
#           subPrintResultHTML
#
sub subPrintResultTAP
{
   my $nResult          = $_[0];
   my $stringDirective  = $_[1];
   my $stringResult     = "not ok";
   my $stringPath       = "";
   my $nLast            = $#arrayScope;
   my $n;

   if ( $nResult eq 0 ) 
   {
      $stringResult = "ok";
   }

   for ( $n = $nLast; $n >= 0; $n-- ) 
   {
      $stringPath = $arrayScope[$n]{Name} . "/" . $stringPath;
   }

   # As stderr/stdout from tests and this output mix, make
   # sure TAP line starts at the beginning of a line
   print( sprintf( "\n%s %d - %-70s %s\n", $stringResult, $nTest, $stringPath, $stringDirective ) );
}

#  \brief   Get a variable.
#
#           Get a variable value, if it exists. We start with the current scope level
#           and work our way backwards - to outer levels of scope.
#
#  \param   0 Name.
# 
#  \returns string
# 
#  \retval  value
#  \retval  ""
#
sub subGetVariable
{
   my $stringName    = $_[0];
   my $nLast         = $#arrayScope;
   my $n;

   for ( $n = $nLast; $n >= 0; $n-- ) 
   {
      if ( $arrayScope[$n]{$stringName} ) 
      {
         return $arrayScope[$n]{$stringName};
      }
   }

   return "";
}

#  \brief   Set a variable.
#
#           Create a new variable in the scope (file). If the variable already exists 
#           at this level then simply replace value.
#
#  \param   0 Name.
#  \param   1 Value. 
#
sub subSetVariable
{
   my $stringName    = $_[0];
   my $stringValue   = $_[1];
   my $nLast         = $#arrayScope;

   $arrayScope[$nLast]{$stringName}  = $stringValue; 
}

#  \brief   Execute the given command.
#
#           The command can be any viable system() command; a shell script,
#           an executable binary, or other.
#
#           The command may have arguments which are constant and/or variable. 
#           The variable args can be environment variables as supported by the 
#           operating system or can be variables set in the ini files.
#
#           Command arguments which include ini variables bracketed with %
#           are substituted for their value.
#
#  \param   0 Command to execute.
#
sub subCommandExec
{
   my $stringCommand = $_[0];
   my $nReturn       = 0;
   my $pick          = "no";
   my $nResultFormat = subGetVariable( "ResultFormat" ) + 0;

   $nTest++;

   # substitute any variables (enclosed in %) with their value
   $_ = $stringCommand;
   $_ =~ s/%(\w+)%?/@{[subGetVariable("$1")]}/g;

   if ( subGetVariable( "Debug" ) ) 
   {
      print( "Command: " . $_ . "\n" );
   }

   RETRY:

   # execute whatever someone wants (binary, shell, whatever)
   $nReturn = system( $_ );

   # handle result
   if ( $nResultFormat eq $RESULTS_FORMAT_NORMAL ) 
   {
      subPrintResultNormal( $nReturn );
   }
   elsif ( $nResultFormat eq $RESULTS_FORMAT_HTML ) 
   {   
      subPrintResultHTML( $nReturn );
   }
   elsif ( $nResultFormat eq $RESULTS_FORMAT_XML ) 
   {   
      subPrintResultXML( $nReturn );
   }
   elsif ( $nResultFormat eq $RESULTS_FORMAT_TAP ) 
   {
      subPrintResultTAP( $nReturn, "" );
   }
   elsif ( $nResultFormat eq $RESULTS_FORMAT_INTERACTIVE ) 
   {
      if ( $nReturn != 0 ) 
      {
         subPrintResultNormal( "[FAILED]\n" );
         print ( "Abort/Retry/Ignore? ");
         chomp($pick = <STDIN>);
         if ( $pick eq "a" ) {
           die "Aborting";
         } elsif ( $pick eq "r" ) {
           goto RETRY;
         }
      }
      else 
      {
         subPrintResultNormal( "[OK    ]\n" );
      }
   }
   else
   {
      print( sprintf( "[ERROR] Unknown ResultFormat %d\n", $nResultFormat ) );
   }
}

#  \brief   Process the given ini section parameter.
# 
#           If its a command; we execute it otherwise we assume its a request
#           to set a variable.
#
#  \param   0 An ini object created from Config::IniFiles().
#  \param   1 The section name.
#  \param   2 Parameter name.
#
sub subProcessParameter
{
   my $objectIni       = $_[0];
   my $stringSection   = $_[1];
   my $stringParameter = $_[2];
   my $stringValue     = "";

   $stringValue = $objectIni->val( $stringSection, $stringParameter );

   if ( subGetVariable( "Debug" ) ) 
   {
      print( $stringParameter . "\t\t" . $stringValue ."\n" );
   }

   if ( $stringParameter eq "Cd" ) 
   {
      subCommandCd( $stringValue );
   }
   elsif ( $stringParameter eq "Exec" ) 
   {
      if ( $bDryRun )
      {
         $nTests++;
      }
      else
      {
         subCommandExec( $stringValue );
      }
   }
   else 
   {
      subSetVariable( $stringParameter, $stringValue );
   }
}

#  \brief   Process the given ini section.
# 
#           We process each section parameter sequentially.
#
#  \param   0 An ini object created from Config::IniFiles().
#  \param   1 The section name.
#
sub subProcessSection
{
   my $objectIni      = $_[0];
   my $stringSection  = $_[1];

   foreach ( $objectIni->Parameters( $stringSection ) )
   {
      subProcessParameter( $objectIni, $stringSection, $_ );
   }
}

#  \brief   Process the given ini file.
#
#           We process the ini file sequentially -  top to bottom. The section
#           names are not important - not even the config section name. We simply
#           execute key/value pairs which are commands and store all other key/value
#           pairs as variables.
#
#           Sections do not define any kind of scope - every variable in a file has
#           the same scope.
#
#  \param   0 An ini object created from Config::IniFiles().
#
sub subProcessIni
{
   my $objectIni     = $_[0];
   my $hashVariables = {};

   push( @arrayScope, $hashVariables );

   foreach ( $objectIni->Sections )
   {
      if ( subGetVariable( "Debug" ) ) 
      {
         print( $_ ."\n" );
      }
      subProcessSection( $objectIni, $_ );
   }

   pop( @arrayScope );
}

# \brief Processes a given directory.
#
#        We change to the given dir and process the index.ini.
#
# \param 0 String with dir name to process.
# 
sub subCommandCd
{
   my $objectIni;
   my $stringDir = getcwd();
  
   chdir $_[0] or die "[ERROR] Can't cd to $_[0]: $!\n";

   if ( !defined( $objectIni = new Config::IniFiles( -file => "index.ini" ) ) ) 
   {
      chdir $stringDir or die "[ERROR] Can't cd to $stringDir: $!\n";
      return;
   }

   subProcessIni( $objectIni );

   chdir $stringDir or die "[ERROR] Can't cd to $stringDir: $!\n";
}

#  \brief   Process command-line args.
#
#           At the moment we assume we are only getting key=value pairs.
#           
sub subCommandLineArgs
{
   my $nLast = $#ARGV;
   my $n;

   for ( $n = $nLast; $n >= 0; $n-- ) 
   {
      my @sParts = split(  '=', $ARGV[$n] );

      if ( $#sParts >= 1 ) 
      {
         subSetVariable( $sParts[0], $sParts[1] );
      }
   }
}

# \brief Our "main".
#
#        This is the "main" entry point for this program - it all
#        starts here.
#
# \param Zero or more Key/Value pairs to prime our scoped vars with. Each
#        string should be in the form "key=value". For example;
#
# \code
#        $ ./tst "UID=testuid" "PWD=testpwd"
# \endcode
#
{
   my $hashVariables = {};

   # Create a global variable scope...
   push( @arrayScope, $hashVariables );

   # Set some defaults at global scope...
   subSetVariable( "Name", "." );
   subSetVariable( "ResultFormat", $RESULTS_FORMAT_TAP );
   subSetVariable( "Debug", 0 );

   # process args here...
   subCommandLineArgs();

   # Dry run to count tests only...
   # Start exec with index.ini in current dir...
   my $nResultFormat = subGetVariable( "ResultFormat" ) + 0;
   if ( $nResultFormat eq $RESULTS_FORMAT_TAP ) 
   {
      subCommandCd( "." );
      if ( $nTests ) 
      {
         print( sprintf( "1..%d\n", $nTests ) );
      }
      else
      {
         print( sprintf( "0..%d\n", $nTests ) );
      }
   }

   # Now really run tests...
   $bDryRun = 0;
   # Start exec with index.ini in current dir...
   subCommandCd( "." );

   # Free global variable scope...
   pop( @arrayScope );
}

