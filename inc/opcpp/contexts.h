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
	// Context Interfaces are used by 
	// many nodes in order to unify parsing
	// across certain contexts

	///==========================================
	/// Declaration context
	///==========================================

	//NOTES: - order is often important in a context
	// Declaration Context is for the inner blocks of opclasses and opstructs (for now)
	// Inheritance Context is for the inheritance definition of opclasses (for now)
	
	template<class Parent>
	class BasicTypes : public	FunctionPointers<
								PointerMembers<
								Scopes< 
								Arrays< 
								FundamentalTypes<
								Pointers< 
								References< 
								TemplateTypes< 
								Blocks< 
								Clean< 
								Parent
								> > > > > > > > > >
	{
	public:
		IMPLEMENTS_INTERFACE(BasicTypes)
		
		bool Parse()
		{
			PARSE_START;
			{
				this->FindBasicTypes();
			}
			PARSE_END;
		}
		
		void FindBasicTypes()
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
	};

	template<class Parent>
	class Declaration : public CPPConstructs<
							   CPlusPluses<
							   Templated<
							   Usings<
		                       Typedefs<
							   TemplateDecls<
		                       Friends<
							   FundamentalTypes<
							   BasicStatements< 
							   OPObjects< 
							   OPEnums< 
							   Operators< 
							   VisibilityLabels< 
							   FunctionDefinitions< 
							   Functions<
							   Constructors<
							   Destructors<
							   ConstructorDefinitions<
							   DestructorDefinitions<
							   Modifiers<
							   ValuedModifiers< 
							   context::BasicTypes<
							   Parent
							   > > > > > > > > > > > > > > > > > > > > > >
	{
	public:
		IMPLEMENTS_INTERFACE(Declaration)

		virtual opString GetClassName()
		{
			ABSTRACT_FUNCTION;
			return "";
		}

		bool Parse()
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

		bool PostParse()
		{
			POSTPARSE_START;
			{
				this->AllowOnlyBasicStatements();
			}
			POSTPARSE_END;
		}
	};

	///==========================================
	/// Argument context
	///==========================================

	template<class Parent>
	class Argument : public context::BasicTypes<Parent>
	{
	public:
		IMPLEMENTS_INTERFACE(Argument)

		bool Parse()
		{
			PARSE_START;
			{
				this->FindBasicTypes();
			}
			PARSE_END;
		}
	};

	///==========================================
	/// State context
	///==========================================

	template<class Parent>
	class State : public	BasicStateStatements< 
							States< 
							FunctionPointers< 
							Scopes< 
							Arrays< 
							VisibilityLabels< 
							FunctionDefinitions<
							Pointers< 
							References< 
							Functions< 
							TemplateTypes< 
							Blocks< 
							Clean<
							Parent
							> > > > > > > > > > > > > 
	{
	public:
		IMPLEMENTS_INTERFACE(State)

		bool Parse()
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

		bool PostParse()
		{
			POSTPARSE_START;
			{
				this->AllowOnlyStateStatements();
			}
			POSTPARSE_END;
		}
	};

	///==========================================
	/// Inheritance context
	///==========================================

	template<class Parent>
	class Inheritance : public Scopes< 
							   TemplateTypes< 
							   Blocks< 
							   Clean< 
							   Parent 
							   > > > > 
	{
	public:
		IMPLEMENTS_INTERFACE(Inheritance)

		bool Parse()
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
	};

	///==========================================
	/// NamespaceDecl context
	/// 
	/// this is like namespace A::B {}, not inside the braces
	///==========================================

	template<class Parent>
	class NamespaceDecl : public Scopes< 
								 Clean< 
		                         Parent 
								 > > 
	{
	public:
		IMPLEMENTS_INTERFACE(NamespaceDecl)

		bool Parse()
		{
			PARSE_START;

			this->CleanAll();

			this->FindScopes();

			PARSE_END;
		}
	};

	///==========================================
	/// Global
	///==========================================

	template<class Parent>
	class Global : public ConditionalPreprocessorStatements<
						  CPlusPluses<
						  OPObjects<
						  OPEnums<
						  OPDefines<
						  Namespaces<
						  UsingNamespaceKeywords<
						  OPIncludes<
						  Parent
						  > > > > > > > > 
	{
	public:
		IMPLEMENTS_INTERFACE(Global)

		bool PreParse()
		{
			PREPARSE_START;
			{
				this->FindOPIncludes();
			}
			PREPARSE_END;
		}

		bool Parse()
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

		bool PostParse()
		{
			POSTPARSE_START;
			{
				NameResolverWalker walker(this);
			}
			POSTPARSE_END;
		}
	};

	///==========================================
	/// Operator context
	///==========================================

	template<class Parent>
	class Operator : public Scopes< 
							Pointers< 
							References< 
							Clean< 
		                    Parent 
						    > > > > 
	{
	public:
		IMPLEMENTS_INTERFACE(Operator)

		void Init()
		{
        	bCastOperator = false;
			OperatorType  = NULL;
		}

		bool Parse()
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
				OperatorType = CheckOverloadableOperator();

			this->CheckNone();

			PARSE_END;
		}

	protected:
		opNode* OperatorType;
		bool    bCastOperator;
	};

	///==========================================
	/// Dialect context
	///==========================================
	
	template<class Parent>
	class Dialect : public GlobalDialectStatements<
						   DialectNamespaces<
						   OPMacros<
						   ExpandCalls<
						   OPDefines<
						   OPIncludes<
						   Enumerations<
						   Categories<
						   CodeLocations<
						   Scopes<
						   NoteDefinitions<						   
						   FileDeclarations<
						   Extensions<
						   ExtendPoints<
						   Clean<
						   Parent
						   > > > > > > > > > > > > > > > 
	{
	public:
		IMPLEMENTS_INTERFACE(Dialect)

		bool PreParse()
		{
			PREPARSE_START;
			{
				this->FindCodeLocations();
				this->FindOPIncludes();
			}
			PREPARSE_END;
		}

		bool Parse()
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

		bool PostParse()
		{
			POSTPARSE_START;
			{
				this->AllowOnlyGlobalDialectStatements();
			}
			POSTPARSE_END;
		}
	};

	///==========================================
	/// Category Context
	///==========================================

	template<class Parent>
	class Category : public DialectCategoryStatements<
							CategoryLocations<
							Disallows<
							FunctionModifiers<
							DataModifiers<
							Clean<
							Parent
							> > > > > >
	{
	public:
		IMPLEMENTS_INTERFACE(Category);
		
		bool Parse()
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

		bool PostParse()
		{
			POSTPARSE_START;
			{
				this->AllowOnlyDialectCategoryStatements();
			}
			POSTPARSE_END;
		}
	};

	///==========================================
	/// Category Location Context
	///==========================================

	template<class Parent>
	class CategoryLocation : public CategoryLocationStatements<
									Notes<
							        CategoryMaps<
							        Clean<
							        Parent
							        > > > >
	{
	public:
		IMPLEMENTS_INTERFACE(CategoryLocation);
		
		bool Parse()
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

		bool PostParse()
		{
			POSTPARSE_START;
			{
				this->AllowOnlyCategoryLocationStatements();
			}
			POSTPARSE_END;
		}
	};

	///==========================================
	/// Map Context
	///==========================================

	template<class Parent>
	class CategoryMap : public DialectCriteriaStatements<
							   CriteriaExpressions<
					           Clean<
					           Parent
					           > > >
	{
	public:
		IMPLEMENTS_INTERFACE(CategoryMap);

		bool Parse()
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

		bool PostParse()
		{
			POSTPARSE_START;
			{
				this->AllowOnlyDialectCriteriaStatements();
			}
			POSTPARSE_END;
		}
	};

	///==========================================
	/// Enumeration Context
	///==========================================

	template<class Parent>
	class Enumeration : public DialectEnumerationStatements<
							   EnumerationLocations<
							   Disallows<
							   Clean<
							   Parent
							   > > > >
	{
	public:
		IMPLEMENTS_INTERFACE(Enumeration);
		
		bool Parse()
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

		bool PostParse()
		{
			POSTPARSE_START;
			{
				this->AllowOnlyDialectEnumStatements();
			}
			POSTPARSE_END;
		}
	};

	///==========================================
	/// Enumeration Location Context
	///==========================================

	template<class Parent>
	class EnumerationLocation : public EnumerationLocationStatements<
									   EnumerationMaps<
		                               Notes<
									   Clean<
									   Parent
									   > > > >
	{
	public:
		IMPLEMENTS_INTERFACE(EnumerationLocation);

		bool Parse()
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

		bool PostParse()
		{
			POSTPARSE_START;
			{
				this->AllowOnlyEnumerationLocationStatements();
			}
			POSTPARSE_END;
		}
	};

	//==========================================
	// FileDeclaration
	//==========================================

	template<class Parent>
	class FileDeclaration : public FileDeclarationLocationStatements<
								   FileDeclarationLocations<
		                           Clean<
								   Parent
								   > > >
	{
	public:
		IMPLEMENTS_INTERFACE(FileDeclaration);

		bool Parse()
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

		bool PostParse()
		{
			POSTPARSE_START;
			{
				this->AllowOnlyFileDeclarationLocationStatements();
			}
			POSTPARSE_END;
		}
	};

	//==========================================
	// TemplateType
	//==========================================

	template<class Parent>
	class TemplateType : public TemplateTypes<
		                        Scopes<
		                        Clean<
		                        Parent
	                            > > > 
	{
	public:
		IMPLEMENTS_INTERFACE(TemplateType)

		bool Parse()
		{
			PARSE_START;
			{
				this->CleanAll();
				
				this->FindTemplateTypes();
				this->FindScopes();
			}
			PARSE_END;
		}
	};

} // end namespace context

