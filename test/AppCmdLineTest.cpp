//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id$
//
// Description:
//	Test suite case for the AppCmdLine & friends.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Andy Salnikov		originator
//
// Copyright Information:
//	Copyright (C) 2003	SLAC
//
//------------------------------------------------------------------------

//---------------
// C++ Headers --
//---------------
#include <string>
#include <iostream>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AppUtils/AppCmdLine.h"
#include "AppUtils/AppCmdArg.h"
#include "AppUtils/AppCmdArgList.h"
#include "AppUtils/AppCmdExceptions.h"
#include "AppUtils/AppCmdOpt.h"
#include "AppUtils/AppCmdOptBool.h"
#include "AppUtils/AppCmdOptIncr.h"
#include "AppUtils/AppCmdOptList.h"
#include "AppUtils/AppCmdOptSize.h"
#include "AppUtils/AppCmdOptToggle.h"
#include "AppUtils/AppCmdOptNamedValue.h"

using namespace AppUtils ;


#define BOOST_TEST_MODULE AppCmdLineTest
#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_CASE( cmdline_test_simple )
{
  AppCmdLine cmdline("command") ;

  // create a bunch of arguments and add them
  AppCmdArg<std::string> argString1( "name", "specifies the name" ) ;
  BOOST_CHECK_NO_THROW( cmdline.addArgument ( argString1 ) ) ;

  AppCmdArg<int> argInt1( "number", "specifies the number" ) ;
  BOOST_CHECK_NO_THROW( cmdline.addArgument ( argInt1 ) ) ;

  AppCmdArg<int> argInt2( "number", "specifies the number 2", 1000 ) ;
  BOOST_CHECK_NO_THROW( cmdline.addArgument ( argInt2 ) ) ;

  // make a command line
  std::vector<std::string> args ;
  args.push_back ( "MyName" ) ;
  args.push_back ( "12345" ) ;

  // first try, string for optional argument is missing
  BOOST_CHECK_NO_THROW ( cmdline.parse ( args.begin(), args.end() ) );
  BOOST_CHECK_EQUAL ( argString1.value(), "MyName" ) ;
  BOOST_CHECK_EQUAL ( argInt1.value(), 12345 ) ;
  BOOST_CHECK_EQUAL ( argInt1.valueChanged(), true ) ;
  BOOST_CHECK_EQUAL ( argInt2.value(), 1000 ) ;
  BOOST_CHECK_EQUAL ( argInt2.valueChanged(), false ) ;

  // add data for optional argument
  args.push_back ( "123" ) ;

  BOOST_CHECK_NO_THROW ( cmdline.parse ( args.begin(), args.end() ) ) ;
  BOOST_CHECK_EQUAL ( argString1.value(), "MyName" ) ;
  BOOST_CHECK_EQUAL ( argInt1.value(), 12345 ) ;
  BOOST_CHECK_EQUAL ( argInt2.value(), 123 ) ;
  BOOST_CHECK_EQUAL ( argInt2.valueChanged(), true ) ;

  // add more options
  AppCmdOptIncr optVerbose ( "v,verbose", "more noise", 0 ) ;
  BOOST_CHECK_NO_THROW ( cmdline.addOption ( optVerbose ) ) ;

  AppCmdOptToggle optToggle ( "t,toggle", "toggle something", false ) ;
  BOOST_CHECK_NO_THROW ( cmdline.addOption ( optToggle ) ) ;

  AppCmdOpt<int> optInt1 ( "i,int", "number", "some number", 123 ) ;
  BOOST_CHECK_NO_THROW ( cmdline.addOption ( optInt1 ) ) ;

  AppCmdOpt<int> optInt2 ( "I,INT", "NUMBER", "some number", 123 ) ;
  BOOST_CHECK_NO_THROW ( cmdline.addOption ( optInt2 ) ) ;

  AppCmdOpt<std::string> optString1 ( "s,string", "astring", "some string", "<none>" ) ;
  BOOST_CHECK_NO_THROW ( cmdline.addOption ( optString1 ) ) ;

  AppCmdOpt<std::string> optString2 ( "S,STRING", "Astring", "some string", "<none>" ) ;
  BOOST_CHECK_NO_THROW ( cmdline.addOption ( optString2 ) ) ;

  AppCmdOpt<std::string> optString3 ( "d,dummy", "Astring", "some string", "<none>" ) ;
  BOOST_CHECK_NO_THROW ( cmdline.addOption ( optString3 ) ) ;

  // new command line
  args.clear() ;
  args.push_back ( "-v" ) ;
  args.push_back ( "--verbose" ) ;
  args.push_back ( "-t" ) ;
  args.push_back ( "--toggle" ) ;
  args.push_back ( "-vvvt" ) ;
  args.push_back ( "--int=654" ) ;
  args.push_back ( "--INT" ) ;
  args.push_back ( "654" ) ;
  args.push_back ( "-sNone" ) ;
  args.push_back ( "-S" ) ;
  args.push_back ( "NONE" ) ;
  args.push_back ( "--" ) ;
  args.push_back ( "MyName" ) ;
  args.push_back ( "12345" ) ;

  // first try, string for optional argument is mising
  BOOST_CHECK_NO_THROW ( cmdline.parse ( args.begin(), args.end() ) ) ;
  BOOST_CHECK_EQUAL ( optVerbose.value(), 5 ) ;
  BOOST_CHECK_EQUAL ( optToggle.value(), true ) ;
  BOOST_CHECK_EQUAL ( optInt1.value(), 654 ) ;
  BOOST_CHECK_EQUAL ( optInt2.value(), 654 ) ;
  BOOST_CHECK_EQUAL ( optString1.value(), "None" ) ;
  BOOST_CHECK_EQUAL ( optString2.value(), "NONE" ) ;
  BOOST_CHECK_EQUAL ( optString2.valueChanged(), true ) ;
  BOOST_CHECK_EQUAL ( optString3.valueChanged(), false ) ;
}

