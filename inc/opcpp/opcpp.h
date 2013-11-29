///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: opCPP.h
/// Date: 11/26/2006
///
/// Description:
///
/// opC++ base header.
///****************************************************************

#pragma once

// system
#include <fstream>
#include <iomanip>
#include <time.h>

// configuration
#include "opcpp/config.h"

// opstl
#include "opcpp/opstl/opstl.h"

// boost
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/fstream.hpp>
using boost::filesystem::path;

// opcpp
#include "opcpp/time.h"
#include "opcpp/macros.h"
#include "opcpp/platforms.h"
#include "opcpp/file.h"
#include "opcpp/namespaces.h"
#include "opcpp/delegates.h"
#include "opcpp/exceptions.h"
#include "opcpp/memory_tracker.h"
#include "opcpp/parameters.h"
#include "opcpp/stream.h"
#include "opcpp/demo.h"
#include "opcpp/tokens.h"
#include "opcpp/log.h"
#include "opcpp/beta.h"
#include "opcpp/version.h"
#include "opcpp/error.h"
#include "opcpp/scanner.h"
#include "opcpp/symbol_tracker.h"
#include "opcpp/visitors.h"
#include "opcpp/node.h"
#include "opcpp/utility_interfaces.h"
#include "opcpp/interfaces.h"
#include "opcpp/statement_interfaces.h"
#include "opcpp/dialect_interfaces.h"
#include "opcpp/dialect_statement_interfaces.h"
#include "opcpp/contexts.h"
#include "opcpp/basic_nodes.h"
#include "opcpp/file_nodes.h"
#include "opcpp/dialect_tracker.h"
#include "opcpp/dialect_nodes.h"
#include "opcpp/modifier_nodes.h"
#include "opcpp/dialect_modifier_nodes.h"
#include "opcpp/extension_nodes.h"
#include "opcpp/macro_nodes.h"
#include "opcpp/statement_nodes.h"
#include "opcpp/dialect_statement_nodes.h"
#include "opcpp/node_inlines.h"
#include "opcpp/paths.h"
#include "opcpp/driver.h"
#include "opcpp/inlines.h"
#include "opcpp/globber.h"
#include "opcpp/mappings.h"
#include "opcpp/modifiers.h"
#include "opcpp/timer.h"
#include "opcpp/regex_support.h"
#include "opcpp/macro_interfaces.h"
#include "opcpp/walkers.h"
#include "opcpp/dialect_visitors.h"
#include "opcpp/code_visitors.h"
#include "opcpp/interfaces_inlines.h"
#include "opcpp/statement_interfaces_inlines.h"



