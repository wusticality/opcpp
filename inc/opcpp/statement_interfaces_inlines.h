///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: StatementInterfaces.h
/// Date: 12/19/2006
///
/// Description: Statement related interface definitions
///
///****************************************************************

namespace interfaces
{

///
/// BasicStatementsBase
///

	///==========================================
	/// UnknownStatement
	///==========================================

	//is current an unknown statement? transform stuff
	inline bool BasicStatementsBase::UnknownStatement(stacked<opNode>& stuff)
	{
		if(this->IsCurrent(T_SEMICOLON))
		{
			stacked<StatementNode> statement = opNode::Transform<StatementNode>(stuff);
			
			if(statement->IsEmpty())
				statement->CopyBasics(CurrentNode());
			
			this->Erase(T_SEMICOLON);
			
			UnknownStatements.push_back(*statement);
			this->InsertNodeAtCurrent(statement);
			return true;
		}
		return false;
	}

	///==========================================
	/// OPEnumStatement
	///==========================================

	//is current an openum statement? transform stuff
	inline bool BasicStatementsBase::OPEnumStatement(stacked<opNode>& stuff)
	{
		if(this->IsCurrent(G_OPENUM))
		{
			stackedcontext<OPEnumStatementNode> statement = opNode::Make<OPEnumStatementNode>(G_OPENUM);

			stacked<OPEnumNode> openum = opNode::Expect<OPEnumNode>(G_OPENUM);

			if(!stuff->IsEmpty())
			{
				stacked<ModifiersNode> modifiers = opNode::Transform<ModifiersNode>(stuff);
				statement->SetModifiers(*modifiers);
				statement->AppendNode(modifiers);
			}
			else
				stuff.Delete();

			statement->SetEnum(*openum);
			statement->AppendNode(openum);

			OPEnumStatements.push_back(*statement);
			this->InsertNodeAtCurrent(statement);
			return true;
		}
		return false;
	}

	///==========================================
	/// OPObjectStatement
	///==========================================

	//is current an opclass statement? transform stuff
	inline bool BasicStatementsBase::OPObjectStatement(stacked<opNode>& stuff)
	{
		if(this->IsCurrent(G_OPOBJECT))
		{
			stackedcontext<OPObjectStatementNode> statement = opNode::Make<OPObjectStatementNode>(G_OPOBJECT);

			stacked<OPObjectNode> object = opNode::Expect<OPObjectNode>(G_OPOBJECT);

			if(!stuff->IsEmpty())
			{
				stacked<ModifiersNode> modifiers = opNode::Transform<ModifiersNode>(stuff);
				statement->SetModifiers(*modifiers);
				statement->AppendNode(modifiers);
			}
			else
				stuff.Delete();

			statement->SetObject(*object);
			statement->AppendNode(object);

			OPObjectStatements.push_back(*statement);
			this->InsertNodeAtCurrent(statement);
			return true;
		}
		return false;
	}

	///==========================================
	/// StateStatement
	///==========================================

	//is current an opstate statement? transform stuff
	inline bool BasicStatementsBase::StateStatement(stacked<opNode>& stuff)
	{
		if(this->IsCurrent(G_STATE))
		{
			stackedcontext<StateStatementNode> statement = opNode::Make<StateStatementNode>(G_STATE);

			stacked<StateNode> state = opNode::Expect<StateNode>(G_STATE);

			if(!stuff->IsEmpty())
			{
				stacked<ModifiersNode> modifiers = opNode::Transform<ModifiersNode>(stuff);
				statement->SetModifiers(*modifiers);
				statement->AppendNode(modifiers);
			}
			else
				stuff.Delete();					

			statement->SetState(*state);
			statement->AppendNode(state);

			StateStatements.push_back(*statement);
			this->InsertNodeAtCurrent(statement);
			return true;
		}
		return false;
	}

	///==========================================
	/// VisibilityStatement
	///==========================================