// ==============================================================


BOOST_AUTO_TEST_CASE( cmdline_test_simple_newstyle )
{
  AppCmdLine cmdline("command") ;

  // create a bunch of arguments and add them
  AppCmdArg<std::string> argString1(cmdline, "name", "specifies the name");
  AppCmdArg<int> argInt1(cmdline, "number", "specifies the number");
  AppCmdArg<int> argInt2(cmdline, "number", "specifies the number 2", 1000 ) ;

  // make a command line
  std::vector<std::string> args ;
  args.push_back ( "MyName" ) ;
  args.push_back ( "12345" ) ;

  // first try, string for optional argument is missing
  BOOST_CHECK_NO_THROW ( cmdline.parse ( args.begin(), args.end() ) ) ;
  BOOST_CHECK_EQUAL ( argString1.value(), "MyName" ) ;
  BOOST_CHECK_EQUAL ( argInt1.value(), 12345 ) ;
  BOOST_CHECK_EQUAL ( argInt1.valueChanged(), true ) ;
  BOOST_CHECK_EQUAL ( argInt2.value(), 1000 ) ;
  BOOST_CHECK_EQUAL ( argInt2.valueChanged(), false ) ;

  // add data for optional argument
  args.push_back ( "123" ) ;

  BOOST_CHECK_NO_THROW ( cmdline.parse ( args.begin(), args.end() ) ) ;
  BOOST_CHECK_EQUAL ( argString1.value(), "MyName" ) ;
  BOOST_CHECK_EQUAL ( argInt1.value(), 12345 ) ;
  BOOST_CHECK_EQUAL ( argInt2.value(), 123 ) ;
  BOOST_CHECK_EQUAL ( argInt2.valueChanged(), true ) ;

  // add more options
  AppCmdOptIncr optVerbose (cmdline, "verbose,v,V,VERB", "more noise", 0 ) ;

  AppCmdOptToggle optToggle (cmdline, "toggle,t", "toggle something", false ) ;

  AppCmdOpt<int> optInt1 (cmdline, "int,i", "number", "some number", 123 ) ;

  AppCmdOpt<int> optInt2 (cmdline, "I,INT", "NUMBER", "some number", 123 ) ;

  AppCmdOpt<std::string> optString1 (cmdline, "s,string", "astring", "some string", "<none>" ) ;

  AppCmdOpt<std::string> optString2 (cmdline, "STRING,S", "Astring", "some string", "<none>" ) ;

  AppCmdOpt<std::string> optString3 (cmdline, "dummy", "Astring", "some string", "<none>" ) ;

  // new command line
  args.clear() ;
  args.push_back ( "-v" ) ;
  args.push_back ( "--verbose" ) ;
  args.push_back ( "--VERB" ) ;
  args.push_back ( "-t" ) ;
  args.push_back ( "--toggle" ) ;
  args.push_back ( "-vVt" ) ;
  args.push_back ( "--int=654" ) ;
  args.push_back ( "--INT" ) ;
  args.push_back ( "654" ) ;
  args.push_back ( "-sNone" ) ;
  args.push_back ( "-S" ) ;
  args.push_back ( "NONE" ) ;
  args.push_back ( "--" ) ;
  args.push_back ( "MyName" ) ;
  args.push_back ( "12345" ) ;

  // first try, string for optional argument is mising
  BOOST_CHECK_NO_THROW ( cmdline.parse ( args.begin(), args.end() ) ) ;
  BOOST_CHECK_EQUAL ( optVerbose.value(), 5 ) ;
  BOOST_CHECK_EQUAL ( optToggle.value(), true ) ;
  BOOST_CHECK_EQUAL ( optInt1.value(), 654 ) ;
  BOOST_CHECK_EQUAL ( optInt2.value(), 654 ) ;
  BOOST_CHECK_EQUAL ( optString1.value(), "None" ) ;
  BOOST_CHECK_EQUAL ( optString2.value(), "NONE" ) ;
  BOOST_CHECK_EQUAL ( optString2.valueChanged(), true ) ;
  BOOST_CHECK_EQUAL ( optString3.valueChanged(), false ) ;
}


