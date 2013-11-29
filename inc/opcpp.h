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
#include "config.h"

// opstl
#include "opstl.h"

// boost
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/fstream.hpp>
using boost::filesystem::path;

// opcpp
#include "time.h"
#include "macros.h"
#include "platforms.h"
#include "file.h"
#include "namespaces.h"
#include "delegates.h"
#include "exceptions.h"
#include "memory_tracker.h"
#include "parameters.h"
#include "stream.h"
#include "demo.h"
#include "tokens.h"
#include "log.h"
#include "beta.h"
#include "version.h"
#include "error.h"
#include "scanner.h"
#include "symbol_tracker.h"
#include "visitors.h"
#include "node.h"
#include "utility_interfaces.h"
#include "interfaces.h"
#include "statement_interfaces.h"
#include "dialect_interfaces.h"
#include "dialect_statement_interfaces.h"
#include "contexts.h"
#include "basic_nodes.h"
#include "file_nodes.h"
#include "dialect_tracker.h"
#include "dialect_nodes.h"
#include "modifier_nodes.h"
#include "dialect_modifier_nodes.h"
#include "extension_nodes.h"
#include "macro_nodes.h"
#include "statement_nodes.h"
#include "dialect_statement_nodes.h"
#include "node_inlines.h"
#include "paths.h"
#include "driver.h"
#include "inlines.h"
#include "globber.h"
#include "mappings.h"
#include "modifiers.h"
#include "timer.h"
#include "interface_inlines.h"
#include "regex_support.h"
#include "macro_interfaces.h"
#include "walkers.h"
#include "dialect_visitors.h"
#include "code_visitors.h"