	//is current a visibility statement? transform stuff
	inline bool BasicStatementsBase::VisibilityStatement(stacked<opNode>& stuff)
	{
		if(this->IsCurrent(G_VISIBILITY_LABEL))
		{
			//ok, we need to turn this into a visibility statement
			//we actually need to error if we had something in this... it should be caught by AllowOnly!
			//looks...ok (would rather collapse stuff here)

			stackedcontext<VisibilityStatementNode> visstatement = opNode::Make<VisibilityStatementNode>(G_VISIBILITY_LABEL);

			stacked<VisibilityLabelNode> vislabel = opNode::Expect<VisibilityLabelNode>(G_VISIBILITY_LABEL);

			visstatement->SetLabel(*vislabel);
			visstatement->AppendNode(vislabel);

			CollapseNodeAtCurrent(stuff);

			this->InsertNodeAtCurrent(visstatement);
			return true;
		}
		return false;
	}

	//we don't gather a list of visibility statements since they're order dependent

	///==========================================
	/// FunctionDefinitionStatement
	///==========================================

	//is current a function definition? transform stuff
	inline bool BasicStatementsBase::FunctionDefinitionStatement(stacked<opNode>& stuff)
	{
		if(this->IsCurrent(G_FUNCTION_DEFINITION))
		{
			//ok, we need to turn this into a function statement
			//how?
			stackedcontext<FunctionDefinitionStatementNode> statement = opNode::Make<FunctionDefinitionStatementNode>(G_FUNCTION_DEFINITION);
			
			stacked<FunctionDefinitionNode> functiondefinition = opNode::Expect<FunctionDefinitionNode>(G_FUNCTION_DEFINITION);
			
			if(!stuff->IsEmpty())
			{
				stacked<ModifiersNode> modifiers = opNode::Transform<ModifiersNode>(stuff);
				statement->SetModifiers(*modifiers);
				statement->AppendNode(modifiers);
			}
			else
				stuff.Delete();
			
			//need to do something w/ this
			statement->SetFunctionDefinition(*functiondefinition);
			statement->AppendNode(functiondefinition);
			
			FunctionDefinitionStatements.push_back(*statement);
			this->InsertNodeAtCurrent(statement);
			return true;
		}
		return false;
	}

	///==========================================
	/// ConstructorStatement
	///==========================================
	
	inline bool BasicStatementsBase::ConstructorStatement(stacked<opNode>& stuff)
	{
		if(this->IsCurrent(G_CONSTRUCTOR_DEFINITION))
		{
			//ok, we need to turn this into a function statement
			//how?
			stackedcontext<ConstructorDefinitionStatementNode> 
			statement = opNode::Make<ConstructorDefinitionStatementNode>(G_CONSTRUCTOR_DEFINITION);
			
			stacked<ConstructorDefinitionNode>
			constructordefinition = opNode::Expect<ConstructorDefinitionNode>(G_CONSTRUCTOR_DEFINITION);
			
			if(!stuff->IsEmpty())
			{
				stacked<ModifiersNode> modifiers = opNode::Transform<ModifiersNode>(stuff);
				statement->SetModifiers(*modifiers);
				statement->AppendNode(modifiers);
			}
			else
				stuff.Delete();
			
			//need to do something w/ this
			statement->SetConstructorDefinition(*constructordefinition);
			statement->AppendNode(constructordefinition);
			
			ConstructorDefinitionStatements.push_back(*statement);
			this->InsertNodeAtCurrent(statement);
			return true;
		}
		return false;
	}
	
	///==========================================
	/// DestructorStatement
	///==========================================
	
