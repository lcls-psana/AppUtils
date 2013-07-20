//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id$
//
// Description:
//	Class AppCmdOptIncr
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

//-----------------------
// This Class's Header --
//-----------------------
#include "AppUtils/AppCmdOptIncr.h"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

namespace AppUtils {

AppCmdOptIncr::AppCmdOptIncr ( char shortOpt,
			       const std::string& longOpt,
			       const std::string& descr,
			       int defValue )
  : AppCmdOptBase(longOpt+","+std::string(1, shortOpt), "(incr)", descr)
  , _value(defValue)
  , _defValue(defValue)
  , _changed(false)
{
}

AppCmdOptIncr::AppCmdOptIncr ( const std::string& optNames,
                               const std::string& descr,
                               int defValue )
  : AppCmdOptBase(optNames, "(incr)", descr)
  , _value(defValue)
  , _defValue(defValue)
  , _changed(false)
{
}

AppCmdOptIncr::AppCmdOptIncr ( char shortOpt,
                               const std::string& descr,
                               int defValue )
  : AppCmdOptBase(std::string(1, shortOpt), "(incr)", descr)
  , _value(defValue)
  , _defValue(defValue)
  , _changed(false)
{
}

// Destructor
AppCmdOptIncr::~AppCmdOptIncr( )
{
}

/**
 *  Returns true if option requires argument. Does not make sense for
 *  positional arguments.
 */
bool
AppCmdOptIncr::hasArgument() const
{
  return false ;
}

/**
 *  Set the value of the argument.
 */
void
AppCmdOptIncr::setValue ( const std::string& value )
{
  ++ _value ;
  _changed = true ;
}

/**
 *  True if the value of the option was changed from command line.
 */
bool
AppCmdOptIncr::valueChanged () const
{
  return _changed ;
}

/**
 *  Return current value of the argument
 */
int
AppCmdOptIncr::value() const
{
  return _value ;
}

/**
 *  reset option to its default value
 */
void
AppCmdOptIncr::reset()
{
  _value = _defValue ;
  _changed = false ;
}

} // namespace AppUtils
