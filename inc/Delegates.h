///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Delegates.h
/// Date: 10/05/2007
///
/// Description:
///
/// Delegate Types
///****************************************************************

//include fast delegate
// #include "../../Lib/FastDelegate/FastDelegate.h"
#include <boost/function.hpp>

namespace delegates
{
    // kevin: Fix this!
	// typedef fastdelegate::FastDelegate< opNode* (const opString& modifiername) > ModifierDelegate;
    typedef boost::function< opNode* (const opString& modifiername) > ModifierDelegate;
}