	inline bool BasicStatementsBase::DestructorStatement(stacked<opNode>& stuff)
	{
		if(this->IsCurrent(G_DESTRUCTOR_DEFINITION))
		{
			//ok, we need to turn this into a function statement
			//how?
			stackedcontext<DestructorDefinitionStatementNode> 
				statement = opNode::Make<DestructorDefinitionStatementNode>(G_DESTRUCTOR_DEFINITION);

			stacked<DestructorDefinitionNode>
				destructordefinition = opNode::Expect<DestructorDefinitionNode>(G_DESTRUCTOR_DEFINITION);

			if(!stuff->IsEmpty())
			{
				stacked<ModifiersNode> modifiers = opNode::Transform<ModifiersNode>(stuff);
				statement->SetModifiers(*modifiers);
				statement->AppendNode(modifiers);
			}
			else
				stuff.Delete();
			
			//need to do something w/ this
			statement->SetDestructorDefinition(*destructordefinition);
			statement->AppendNode(destructordefinition);
			
			DestructorDefinitionStatements.push_back(*statement);
			this->InsertNodeAtCurrent(statement);
			return true;
		}
		return false;
	}
	
	///==========================================
	/// PreprocessorStatement
	///==========================================
	
	inline bool BasicStatementsBase::ConditionalPreprocessorStatement(stacked<opNode>& stuff)
	{
		if (this->IsCurrent(G_POUND_ELIF)
		||  this->IsCurrent(G_POUND_ELSE)
		||  this->IsCurrent(G_POUND_ENDIF)
		||  this->IsCurrent(G_POUND_IF)
		||  this->IsCurrent(G_POUND_IFDEF)
		||  this->IsCurrent(G_POUND_IFNDEF))
		{
			Token                                     id           = CurrentNode()->GetId();
			stackedcontext<PreprocessorStatementNode> statement    = opNode::Make<PreprocessorStatementNode>(id);
			stacked<PreprocessorNode>                 preprocessor = opNode::Expect<PreprocessorNode>(id);

			statement->SetPreprocessor(*preprocessor);
			statement->AppendNode(preprocessor);

			if (!stuff->IsEmpty())
				statement->CollapseNode(stuff, statement->GetBegin());
			else
				stuff.Delete();

			this->InsertNodeAtCurrent(statement);

			return true;
		}

		return false;
	}
	
	///==========================================
	/// CPPConstructStatement
	///==========================================
	
	inline bool BasicStatementsBase::CPPConstructStatement(stacked<opNode>& stuff)
	{
		if (this->IsCurrent(G_CLASS)
		||  this->IsCurrent(G_STRUCT)
		||  this->IsCurrent(G_ENUM)
		||  this->IsCurrent(G_UNION))
		{
			Token                                     id        = CurrentNode()->GetId();
			stackedcontext<CPPConstructStatementNode> statement = opNode::Make<CPPConstructStatementNode>(id);
			stacked<CPPConstructNode>                 construct = opNode::Expect<CPPConstructNode>(id);

			if ( !stuff->IsEmpty() )
			{
				stacked<ModifiersNode> modifiers = opNode::Transform<ModifiersNode>( stuff );
				statement->SetModifiers( *modifiers );
				statement->AppendNode( modifiers );
			}
			else
				stuff.Delete();
			
			statement->SetConstruct(*construct);
			statement->AppendNode(construct);

			this->InsertNodeAtCurrent(statement);

			return true;
		}

		return false;
	}

	//==========================================
	// FriendStatements
	//==========================================

	inline bool BasicStatementsBase::FriendStatement(stacked<opNode>& stuff)
	{
		if ( this->IsCurrent( G_FRIEND ) )
		{
			stacked<FriendStatementNode> statement = opNode::Make<FriendStatementNode>( G_FRIEND );
			stacked<FriendNode>          thefriend = opNode::Expect<FriendNode>( G_FRIEND );

			statement->SetFriend( *thefriend );
			statement->AppendNode( thefriend );

			if (!stuff->IsEmpty())
				statement->CollapseNode(stuff, statement->GetBegin());
			else
				stuff.Delete();

			this->InsertNodeAtCurrent(statement);

			return true;
		}

		return false;
	}

	//==========================================
	// TypedefStatements
	//==========================================

