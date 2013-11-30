///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Contexts.h
/// Date: 11/26/2006
///
/// Description:
///
/// Interface contexts code.
///****************************************************************

///==========================================
/// Context interfaces
///==========================================

namespace context
{
	///==========================================
	/// BasicTypes context
	///==========================================

    template<class Parent> inline bool BasicTypes<Parent>::Parse()
    {
        PARSE_START;
        {
            this->FindBasicTypes();
        }
        PARSE_END;
    }
		
    template<class Parent> inline void BasicTypes<Parent>::FindBasicTypes()
    {
        this->FindAngles();
			
        this->CleanAll();
			
        //NOTE: we want standalone angles anytime we can have expressions.
        this->Disallow(T_LESS_THAN);//??
        this->Disallow(T_GREATER_THAN);
			
        this->FindTemplateTypes();
			
        this->FindScopes(); // id::id
		
        this->FindSigned();
        this->FindUnsigned();

        this->FindArrays(); // id[...][...]
						
        this->FindPointers();
        this->FindReferences();

        this->FindFunctionPointers();

        this->FindPointerMembers();
    }

	///==========================================
	/// Declaration context
	///==========================================

    template<class Parent> inline opString Declaration<Parent>::GetClassName()
    {
        ABSTRACT_FUNCTION;
        return "";
    }

    template<class Parent> inline bool Declaration<Parent>::Parse()
    {
        PARSE_START;
			
        this->CleanAll();

        //needs to be before arrays
        this->FindOperators();	 // operator ... [(...)]

        this->FindAngles();

        ConcatenationWalker performconcat(this);

        this->FindCPlusPluses();
		
        this->FindTemplateDecls(); // template< ... >

        this->FindTemplateTypes();

        this->FindSigned();
        this->FindUnsigned();

        this->FindModifiers();
        this->FindValuedModifiers();
			
        this->FindScopes(); // id::id
			
        this->FindArrays(); // id[...][...]
			
        this->FindPointers();
			
        this->FindReferences();
			
        this->FindFunctionPointers();
			
        this->FindPointerMembers();			
			
        this->FindFunctions();
			
        this->FindDestructors(GetClassName());
			
        this->FindConstructors(GetClassName());
			
        this->FindDestructorDefinitions();

        this->FindConstructorDefinitions();			

        this->FindFunctionDefinitions();

        this->FindFriends();

        this->FindUsings();

        this->FindTypedefs();

        this->FindVisibilityLabels();

        this->FindCPPConstructs();

        this->FindOPEnums();
        this->FindOPObjects();

        this->FindTemplated();
			
        this->FindBasicStatements();
			
        PARSE_END;
    }

    template<class Parent> inline bool Declaration<Parent>::PostParse()
    {
        POSTPARSE_START;
        {
            this->AllowOnlyBasicStatements();
        }
        POSTPARSE_END;
    }

	///==========================================
	/// Argument context
	///==========================================

    template<class Parent> inline bool Argument<Parent>::Parse()
    {
        PARSE_START;
        {
            this->FindBasicTypes();
        }
        PARSE_END;
    }

	///==========================================
	/// State context
	///==========================================

    template<class Parent> inline bool State<Parent>::Parse()
    {
        PARSE_START;
			
        //TODO: definitely should group these things... (share between stuff...)
        this->FindAngles();
		
        this->CleanAll();
			
        this->Disallow(T_LESS_THAN);
        this->Disallow(T_GREATER_THAN);

        this->FindTemplateTypes();
						
        this->FindScopes();
		
        this->FindArrays();
				
        this->FindPointers();
        this->FindReferences();
			
        this->FindFunctionPointers();
			
        this->FindFunctions();
        this->FindFunctionDefinitions();
			
        this->FindStates();
			
        this->FindVisibilityLabels();
			
        this->FindStateStatements();
			
        PARSE_END;
    }