// ==============================================================

// check for argument count
BOOST_AUTO_TEST_CASE( cmdline_test_except_args_count )
{
  AppCmdLine cmdline("command") ;

  // create a bunch of arguments and add them
  AppCmdArg<std::string> argString1(cmdline, "name", "specifies the name");
  AppCmdArg<int> argInt1(cmdline, "number", "specifies the number");

  // make a command line
  std::vector<std::string> args ;
  args.push_back ( "MyName" ) ;

  // too few positional parameters yet
  BOOST_CHECK_THROW ( cmdline.parse ( args.begin(), args.end() ), AppCmdArgListTooShort ) ;

  // two is OK
  args.push_back ( "12345" ) ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( args.begin(), args.end() ) ) ;

  // add one more, parse() will fail
  args.push_back ( "???" ) ;
  BOOST_CHECK_THROW ( cmdline.parse ( args.begin(), args.end() ), AppCmdArgListTooLong ) ;
}

// ==============================================================

// check for argument order conflict
BOOST_AUTO_TEST_CASE( cmdline_test_except_args_order )
{
  AppCmdLine cmdline("command") ;

  // create a bunch of arguments and add them
  AppCmdArg<std::string> argString1(cmdline, "name", "specifies the name", "");
  AppCmdArg<int> argInt1(cmdline, "number", "specifies the number");

  // should fail on checking arguments
  std::vector<std::string> args ;
  BOOST_CHECK_THROW ( cmdline.parse ( args.begin(), args.end() ), AppCmdArgOrderException ) ;
}

// ==============================================================

// check for option conflicts with short options
BOOST_AUTO_TEST_CASE( cmdline_test_except_duplicate_opt_short )
{
  AppCmdLine cmdline("command") ;

  // make few options
  AppCmdOpt<std::string> optString1 (cmdline, "1,string1", "astring", "some string", "" ) ;
  AppCmdOpt<std::string> optString2 (cmdline, "2,string2", "astring", "some string", "" ) ;
  AppCmdOpt<std::string> optString3 (cmdline, "1,string3", "astring", "some string", "" ) ;

  std::vector<std::string> args ;
  BOOST_CHECK_THROW ( cmdline.parse ( args.begin(), args.end() ), AppCmdOptDefinedException ) ;
}

// ==============================================================

// check for option conflicts with long options
BOOST_AUTO_TEST_CASE( cmdline_test_except_duplicate_opt_long )
{
  AppCmdLine cmdline("command") ;

  // make few options
  AppCmdOpt<std::string> optString1 (cmdline, "1,string1", "astring", "some string", "" ) ;
  AppCmdOpt<std::string> optString2 (cmdline, "2,string2", "astring", "some string", "" ) ;
  AppCmdOpt<std::string> optString3 (cmdline, "3,string1", "astring", "some string", "" ) ;

  std::vector<std::string> args ;
  BOOST_CHECK_THROW ( cmdline.parse ( args.begin(), args.end() ), AppCmdOptDefinedException ) ;
}