	inline bool BasicStatementsBase::TypedefStatement(stacked<opNode>& stuff)
	{
		if ( this->IsCurrent( G_TYPEDEF ) )
		{
			stackedcontext<TypedefStatementNode> statement  = opNode::Make<TypedefStatementNode>( G_TYPEDEF );
			stacked<TypedefNode>                 thetypedef = opNode::Expect<TypedefNode>( G_TYPEDEF );

			if ( !stuff->IsEmpty() )
			{
				stacked<ModifiersNode> modifiers = opNode::Transform<ModifiersNode>( stuff );
				statement->SetModifiers( *modifiers );
				statement->AppendNode( modifiers );
			}
			else
				stuff.Delete();

			statement->SetTypedef( *thetypedef );
			statement->AppendNode( thetypedef );

			this->InsertNodeAtCurrent( statement );

			return true;
		}

		return false;
	}

	///==========================================
	/// TemplateStatements
	///==========================================

	inline bool BasicStatementsBase::TemplateStatement(stacked<opNode>& stuff)
	{
		if ( this->IsCurrent(G_TEMPLATED) )
		{
			stackedcontext<TemplateStatementNode> statement = opNode::Make<TemplateStatementNode>(G_TEMPLATED);
			stacked<TemplatedNode>                templated = opNode::Expect<TemplatedNode>(G_TEMPLATED);

			if ( !stuff->IsEmpty() )
			{
				stacked<ModifiersNode> modifiers = opNode::Transform<ModifiersNode>( stuff );

				statement->SetModifiers( *modifiers );
				statement->AppendNode( modifiers );
			}
			else
				stuff.Delete();

			statement->SetTemplated( *templated );
			statement->AppendNode( templated );

			this->InsertNodeAtCurrent(statement);

			return true;
		}

		return false;
	}

///
/// BasicStatements
///

	inline bool BasicStatements::Parse()
	{
		PARSE_START;
		{
			FindBasicStatements();
		}
		PARSE_END;
	}

	inline bool BasicStatements::PostParse()
	{
		POSTPARSE_START;
		{
			AllowOnlyBasicStatements();
		}
		POSTPARSE_END;
	}
	
	// Only allow the following statements in opobjects.
	inline void BasicStatements::AllowOnlyBasicStatements()
	{
		AllowOnly(G_STATEMENT,
				  G_OPENUM_STATEMENT,
				  G_OPOBJECT_STATEMENT,
				  G_FUNCTION_DEFINITION_STATEMENT,
				  G_VISIBILITY_STATEMENT,
				  G_CONSTRUCTOR_DEFINITION_STATEMENT,
				  G_DESTRUCTOR_DEFINITION_STATEMENT,
				  G_TYPEDEF_STATEMENT,
				  G_FRIEND_STATEMENT,
				  G_PREPROCESSOR_STATEMENT,
				  G_CPPCONSTRUCT_STATEMENT,
				  G_CPLUSPLUS,
				  G_TEMPLATE_STATEMENT);
	}
	
	inline stacked<opNode> BasicStatements::PushUntilStatementEnd()
	{
		return opNode::PushUntilOr<opNode>(T_SEMICOLON,
										   G_OPOBJECT,
										   G_OPENUM,
										   G_FUNCTION_DEFINITION,
										   G_VISIBILITY_LABEL,
										   G_CONSTRUCTOR_DEFINITION,
										   G_DESTRUCTOR_DEFINITION,
										   G_TEMPLATED,
										   G_TYPEDEF,
										   G_FRIEND,
										   G_CLASS,
										   G_STRUCT,
										   G_ENUM,
										   G_UNION,
										   G_POUND_ELIF,
										   G_POUND_ELSE,
										   G_POUND_ENDIF,
										   G_POUND_IF,
										   G_POUND_IFDEF,
										   G_POUND_IFNDEF,
										   false);
	}
	