    template<class Parent> inline bool State<Parent>::PostParse()
    {
        POSTPARSE_START;
        {
            this->AllowOnlyStateStatements();
        }
        POSTPARSE_END;
    }

	///==========================================
	/// Inheritance context
	///==========================================

    template<class Parent> inline bool Inheritance<Parent>::Parse()
    {
        PARSE_START;

        this->FindAngles();
		
        this->CleanAll();

        this->Disallow(T_LESS_THAN);
        this->Disallow(T_GREATER_THAN);

        this->FindTemplateTypes();
        this->FindScopes();

        PARSE_END;
    }

	///==========================================
	/// NamespaceDecl context
	/// 
	/// this is like namespace A::B {}, not inside the braces
	///==========================================

    template<class Parent> inline bool NamespaceDecl<Parent>::Parse()
    {
        PARSE_START;

        this->CleanAll();

        this->FindScopes();

        PARSE_END;
    }

	///==========================================
	/// Global
	///==========================================

    template<class Parent> inline bool Global<Parent>::PreParse()
    {
        PREPARSE_START;
        {
            this->FindOPIncludes();
        }
        PREPARSE_END;
    }

    template<class Parent> inline bool Global<Parent>::Parse()
    {
        PARSE_START;
        {
            ConcatenationWalker performconcat(this);

            this->FindOPDefines();
            this->FindUsingNamespaceKeywords();
            this->FindNamespaces();
            this->FindCPlusPluses();
            this->FindOPEnums();
            this->FindOPObjects();
            this->FindConditionalPreprocessorStatements();
        }
        PARSE_END;
    }

    template<class Parent> inline bool Global<Parent>::PostParse()
    {
        POSTPARSE_START;
        {
            NameResolverWalker walker(this);
        }
        POSTPARSE_END;
    }

	///==========================================
	/// Operator context
	///==========================================

    template<class Parent> inline void Operator<Parent>::Init()
    {
        bCastOperator = false;
        OperatorType  = NULL;
    }

    template<class Parent> inline bool Operator<Parent>::Parse()
    {
        PARSE_START;

        //TODO: validate this, its probably wrong.
        this->FindScopes();

        if(!this->IsCurrent(T_STAR))
            this->FindPointers();
			
        if(!this->IsCurrent(T_AMPERSAND))
            this->FindReferences();

        if( this->IsCurrent(G_POINTER) || 
            this->IsCurrent(T_ID) || 
            this->IsCurrent(G_REFERENCE) || 
            this->IsCurrent(G_SCOPE) )
        {
            bCastOperator = true;
            OperatorType  = this->CurrentNode();
            this->IncrementPosition();
        }
        else
            OperatorType = this->CheckOverloadableOperator();

        this->CheckNone();

        PARSE_END;
    }

	///==========================================
	/// Dialect context
	///==========================================
	
    template<class Parent> inline bool Dialect<Parent>::PreParse()
    {
        PREPARSE_START;
        {
            this->FindCodeLocations();
            this->FindOPIncludes();
        }
        PREPARSE_END;
    }

    template<class Parent> inline bool Dialect<Parent>::Parse()
    {
        PARSE_START;
        {
            this->CleanAll();	
            this->FindOPDefines();
            this->FindScopes();
            this->FindDialectNamespaces();
            this->FindEnumerations();
            this->FindCategories();
            this->FindNoteDefinitions();
            this->FindFileDeclarations();
            this->FindExtensions();
            this->FindExtendPoints();

            // look for extensionpoint's (everywhere)
            {
                ExtensionPointWalker walker(this);
            }

            // parse global dialect statements
            this->FindGlobalDialectStatements();
        }
        PARSE_END;
    }

    template<class Parent> inline bool Dialect<Parent>::PostParse()
    {
        POSTPARSE_START;
        {
            this->AllowOnlyGlobalDialectStatements();
        }
        POSTPARSE_END;
    }