// ==============================================================

// check few conversions to numeric types
BOOST_AUTO_TEST_CASE( cmdline_test_cvt )
{
  AppCmdLine cmdline("command") ;

  const char* args[5] = { "" } ;

  // try few conversions
  AppCmdOpt<int> optInt1 ( "i,int1", "number", "some number", 0 ) ;
  AppCmdOpt<float> optFloat1 ( "f,float1", "number", "some number", 0. ) ;
  AppCmdOpt<double> optDouble1 ( "d,double1", "number", "some number", 0. ) ;

  BOOST_CHECK_NO_THROW ( cmdline.addOption( optInt1 ) ) ;
  BOOST_CHECK_NO_THROW ( cmdline.addOption( optFloat1 ) ) ;
  BOOST_CHECK_NO_THROW ( cmdline.addOption( optDouble1 ) ) ;

  // test integer conversions
  args[1] = "-i" ;
  args[2] = "1000" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 3, args ) ) ;
  BOOST_CHECK_EQUAL ( optInt1.value(), 1000 ) ;

  args[2] = "-1000" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 3, args ) ) ;
  BOOST_CHECK_EQUAL ( optInt1.value(), -1000 ) ;

  args[2] = "0x1000" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 3, args ) ) ;
  BOOST_CHECK_EQUAL ( optInt1.value(), 0x1000 ) ;

  args[2] = "01000" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 3, args ) ) ;
  BOOST_CHECK_EQUAL ( optInt1.value(), 01000 ) ;

  args[2] = "1000000000000000" ;
  BOOST_CHECK_THROW ( cmdline.parse ( 3, args ), AppCmdException ) ;

  args[2] = "" ;
  BOOST_CHECK_THROW ( cmdline.parse ( 3, args ), AppCmdException ) ;

  args[2] = "abcd" ;
  BOOST_CHECK_THROW ( cmdline.parse ( 3, args ), AppCmdException ) ;

  args[2] = "100x" ;
  BOOST_CHECK_THROW ( cmdline.parse ( 3, args ), AppCmdException ) ;

  args[2] = "0x1000_" ;
  BOOST_CHECK_THROW ( cmdline.parse ( 3, args ), AppCmdException ) ;


  // test float conversions
  args[1] = "-f" ;
  args[2] = "1000" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 3, args ) ) ;
  BOOST_CHECK_EQUAL ( optFloat1.value(), 1000.f ) ;

  args[2] = "0.00000001" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 3, args ) ) ;
  BOOST_CHECK_EQUAL ( optFloat1.value(), 0.00000001f ) ;

  args[2] = "-0.00000001" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 3, args ) ) ;
  BOOST_CHECK_EQUAL ( optFloat1.value(), -0.00000001f ) ;

  args[2] = "1.e10" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 3, args ) ) ;
  BOOST_CHECK_EQUAL ( optFloat1.value(), 1e10f ) ;

  args[2] = "1.e-20" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 3, args ) ) ;
  BOOST_CHECK_EQUAL ( optFloat1.value(), 1e-20f ) ;

  args[2] = "0.0002e" ;
  BOOST_CHECK_THROW ( cmdline.parse ( 3, args ), AppCmdException ) ;

  args[2] = "1e+128" ;
  BOOST_CHECK_THROW ( cmdline.parse ( 3, args ), AppCmdException ) ;

  args[2] = "1e-128" ;
  BOOST_CHECK_THROW ( cmdline.parse ( 3, args ), AppCmdException ) ;

  // test double conversions
  args[1] = "-d" ;
  args[2] = "1e+128" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 3, args ) ) ;
  BOOST_CHECK_EQUAL ( optDouble1.value(), 1e+128 ) ;

  args[2] = "1e-128" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 3, args ) ) ;
  BOOST_CHECK_EQUAL ( optDouble1.value(), 1e-128 ) ;

  args[2] = "1e+1000" ;
  BOOST_CHECK_THROW ( cmdline.parse ( 3, args ), AppCmdException ) ;

  args[2] = "1e-1000" ;
  BOOST_CHECK_THROW ( cmdline.parse ( 3, args ), AppCmdException ) ;

  args[2] = "NAN" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 3, args ) ) ;

  args[2] = "INF" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 3, args ) ) ;

  args[2] = "+INF" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 3, args ) ) ;

  args[2] = "-INF" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 3, args ) ) ;
}

