///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: opCPPSource.cpp
/// Date: 11/26/2006
///
/// Description:
///
/// This file is the source code "glob" file.  It merges all .cpp's into one 
/// before compiling.  This dramatically reduces compile times.
///****************************************************************

#include "opcpp/opcpp.h"

//
// OPCPP Source Glob
//

#ifdef PLATFORM_WINDOWS
	//OPCPP Related:
	//print a compiling message to stdout ?
	#define OPCOMPILING_SOURCE(filestr)	\
		__pragma(message(filestr))

	#define OPCOMPILING_SOURCE_START \
		__pragma(message(" :::::::::::"))

	#define OPCOMPILING_SOURCE_END \
		__pragma(message(" =========== "))

#else
	#define OPCOMPILING_SOURCE(filestr) 
	#define OPCOMPILING_SOURCE_START 
	#define OPCOMPILING_SOURCE_END 
#endif

OPCOMPILING_SOURCE_START;

OPCOMPILING_SOURCE("opcpp/beta.cpp");
#include "opcpp/beta.cpp"

OPCOMPILING_SOURCE("opcpp/version.cpp");
#include "opcpp/version.cpp"

OPCOMPILING_SOURCE("opcpp/platforms.cpp");
#include "opcpp/platforms.cpp"

OPCOMPILING_SOURCE("opcpp/error.cpp");
#include "opcpp/error.cpp"

OPCOMPILING_SOURCE("opcpp/tokens.cpp");
#include "opcpp/tokens.cpp"

OPCOMPILING_SOURCE("opcpp/driver.cpp");
#include "opcpp/driver.cpp"

OPCOMPILING_SOURCE("opcpp/stream.cpp");
#include "opcpp/stream.cpp"

OPCOMPILING_SOURCE("opcpp/node.cpp");
#include "opcpp/node.cpp"

OPCOMPILING_SOURCE("opcpp/symbol_tracker.cpp");
#include "opcpp/symbol_tracker.cpp"

OPCOMPILING_SOURCE("opcpp/pre_parse.cpp");
#include "opcpp/pre_parse.cpp"

OPCOMPILING_SOURCE("opcpp/parameters.cpp");
#include "opcpp/parameters.cpp"

OPCOMPILING_SOURCE("opcpp/exceptions.cpp");
#include "opcpp/exceptions.cpp"

OPCOMPILING_SOURCE("opcpp/parse.cpp");
#include "opcpp/parse.cpp"

OPCOMPILING_SOURCE("opcpp/post_parse.cpp");
#include "opcpp/post_parse.cpp"

OPCOMPILING_SOURCE("opcpp/print.cpp");
#include "opcpp/print.cpp"

OPCOMPILING_SOURCE("opcpp/print_notes.cpp")
#include "opcpp/print_notes.cpp"

OPCOMPILING_SOURCE("opcpp/print_maps.cpp")
#include "opcpp/print_maps.cpp"

OPCOMPILING_SOURCE("opcpp/file_nodes.cpp");
#include "opcpp/file_nodes.cpp"

OPCOMPILING_SOURCE("opcpp/statement_nodes.cpp");
#include "opcpp/statement_nodes.cpp"

OPCOMPILING_SOURCE("opcpp/modifier_nodes.cpp");
#include "opcpp/modifier_nodes.cpp"

OPCOMPILING_SOURCE("opcpp/dialect_modifier_nodes.cpp");
#include "opcpp/dialect_modifier_nodes.cpp"

OPCOMPILING_SOURCE("opcpp/extension_nodes.cpp");
#include "opcpp/extension_nodes.cpp"

OPCOMPILING_SOURCE("opcpp/macro_nodes.cpp")
#include "opcpp/macro_nodes.cpp"

OPCOMPILING_SOURCE("opcpp/scanner.cpp");
#include "opcpp/scanner.cpp"

OPCOMPILING_SOURCE("opcpp/timer.cpp");
#include "opcpp/timer.cpp"

OPCOMPILING_SOURCE("opcpp/dialect_nodes.cpp");
#include "opcpp/dialect_nodes.cpp"

OPCOMPILING_SOURCE("opcpp/print_dialect.cpp");
#include "opcpp/print_dialect.cpp"

OPCOMPILING_SOURCE("opcpp/globber.cpp")
#include "opcpp/globber.cpp"

OPCOMPILING_SOURCE("opcpp/memory_tracker.cpp")
#include "opcpp/memory_tracker.cpp"

OPCOMPILING_SOURCE("opcpp/dialect_tracker.cpp")
#include "opcpp/dialect_tracker.cpp"

OPCOMPILING_SOURCE("opcpp/states.cpp")
#include "opcpp/states.cpp"

OPCOMPILING_SOURCE("opcpp/error_names.cpp")
#include "opcpp/error_names.cpp"

OPCOMPILING_SOURCE("opcpp/main.cpp");
#include "opcpp/main.cpp"

OPCOMPILING_SOURCE("opcpp/demo.cpp");
#include "opcpp/demo.cpp"

OPCOMPILING_SOURCE("opcpp/blacklist.cpp");
#include "opcpp/blacklist.cpp"

OPCOMPILING_SOURCE("opcpp/modifiers.cpp");
#include "opcpp/modifiers.cpp"

OPCOMPILING_SOURCE_END;