	///==========================================
	/// Category Context
	///==========================================

    template<class Parent> inline bool Category<Parent>::Parse()
    {
        PARSE_START;
        {
            this->CleanAll();
            this->FindDataModifiers();
            this->FindFunctionModifiers();
            this->FindCategoryLocations();
            this->FindDisallows();

            this->FindDialectCategoryStatements();
        }
        PARSE_END;
    }

    template<class Parent> inline bool Category<Parent>::PostParse()
    {
        POSTPARSE_START;
        {
            this->AllowOnlyDialectCategoryStatements();
        }
        POSTPARSE_END;
    }

	///==========================================
	/// Category Location Context
	///==========================================

    template<class Parent> inline bool CategoryLocation<Parent>::Parse()
    {
        PARSE_START;
        {
            this->CleanAll();
            this->FindNotes();
            this->FindCategoryMaps();

            // parse category location statements, then 
            // do an appropriate allow only
            this->FindCategoryLocationStatements();
        }
        PARSE_END;
    }

    template<class Parent> inline bool CategoryLocation<Parent>::PostParse()
    {
        POSTPARSE_START;
        {
            this->AllowOnlyCategoryLocationStatements();
        }
        POSTPARSE_END;
    }

	///==========================================
	/// Map Context
	///==========================================

    template<class Parent> inline bool CategoryMap<Parent>::Parse()
    {
        PARSE_START;
        {
            this->CleanAll();
            this->FindCriteriaExpressions();

            // parse category map criteria expressions, and 
            // do the correct allowonly
            this->FindCriteriaStatements();
        }
        PARSE_END;
    }

    template<class Parent> inline bool CategoryMap<Parent>::PostParse()
    {
        POSTPARSE_START;
        {
            this->AllowOnlyDialectCriteriaStatements();
        }
        POSTPARSE_END;
    }

	///==========================================
	/// Enumeration Context
	///==========================================

    template<class Parent> inline bool Enumeration<Parent>::Parse()
    {
        PARSE_START;
        {
            this->CleanAll();
            this->FindEnumerationLocations();
            this->FindDisallows();

            this->FindDialectEnumStatements();
        }
        PARSE_END;
    }

    template<class Parent> inline bool Enumeration<Parent>::PostParse()
    {
        POSTPARSE_START;
        {
            this->AllowOnlyDialectEnumStatements();
        }
        POSTPARSE_END;
    }

	///==========================================
	/// Enumeration Location Context
	///==========================================

    template<class Parent> inline bool EnumerationLocation<Parent>::Parse()
    {
        PARSE_START;
        {
            this->CleanAll();
            this->FindNotes();
            this->FindEnumerationMaps();

            // parse statements
            this->FindEnumerationLocationStatements();
        }
        PARSE_END;
    }

    template<class Parent> inline bool EnumerationLocation<Parent>::PostParse()
    {
        POSTPARSE_START;
        {
            this->AllowOnlyEnumerationLocationStatements();
        }
        POSTPARSE_END;
    }

	//==========================================
	// FileDeclaration
	//==========================================

    template<class Parent> inline bool FileDeclaration<Parent>::Parse()
    {
        PARSE_START;
        {
            this->CleanAll();
            this->FindFileDeclarationLocations();

            // parse into statements
            this->FindFileDeclarationLocationStatements();
        }
        PARSE_END;
    }

    template<class Parent> inline bool FileDeclaration<Parent>::PostParse()
    {
        POSTPARSE_START;
        {
            this->AllowOnlyFileDeclarationLocationStatements();
        }
        POSTPARSE_END;
    }

	//==========================================
	// TemplateType
	//==========================================

    template<class Parent> inline bool TemplateType<Parent>::Parse()
    {
        PARSE_START;
        {
            this->CleanAll();
				
            this->FindTemplateTypes();
            this->FindScopes();
        }
        PARSE_END;
    }

} // end namespace context