// ==============================================================

BOOST_AUTO_TEST_CASE( cmdline_test_strlist )
{

  // Install one more cmd line parser, cannot fail
  AppCmdLine cmdline1( "command1" ) ;

  // create a bunch of arguments and add them
  AppCmdArgList<std::string> argStringL( "names", "specifies the name(s)" ) ;
  BOOST_CHECK_NO_THROW ( cmdline1.addArgument ( argStringL ) ) ;

  AppCmdOptList<std::string> optStringL( "n,name", "string", "specifies the name(s)" ) ;
  BOOST_CHECK_NO_THROW ( cmdline1.addOption ( optStringL ) ) ;

  std::vector<std::string> args ;
  args.push_back ( "-nname1" ) ;
  args.push_back ( "-n" ) ;
  args.push_back ( "name2" ) ;
  args.push_back ( "--name=name3" ) ;
  args.push_back ( "--name" ) ;
  args.push_back ( "name4" ) ;
  args.push_back ( "--name" ) ;
  args.push_back ( "name5,name6,name7,name8" ) ;
  args.push_back ( "name1" ) ;
  args.push_back ( "name2" ) ;
  args.push_back ( "name3" ) ;

  BOOST_CHECK_NO_THROW ( cmdline1.parse ( args.begin(), args.end() ) ) ;

  AppCmdOptList<std::string>::const_iterator obegin = optStringL.begin() ;
  BOOST_CHECK_EQUAL( *obegin, "name1" ) ;
  ++ obegin ;
  BOOST_CHECK_EQUAL( *obegin, "name2" ) ;
  ++ obegin ;
  BOOST_CHECK_EQUAL( *obegin, "name3" ) ;
  ++ obegin ;
  BOOST_CHECK_EQUAL( *obegin, "name4" ) ;
  ++ obegin ;
  BOOST_CHECK_EQUAL( *obegin, "name5" ) ;
  ++ obegin ;
  BOOST_CHECK_EQUAL( *obegin, "name6" ) ;
  ++ obegin ;
  BOOST_CHECK_EQUAL( *obegin, "name7" ) ;
  ++ obegin ;
  BOOST_CHECK_EQUAL( *obegin, "name8" ) ;
  ++ obegin ;
  BOOST_CHECK( obegin == optStringL.end() ) ;

  AppCmdArgList<std::string>::const_iterator begin = argStringL.begin() ;
  BOOST_CHECK_EQUAL( *begin, "name1" ) ;
  ++ begin ;
  BOOST_CHECK_EQUAL( *begin, "name2" ) ;
  ++ begin ;
  BOOST_CHECK_EQUAL( *begin, "name3" ) ;
  ++ begin ;
  BOOST_CHECK( begin == argStringL.end() ) ;

  // print usage info about command
  //cmdline1.usage ( std::cout ) ;
}

// ==============================================================

BOOST_AUTO_TEST_CASE( cmdline_test_helpopt )
{
  // Install one more cmd line parser, cannot fail
  AppCmdLine cmdline1( "command1" ) ;

  // check how help options work
  const char* args[5] = { "" } ;

  args[1] = "--help" ;
  BOOST_CHECK_NO_THROW( cmdline1.parse ( 2, args ) ) ;
  BOOST_CHECK( cmdline1.helpWanted() ) ;

  args[1] = "-?" ;
  BOOST_CHECK_NO_THROW( cmdline1.parse ( 2, args ) ) ;
  BOOST_CHECK( cmdline1.helpWanted() ) ;

  args[1] = "-h" ;
  BOOST_CHECK_NO_THROW( cmdline1.parse ( 2, args ) ) ;
  BOOST_CHECK( cmdline1.helpWanted() ) ;

  // should not be able to add -h or --help
  AppCmdOptIncr optHelp ( cmdline1 ,"h,help", "gimme help", 0 ) ;
  BOOST_CHECK_THROW( cmdline1.parse ( 2, args ), AppCmdOptDefinedException ) ;
}

// ==============================================================