	// Find all statements except preprocessor ones.
	inline void BasicStatements::FindBasicStatements()
	{
		bool bFinished = false;

		while(!bFinished)
		{
			//if ( TemplateStatement() )
			//	continue;

			stacked<opNode> stuff = PushUntilStatementEnd();
			
			bFinished = !(*stuff);
			
			if (bFinished)
				break;
			
			if		(FunctionDefinitionStatement(stuff));
			else if	(VisibilityStatement(stuff));
			else if (OPObjectStatement(stuff));
			else if	(OPEnumStatement(stuff));
			else if (ConstructorStatement(stuff));
			else if (DestructorStatement(stuff));
			else if (TemplateStatement(stuff));
			else if (TypedefStatement(stuff));
			else if (FriendStatement(stuff));
			else if (CPPConstructStatement(stuff));
			else if (ConditionalPreprocessorStatement(stuff));
			else if	(UnknownStatement(stuff));
			else
			{
				CollapseNodeAtCurrent(stuff);
				
				//I think this function should never error.
				bFinished = true;
			}
		}
	}

///
/// BasicStateStatements
///

	//TODO: split up the utility function interface
	//		so we don't have all these extra gathering vectors.

	inline void BasicStateStatements::FindStateStatements()
	{
		//for now we only look for function definitions
		//later we can add a new statement type (G_STATEMENT_FUNCTIONSONLY)
		bool bfinished = false;
		while(!bfinished)
		{
			stacked<opNode> stuff = opNode::PushUntilOr<opNode>(G_FUNCTION_DEFINITION,
														G_STATE,
														T_SEMICOLON,
														false);
			
			bfinished = !(*stuff);
			if(bfinished)
				break;
			
			//handle null statements...
			if(this->IsCurrent(T_SEMICOLON))
			{
				this->Erase(T_SEMICOLON);

				CollapseNodeAtCurrent(stuff);
			}
			else if	(FunctionDefinitionStatement(stuff));
			else if (StateStatement(stuff));
			else
			{
				CollapseNodeAtCurrent(stuff);

				//I think this function should never error.
				bfinished = true;
			}
		}
	}

	inline void BasicStateStatements::AllowOnlyStateStatements()
	{
		AllowOnly(G_FUNCTION_DEFINITION_STATEMENT,
				  G_STATE_STATEMENT);
	}

///==========================================
/// UsingStatements
///==========================================

	inline bool UsingStatements::Parse()
	{
		PARSE_START;
		{
			FindUsingStatements();
		}
		PARSE_END;
	}

	inline bool UsingStatements::FindUsingStatements()
	{
		INSPECT_START(G_USING_STATEMENT);
		{
			if (PeekStart(G_USING))
			{
				stacked<UsingStatementNode> newNode = opNode::PushUntilEnd<UsingStatementNode>();

				SetInnerStatement(*newNode);
				AppendNode(newNode);
			}
		}
		INSPECT_END;
	}

///
/// NullStatements
///

	inline bool NullStatements::Parse()
	{
		PARSE_START;
		{
			FindNullStatements();
		}
		PARSE_END;
	}

	inline bool NullStatements::FindNullStatements()
	{
		INSPECT_START(G_NULL_STATEMENT);
		{
			if (IsEmpty())
			{
				stacked<NullStatementNode> newNode = NEWNODE(NullStatementNode());

				newNode->CopyBasics(this);

				SetInnerStatement(*newNode);
				AppendNode(newNode);

				return true;
			}
		}
		INSPECT_END;
	}

///
/// FuncPointerStatements
///

	inline bool FuncPointerStatements::Parse()
	{
		PARSE_START;
		{
			FindFuncPointerStatements();
		}
		PARSE_END;
	}

