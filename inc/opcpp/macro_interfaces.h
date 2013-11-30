///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: MacroInterfaces.h
/// Date: 10/04/2007
///
/// Description:
///
/// Macro interfaces.
///****************************************************************

///==========================================
/// MacroConcatenations
///==========================================

template<class Parent>
class MacroConcatenations : public Parent
{
public:
	IMPLEMENTS_INTERFACE(MacroConcatenations)

	// Finds ID@ID
	void FindConcatenations();
};

///==========================================
/// MacroSingleQuotes
///==========================================

template<class Parent>
class MacroSingleQuotes : public Parent
{
public:
	IMPLEMENTS_INTERFACE(MacroSingleQuotes)

	// Finds `text`
	void FindSingleQuotes();
};

///==========================================
/// MacroDoubleQuotes
///==========================================

template<class Parent>
class MacroDoubleQuotes : public Parent
{
public:
	IMPLEMENTS_INTERFACE(MacroDoubleQuotes)

	// Finds ``text``
	void FindDoubleQuotes();
};