BOOST_AUTO_TEST_CASE( cmdline_test_intopt )
{
  // Install one more cmd line parser, cannot fail
  AppCmdLine cmdline2( "command2" ) ;

  AppCmdOpt<int> optInt21 ( "i,int", "number", "some number", 0 ) ;
  BOOST_CHECK_NO_THROW ( cmdline2.addOption ( optInt21 ) ) ;

  const char* args[5] = { "" } ;

  args[1] = "-i1" ;
  BOOST_CHECK_NO_THROW ( cmdline2.parse ( 2, args ) ) ;
  BOOST_CHECK_EQUAL ( optInt21.value(), 1 ) ;

  args[1] = "-i-1" ;
  BOOST_CHECK_NO_THROW ( cmdline2.parse ( 2, args ) ) ;
  BOOST_CHECK_EQUAL ( optInt21.value(), -1 ) ;

  args[1] = "-i-i" ;
  BOOST_CHECK_THROW ( cmdline2.parse ( 2, args ), AppCmdException ) ;

}

// ==============================================================

BOOST_AUTO_TEST_CASE( cmdline_test_named )
{
  // Install one more cmd line parser, cannot fail
  AppCmdLine cmdline3( "command3" ) ;

  AppCmdOptNamedValue<int> optInt31 ( "o,option", "string", "one of the zero, one, two", 0 ) ;
  optInt31.add ( "zero", 0 ) ;
  optInt31.add ( "one", 1 ) ;
  optInt31.add ( "two", 2 ) ;
  BOOST_CHECK_NO_THROW ( cmdline3.addOption ( optInt31 ) ) ;

  const char* args[5] = { "" } ;

  args[1] = "-ozero" ;
  BOOST_CHECK_NO_THROW ( cmdline3.parse ( 2, args ) ) ;
  BOOST_CHECK_EQUAL ( optInt31.value(), 0 ) ;

  args[1] = "--option=one" ;
  BOOST_CHECK_NO_THROW ( cmdline3.parse ( 2, args ) ) ;
  BOOST_CHECK_EQUAL ( optInt31.value(), 1 ) ;

  args[1] = "-othree" ;
  BOOST_CHECK_THROW ( cmdline3.parse ( 2, args ), AppCmdException ) ;

}

// ==============================================================

BOOST_AUTO_TEST_CASE( cmdline_test_bool )
{
  // Install one more cmd line parser, cannot fail
  AppCmdLine cmdline( "command" ) ;

  AppCmdOptBool optBool ( "b,bool", "on/off", false ) ;
  BOOST_CHECK_NO_THROW ( cmdline.addOption ( optBool ) ) ;

  const char* args[5] = { "" } ;

  BOOST_CHECK_NO_THROW ( cmdline.parse ( 1, args ) ) ;
  BOOST_CHECK_EQUAL ( optBool.value(), false ) ;

  args[1] = "-b" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 2, args ) ) ;
  BOOST_CHECK_EQUAL ( optBool.value(), true ) ;

  args[2] = "-b" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 3, args ) ) ;
  BOOST_CHECK_EQUAL ( optBool.value(), true ) ;

  args[3] = "--bool" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 4, args ) ) ;
  BOOST_CHECK_EQUAL ( optBool.value(), true ) ;

}

BOOST_AUTO_TEST_CASE( cmdline_test_bool_neg )
{
  // Install one more cmd line parser, cannot fail
  AppCmdLine cmdline( "command" ) ;

  AppCmdOptBool optBool ( "b,bool", "on/off", true ) ;
  BOOST_CHECK_NO_THROW ( cmdline.addOption ( optBool ) ) ;

  const char* args[5] = { "" } ;

  BOOST_CHECK_NO_THROW ( cmdline.parse ( 1, args ) ) ;
  BOOST_CHECK_EQUAL ( optBool.value(), true ) ;

  args[1] = "-b" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 2, args ) ) ;
  BOOST_CHECK_EQUAL ( optBool.value(), false ) ;

  args[2] = "-b" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 3, args ) ) ;
  BOOST_CHECK_EQUAL ( optBool.value(), false ) ;

  args[3] = "--bool" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 4, args ) ) ;
  BOOST_CHECK_EQUAL ( optBool.value(), false ) ;

}

// ==============================================================