	inline bool FuncPointerStatements::FindFuncPointerStatements()
	{
		INSPECT_START(G_FUNCTION_POINTER_STATEMENT);
		{
			if (PeekEnd(G_FUNCTION_POINTER))
			{
				stackedcontext<FuncPointerStatementNode> newNode = NEWNODE(FuncPointerStatementNode());
				newNode->CopyBasics(this);
				
				stacked<ModifiersNode> modifiers = opNode::PushUntil<ModifiersNode>(G_FUNCTION_POINTER);
				stacked<FunctionPointerNode> fpn = opNode::Expect<FunctionPointerNode>(G_FUNCTION_POINTER);
				
				if (!modifiers->IsEmpty())
				{
					modifiers->CopyBasics(*fpn);
					newNode->SetModifiers(*modifiers);
					newNode->AppendNode(modifiers);
				}
				else
					modifiers.Delete();
				
				newNode->SetFunctionPointer(*fpn);
				newNode->AppendNode(fpn);
				
				SetInnerStatement(*newNode);
				AppendNode(newNode);
				
				return true;
			}
		}
		INSPECT_END;
	}

///
/// FuncPrototypeStatements
///

	inline bool FuncPrototypeStatements::Parse()
	{
		PARSE_START;
		{
			FindFuncPrototypeStatements();
		}
		PARSE_END;
	}

	inline bool FuncPrototypeStatements::FindFuncPrototypeStatements()
	{
		INSPECT_START(G_FUNCTION_PROTOTYPE_STATEMENT);
		{
			if (PeekEnd(G_FUNCTION_PROTOTYPE))
			{
				stackedcontext<FuncPrototypeStatementNode> newNode = NEWNODE(FuncPrototypeStatementNode());
				newNode->CopyBasics(this);

				stacked<ModifiersNode> modifiers = opNode::PushUntil<ModifiersNode>(G_FUNCTION_PROTOTYPE);
				stacked<FunctionPrototypeNode> fpn = opNode::Expect<FunctionPrototypeNode>(G_FUNCTION_PROTOTYPE);

				if (!modifiers->IsEmpty())
				{
					modifiers->CopyBasics(*fpn);
					newNode->SetModifiers(*modifiers);
					newNode->AppendNode(modifiers);
				}
				else
					modifiers.Delete();

				newNode->SetFunctionPrototype(*fpn);
				newNode->AppendNode(fpn);

				SetInnerStatement(*newNode);
				AppendNode(newNode);

				return true;
			}
		}
		INSPECT_END;
	}

///
/// ConstructorPrototypeStatements
///

	inline bool ConstructorPrototypeStatements::FindConstructorPrototypeStatements()
	{
		INSPECT_START(G_CONSTRUCTOR_PROTOTYPE_STATEMENT);
		{
			if (PeekEnd(G_CONSTRUCTOR_PROTOTYPE))
			{
				stackedcontext<ConstructorPrototypeStatementNode> newNode = NEWNODE(ConstructorPrototypeStatementNode());
				
				newNode->CopyBasics(this);
				
				stacked<ModifiersNode> modifiers = opNode::PushUntil<ModifiersNode>(G_CONSTRUCTOR_PROTOTYPE);
				stacked<ConstructorPrototypeNode> cpn = opNode::Expect<ConstructorPrototypeNode>(G_CONSTRUCTOR_PROTOTYPE);
				
				if (!modifiers->IsEmpty())
				{
					modifiers->CopyBasics(*cpn);
					newNode->SetModifiers(*modifiers);
					newNode->AppendNode(modifiers);
				}
				else
					modifiers.Delete();
				
				newNode->SetConstructorPrototype(*cpn);
				newNode->AppendNode(cpn);
				
				SetInnerStatement(*newNode);
				AppendNode(newNode);
				
				return true;
			}
		}
		INSPECT_END;
	}

///
/// DestructorPrototypeStatements
///