BOOST_AUTO_TEST_CASE( cmdline_test_size )
{
  // Install one more cmd line parser, cannot fail
  AppCmdLine cmdline( "command" ) ;

  AppCmdOptSize optSize ( "s,size", "number", "size format accepts nnn, nnnk, nnnM, nnnG", 0 ) ;
  BOOST_CHECK_NO_THROW ( cmdline.addOption ( optSize ) ) ;

  const char* args[5] = { "" } ;

  args[1] = "-s1000" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 2, args ) ) ;
  BOOST_CHECK_EQUAL ( optSize.value(), 1000ULL ) ;

  args[1] = "--size=1000k" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 2, args ) ) ;
  BOOST_CHECK_EQUAL ( optSize.value(), 1000*1024ULL ) ;

  args[1] = "--size=1000M" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 2, args ) ) ;
  BOOST_CHECK_EQUAL ( optSize.value(), 1000*1024*1024ULL ) ;

  args[1] = "--size=1000G" ;
  BOOST_CHECK_NO_THROW ( cmdline.parse ( 2, args ) ) ;
  BOOST_CHECK_EQUAL ( optSize.value(), 1000*1024*1024*1024ULL ) ;

}

// ==============================================================

BOOST_AUTO_TEST_CASE( cmdline_test_groups )
{
  // Install one more cmd line parser, cannot fail
  AppCmdLine cmdline( "command" ) ;
  AppCmdOptGroup group1( "Input options" ) ;
  BOOST_CHECK_NO_THROW ( cmdline.addGroup(group1) ) ;
  AppCmdOptGroup group2(cmdline, "Output options");

  AppCmdOptIncr optVerbose(cmdline, "v,verbose", "make more noise", 0 );
  AppCmdOpt<int> optInt1(group1, "x,number1", "number", "some number", 1 ) ;
  AppCmdOpt<int> optInt2(group2, "y,number2", "number", "some number", 2 ) ;

  const char* args[5] = { "" } ;

  args[1] = "-v" ;
  BOOST_CHECK_NO_THROW(cmdline.parse(2, args));
  BOOST_CHECK_EQUAL(optVerbose.value(), 1);
  BOOST_CHECK_EQUAL(optInt1.value(), 1);
  BOOST_CHECK_EQUAL(optInt2.value(), 2);

  args[1] = "--verbose" ;
  BOOST_CHECK_NO_THROW(cmdline.parse(2, args));
  BOOST_CHECK_EQUAL(optVerbose.value(), 1);
  BOOST_CHECK_EQUAL(optInt1.value(), 1);
  BOOST_CHECK_EQUAL(optInt2.value(), 2);

  args[1] = "-x100" ;
  BOOST_CHECK_NO_THROW(cmdline.parse(2, args));
  BOOST_CHECK_EQUAL(optVerbose.value(), 0);
  BOOST_CHECK_EQUAL(optInt1.value(), 100);
  BOOST_CHECK_EQUAL(optInt2.value(), 2);

  args[1] = "--number1=100" ;
  BOOST_CHECK_NO_THROW(cmdline.parse(2, args));
  BOOST_CHECK_EQUAL(optVerbose.value(), 0);
  BOOST_CHECK_EQUAL(optInt1.value(), 100);
  BOOST_CHECK_EQUAL(optInt2.value(), 2);

  args[1] = "-y200" ;
  BOOST_CHECK_NO_THROW(cmdline.parse(2, args));
  BOOST_CHECK_EQUAL(optVerbose.value(), 0);
  BOOST_CHECK_EQUAL(optInt1.value(), 1);
  BOOST_CHECK_EQUAL(optInt2.value(), 200);

  args[1] = "--number2=200" ;
  BOOST_CHECK_NO_THROW(cmdline.parse(2, args));
  BOOST_CHECK_EQUAL(optVerbose.value(), 0);
  BOOST_CHECK_EQUAL(optInt1.value(), 1);
  BOOST_CHECK_EQUAL(optInt2.value(), 200);

  args[1] = "-vvv" ;
  args[2] = "-x3" ;
  args[3] = "-y4" ;
  BOOST_CHECK_NO_THROW(cmdline.parse(4, args));
  BOOST_CHECK_EQUAL(optVerbose.value(), 3);
  BOOST_CHECK_EQUAL(optInt1.value(), 3);
  BOOST_CHECK_EQUAL(optInt2.value(), 4);
}