    inline bool DestructorPrototypeStatements::FindDestructorPrototypeStatements()
	{
		INSPECT_START(G_DESTRUCTOR_PROTOTYPE_STATEMENT);
		{
			if (PeekEnd(G_DESTRUCTOR_PROTOTYPE))
			{
				stackedcontext<DestructorPrototypeStatementNode> newNode = NEWNODE(DestructorPrototypeStatementNode());

				newNode->CopyBasics(this);

				stacked<ModifiersNode> modifiers = opNode::PushUntil<ModifiersNode>(G_DESTRUCTOR_PROTOTYPE);
				stacked<DestructorPrototypeNode> cpn = opNode::Expect<DestructorPrototypeNode>(G_DESTRUCTOR_PROTOTYPE);

				if (!modifiers->IsEmpty())
				{
					modifiers->CopyBasics(*cpn);
					newNode->SetModifiers(*modifiers);
					newNode->AppendNode(modifiers);
				}
				else
					modifiers.Delete();

				newNode->SetDestructorPrototype(*cpn);
				newNode->AppendNode(cpn);

				SetInnerStatement(*newNode);
				AppendNode(newNode);

				return true;
			}
		}
		INSPECT_END;
	}

///
/// DataStatements
///

	inline bool DataStatements::Parse()
	{
		PARSE_START;
		{
			FindDataStatements();
		}
		PARSE_END;
	}

	inline bool DataStatements::FindDataStatements()
	{
		INSPECT_START(G_DATA_STATEMENT);
		{
			FindDataStatementOnly();
		}
		INSPECT_END;
	}

    inline bool DataStatements::FindDataStatementOnly()
    {
    	if (!HasNumChildren(2))
    		return false;

    	stackedcontext<DataStatementNode> newNode = NEWNODE(DataStatementNode());
    	newNode->CopyBasics(FirstChild());
    	
    	stacked<DataDeclarationNode> declaration = opNode::PushUntil<DataDeclarationNode>(T_ASSIGN,false);

    	newNode->SetDeclaration(*declaration);
    	newNode->AppendNode(declaration);

    	if(IsCurrent(T_ASSIGN))
    	{
    		Erase(T_ASSIGN);
    		stacked<DataInitializationNode> initialization = opNode::PushUntilEnd<DataInitializationNode>();
    		
    		newNode->SetInitialization(*initialization);
    		newNode->AppendNode(initialization);
    	}
    	
    	SetInnerStatement(*newNode);
    	AppendNode(newNode);
    	
    	return true;
    }    

	inline bool DataStatementOnly::Parse()
	{
		PARSE_START;
		{
			if(!FindDataStatementOnly())
			{
				opError::MessageError(this,"Unnamed data declarations not allowed.");
				//GenericError("unnamed data declarations not allowed");
			}
		}
		PARSE_END;
	}

///==========================================
/// ConditionalPreprocessorStatements
///==========================================

	inline bool ConditionalPreprocessorStatements::Parse()
	{
		PARSE_START;
		{
			FindConditionalPreprocessorStatements();
		}
		PARSE_END;
	}

	inline void ConditionalPreprocessorStatements::FindConditionalPreprocessorStatements()
	{
		FindConditionalPreprocessorStatement<G_POUND_ELIF>();
		FindConditionalPreprocessorStatement<G_POUND_ELSE>();
		FindConditionalPreprocessorStatement<G_POUND_ENDIF>();
		FindConditionalPreprocessorStatement<G_POUND_IF>();
		FindConditionalPreprocessorStatement<G_POUND_IFDEF>();
		FindConditionalPreprocessorStatement<G_POUND_IFNDEF>();
	}

	template<Token hittoken>
	inline void ConditionalPreprocessorStatements::FindConditionalPreprocessorStatement()
	{
		LOOP_START(G_PREPROCESSOR_STATEMENT);
		{
			HIT(hittoken)
			{
				stackedcontext<PreprocessorStatementNode> newNode = opNode::Make<PreprocessorStatementNode>(hittoken);

				stacked<PreprocessorNode> preprocessor = opNode::Expect<PreprocessorNode>(hittoken);

				newNode->SetPreprocessor(*preprocessor);
				newNode->AppendNode(preprocessor);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

} // end namespace interfaces

