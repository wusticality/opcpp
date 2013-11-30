///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Interfaces.h
/// Date: 11/26/2006
///
/// Description:
///
/// This file contains C++ interfaces implementing various node class
/// functionality.  For example, inheriting from the Braces interface 
/// allows the class to automatically know how to parse for braces.  This 
/// increases code reusability.
///****************************************************************

namespace interfaces
{

///==========================================
/// Namespace interface
///==========================================

	template<class Parent> inline bool Namespaces<Parent>::Parse()
	{
		PARSE_START;
		{
			FindNamespaces();
		}
		PARSE_END;
	}

	template<class Parent> inline void Namespaces<Parent>::FindNamespaces()
	{
		LOOP_START(G_NAMESPACE);
		{
			HIT(T_NAMESPACE)
			{
				stackedcontext<NamespaceNode> newNode = opNode::Make<NamespaceNode>(T_NAMESPACE);
				
				this->Erase(T_NAMESPACE);
				
				this->EatWhitespaceAndComments();
				
				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);
				
				this->EatWhitespaceAndComments();
				
				if(this->IsCurrent(T_ASSIGN))
				{
					//create an alias instead (transform and set...)
					stackedcontext<NamespaceAliasNode> alias = opNode::Transform<NamespaceAliasNode>(newNode);
					
					alias->SetName(*name);
					alias->AppendNode(name);

					this->Erase(T_ASSIGN);
					
					this->EatWhitespaceAndComments();

					while(this->IsCurrent(T_ID))
					{
						stacked<TerminalNode> scope = opNode::Expect<TerminalNode>(T_ID);
						
						alias->AddScope(*scope);
						alias->AppendNode(scope);
						
						if(this->IsCurrent(T_SCOPE_RESOLUTION))
							this->Erase(T_SCOPE_RESOLUTION);
					}
					
					this->EatWhitespaceAndComments();
					
					this->Erase(T_SEMICOLON);

					this->InsertNodeAtCurrent(alias);
				}
				else
				{
					newNode->SetName(*name);
					newNode->AppendNode(name);

					this->EatWhitespaceAndComments();

					stacked<BraceBlockNode>     bbn = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
					stacked<NamespaceBlockNode> nbn = opNode::Transform<NamespaceBlockNode>(bbn);
					newNode->SetBody(*nbn);
					newNode->AppendNode(nbn);

					this->InsertNodeAtCurrent(newNode);
				}
			}
		}
		LOOP_END;
	}

//==========================================
// UsingNamespaces
//==========================================

	template<class Parent> inline bool UsingNamespaceKeywords<Parent>::Parse()
	{
		PARSE_START;
		{
			FindUsingNamespaceKeywords();
		}
		PARSE_END;
	}

	template<class Parent> inline void UsingNamespaceKeywords<Parent>::FindUsingNamespaceKeywords()
	{
		LOOP_START(G_USING_NAMESPACE_KEYWORD)
		{
			HIT(T_USING)
			{
				if ( this->PeekUncleaned(T_NAMESPACE) )
				{
					stackedcontext<UsingNamespaceKeywordNode> newNode = opNode::Make<UsingNamespaceKeywordNode>(T_USING);

					this->Erase(T_USING);

					this->EatWhitespaceAndComments();

					this->Erase(T_NAMESPACE);

					this->InsertNodeAtCurrent(newNode);					
				}
				else
					this->IncrementPosition();
			}
		}
		LOOP_END
	}

///==========================================
/// Usings
///==========================================

	template<class Parent> inline bool Usings<Parent>::Parse()
	{
		PARSE_START;
		{
			FindUsings();
		}
		PARSE_END;
	}

	template<class Parent> inline void Usings<Parent>::FindUsings()
	{
		LOOP_START(G_USING)
		{
			HIT(T_USING)
			{
				stackedcontext<UsingNode> newNode = opNode::Make<UsingNode>(T_USING);

				this->Erase(T_USING);

				stacked<ScopeNode> scope = opNode::Expect<ScopeNode>(G_SCOPE);

				newNode->SetScope(*scope);
				newNode->AppendNode(scope);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END
	}

///==========================================
/// Blocks interface
///==========================================

	//TODO:
	//FIXME: this is unsafe
	// template<Token Grammar, Token Left, Token Right, class T>
	// template<class Parent> inline void Blocks<Parent>::FindMatching()
    template<class Parent>
    template<Token Grammar, Token Left, Token Right, class T>
    inline void Blocks< Parent >::FindMatching()
	{
		//change the throw mode
		setexceptionmode mode(opException::ParseException);//TEST: I don't know that we actually want this :)

		iterator end = this->GetEnd();
		
		LOOP_START(Grammar);
		{
			HIT(Left)
			{
				// we want to create a new node
				stackedcontext<T> newNode = opNode::Make<T>(Left);            
				int        numLeft = 1;  

				// we must delete the current node (Left), our position has moved forward by one
				//Deletethis->CurrentNode();
				stacked<opNode> leftnode = opNode::PopCurrentNode();

				Token last = Left;

				while (numLeft && this->GetPosition() != end)
				{            
					// now we need to peek at the current position node
					// if the nodes type is Left
					Token currentToken = this->CurrentNode()->GetId();

					last = currentToken;

					if (currentToken == Left)
						numLeft++;
					else if (currentToken == Right)
						numLeft--;

					// if there are numLeft still, just push this node in
					if (numLeft)
					{
						stacked<opNode> tempstacked = this->PopCurrentNode();
						newNode->AppendNode(tempstacked);
					}
				}

				//this is used by angled brackets since they're context sensitive
				if(numLeft)
				{
					opError::PrematureError(Right,*leftnode,last==T_EOF);
				}

				leftnode.Delete();

				// we must delete the current node (Right)
				this->DeleteCurrentNode();
				
				// add the new node
				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;

	}

	// stuff we actually care about
	template<class Parent> inline void Blocks<Parent>::FindBraces()
	{
		FindMatching<G_BRACE_BLOCK, T_LEFT_BRACE, T_RIGHT_BRACE, BraceBlockNode>();      
	}

	template<class Parent> inline void Blocks<Parent>::FindParentheses()
	{
		FindMatching<G_PAREN_BLOCK, T_LEFT_PAREN, T_RIGHT_PAREN, ParenBlockNode>();
	}

	template<class Parent> inline void Blocks<Parent>::FindBrackets()
	{
		FindMatching<G_BRACKET_BLOCK, T_LEFT_BRACKET, T_RIGHT_BRACKET, BracketBlockNode>();
	}

	template<class Parent> inline void Blocks<Parent>::FindAngles()
	{
		FindMatching<G_ANGLED_BLOCK, T_LESS_THAN, T_GREATER_THAN, AngledBlockNode>();
	}

///==========================================
/// Pointer interface
///==========================================

	template<class Parent> inline bool Pointers<Parent>::Parse()
	{
		PARSE_START;
		{
			FindPointers();
		}
		PARSE_END;
	}

	template<class Parent> inline void Pointers<Parent>::FindPointers()
	{
		LOOP_START(G_POINTER);
		{
			HIT(T_STAR)
			{
				stackedcontext<PointerNode> newNode = opNode::Make<PointerNode>(T_STAR);
				stacked<TerminalNode>       firststar;

				//grab all the stars and c/v specifiers
				while( this->IsCurrent(T_STAR) )
				{
					// Save off the first star (for error messages).
					if ( firststar.IsValid() )
						this->Erase(T_STAR);
					else
						firststar = opNode::Expect<TerminalNode>(T_STAR);
					
					PointerNode::StarType type = PointerNode::Plain;

					// const
					if( this->IsCurrent(T_CONST) )
					{
						this->Erase(T_CONST);
						type = PointerNode::Const;
					}
					// volatile
					else if ( this->IsCurrent(T_VOLATILE) )
					{
						this->Erase(T_VOLATILE);
						type = PointerNode::Volatile;
					}

					// const volatile
					if ( type == PointerNode::Volatile
					     && this->IsCurrent(T_CONST) )
					{
						this->Erase(T_CONST);
						type = PointerNode::ConstVolatile;
					}
					// volatile const
					else if ( type == PointerNode::Const
					&&        this->IsCurrent(T_VOLATILE) )
					{
						this->Erase(T_VOLATILE);
						type = PointerNode::ConstVolatile;
					}

					newNode->AddStar(type);
				}

				stacked<opNode> type = this->ReverseExpectOr(G_TEMPLATE_TYPE,
													   G_SCOPE,
													   T_ID,
													   T_BASIC_TYPE,
													   G_FUNDAMENTAL_TYPE,
													   *firststar);

				newNode->SetType(*type);
				newNode->AppendNode(type);

				firststar.Delete();

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// Member Pointer Interface
///==========================================

	template<class Parent> inline bool MemberPointers<Parent>::Parse()
	{
		PARSE_START;
		{
			FindMemberPointers();
		}
		PARSE_END;
	}

	template<class Parent> inline void MemberPointers<Parent>::FindMemberPointers()
	{
		LOOP_START(G_POINTER_MEMBER);
		{
			HIT(G_SCOPE)
			{
				//looking for
				//::ID::ID::*
				//how do we find this?
				//what order? before pointer? scope? or after?
				//what about G_SCOPE_POINTER? should we do it that way?
				//scope::*
				ScopeNode* scope = node_cast<ScopeNode>(this->CurrentNode());

                // kevin: I found this like this, pretty bad lol.
                // lazy evaluation for the lose
				//if(scope->Is)
			}
		}
		LOOP_END;
	}

///==========================================
/// Operators interface
///==========================================

	template<class Parent> inline bool Operators<Parent>::Parse()
	{
		PARSE_START;
		{
			FindOperators();
		}
		PARSE_END;
	}

	template<class Parent> inline void Operators<Parent>::FindOperators()
	{
		LOOP_START(G_OPERATOR);
		{
			HIT(T_OPERATOR)
			{
				stackedcontext<OperatorNode> newNode = opNode::Make<OperatorNode>(T_OPERATOR);
				
				this->Erase(T_OPERATOR);
				
				//operator ()() special case
				if(this->IsCurrent(G_PAREN_BLOCK))
				{
					//NOTE: we dont support templated operator()() : P
					stacked<ParenBlockNode> paren = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
					newNode->AppendNode(paren);
					
					//better have a paren block after...
					//Check(G_PAREN_BLOCK);
				}
				else
				{
					this->PushUntilAdd(*newNode,G_PAREN_BLOCK);
					//just to be safe?
					//Check(G_PAREN_BLOCK);
				}
				
				//opNode* oper = ExpectOverloadableOperator();
				
				this->InsertNodeAtCurrent(newNode);	
			}
		}
		LOOP_END;
	}

//==========================================
// Modifiers
//==========================================

	template<class Parent> inline bool Modifiers<Parent>::Parse()
	{
		PARSE_START;
		{
			FindModifiers();
		}
		PARSE_END;
	}

	template<class Parent> inline void Modifiers<Parent>::FindModifiers()
	{
		OPObjectNode*    parent   = opNode::FindParent<OPObjectNode>();
		DialectCategory* category = parent->GetCategorySettings();

		iterator i   = this->GetBegin();
		iterator end = this->GetEnd();

		while (i != end)
		{
			if ( i->GetId() == T_ID )
			{
				TerminalNode* node = node_cast<TerminalNode>( *i );

				if ( category->HasBasicModifier( node->GetValue() ) )
					node->SetId( T_MODIFIER );
			}

			++i;
		}
	}

///==========================================
/// ValuedModifiers
///==========================================

	template<class Parent> inline bool ValuedModifiers<Parent>::Parse()
	{
		PARSE_START;
		{
			FindValuedModifiers();
		}
		PARSE_END;
	}
	
	template<class Parent> inline void ValuedModifiers<Parent>::FindValuedModifiers()
	{
		OPObjectNode*    parent   = opNode::FindParent<OPObjectNode>();
		DialectCategory* category = parent->GetCategorySettings();

		LOOP_START(G_VALUED_MODIFIER);
		{
			HIT(T_ID)
			{
				TerminalNode* id = node_cast<TerminalNode>( this->CurrentNode() );

				if ( this->Peek(G_PAREN_BLOCK)
				&&   category->HasValueModifier( id->GetValue() ) )
				{
 					stackedcontext<ValuedModifierNode> newNode = opNode::Make<ValuedModifierNode>(T_ID);
 
 					stacked<TerminalNode> modifier = opNode::Expect<TerminalNode>(T_ID);
 
 					newNode->SetModifier(*modifier);
 					newNode->AppendNode(modifier);
 
 					stacked<ParenBlockNode>             paren     = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
 					stacked<ValuedModifierArgumentNode> arguments = opNode::Transform<ValuedModifierArgumentNode>(paren);
 
 					newNode->SetArguments(*arguments);
 					newNode->AppendNode(arguments);
 					
 					this->InsertNodeAtCurrent(newNode);
				}
				else
					this->IncrementPosition();
			}
		}
		LOOP_END;
	}

///==========================================
/// Function interface
///==========================================

	template<class Parent> inline bool Functions<Parent>::Parse()
	{
		PARSE_START;
		{
			FindFunctions();
		}
		PARSE_END;
	}

	template<class Parent> inline void Functions<Parent>::FindFunctions()
	{
		LOOP_START(G_FUNCTION);
		{
			HIT(G_PAREN_BLOCK)
			{
				stackedcontext<FunctionNode> newNode = opNode::Make<FunctionNode>(G_PAREN_BLOCK);

				stacked<ParenBlockNode>           Paren     = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
				stacked<FunctionArgumentListNode> Arguments = opNode::Transform<FunctionArgumentListNode>(Paren);

				newNode->SetArguments(*Arguments);

				// now look backwards
				stacked<opNode> Name = this->ReverseExpectOr(G_OPERATOR,
													   G_TEMPLATE_TYPE,
													   T_ID,
													   *Arguments);
				
				newNode->SetName(*Name);
				newNode->AppendNode(Name);

				newNode->AppendNode(Arguments);
				
				// check for const-ness
				if (this->IsCurrent(T_CONST))
				{
					this->Erase(T_CONST);
					newNode->SetConst(true);
				}
				
				// check for pure methods
				// NOTE: we just look for assignment = #, = ID, we dont look for null or anything like that
				if (this->IsCurrent(T_ASSIGN))
				{
					this->Erase(T_ASSIGN);
					
					stacked<TerminalNode> assignment = opNode::ExpectOr<TerminalNode>(T_NUMBER,T_ID);

					newNode->SetAssignment(*assignment);
					newNode->AppendNode(assignment);
				}
				
				this->InsertNodeAtCurrent(newNode);		
			}
		}
		LOOP_END;
	}

///==========================================
/// Interface for function declarations
///==========================================

	template<class Parent> inline bool FunctionDefinitions<Parent>::Parse()
	{
		PARSE_START;
		{
			FindFunctionDefinitions();
		}
		PARSE_END;
	}

	template<class Parent> inline void FunctionDefinitions<Parent>::FindFunctionDefinitions()
	{
		LOOP_START(G_FUNCTION_PROTOTYPE);
		{
			HIT(G_FUNCTION)
			{
				stacked<FunctionNode> Function = opNode::Expect<FunctionNode>(G_FUNCTION);
	
				//NOTE: this function should be ok, test a while.
				//TODO: fix this, this function still sucks - might be ok now..

				bool bFunctionDefinition = !this->IsCurrent(T_SEMICOLON);

				//is it a function declaration?
				if (bFunctionDefinition)
				{
					//create a new node from the block
					stackedcontext<FunctionDefinitionNode> newNode = NEWNODE(FunctionDefinitionNode());
					newNode->CopyBasics(*Function);

					newNode->SetFunction(*Function);
					
					stacked<opNode> Return = GetReturnType(*Function);
					newNode->SetReturn(*Return);		

					stacked<BraceBlockNode> Body = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
					newNode->SetBody(*Body);

					newNode->AppendNode(Return);
					newNode->AppendNode(Function);
					newNode->AppendNode(Body);
					
					this->InsertNodeAtCurrent(newNode);		
				}
				//must be a function prototype
				else
				{
					//create a new node from the block
					stackedcontext<FunctionPrototypeNode> newNode = NEWNODE(FunctionPrototypeNode());
					newNode->CopyBasics(*Function);

					newNode->SetFunction(*Function);

					stacked<opNode> Return = GetReturnType(*Function);
					newNode->SetReturn(*Return);

					newNode->AppendNode(Return);
					newNode->AppendNode(Function);

					this->InsertNodeAtCurrent(newNode);		
				}
			}
		}
		LOOP_END;
	}

	template<class Parent> inline stacked<opNode> FunctionDefinitions<Parent>::GetReturnType(opNode* after)
	{
		//find the return type
		return this->ReverseExpectOr(G_TEMPLATE_TYPE,
							   G_SCOPE,
							   G_POINTER,
							   G_POINTER_MEMBER,
							   G_REFERENCE,
							   T_ID,
							   T_BASIC_TYPE,
							   G_FUNDAMENTAL_TYPE,
							   after);
	}

///==========================================
/// Interface for constructors
///==========================================

	template<class Parent> inline bool ConstructorDefinitions<Parent>::Parse()
	{
		PARSE_START;
		{
			FindConstructorDefinitions();
		}
		PARSE_END;
	}

	template<class Parent> inline void ConstructorDefinitions<Parent>::FindConstructorDefinitions()
	{
		LOOP_START(G_CONSTRUCTOR_DEFINITION);
		{
			HIT(G_CONSTRUCTOR)
			{
				//find the constructor
				stacked<ConstructorNode> Constructor = opNode::Expect<ConstructorNode>(G_CONSTRUCTOR);

				//cases:
				//ctor() : ... {}
				//ctor() {}
				//ctor()

				bool bdefinition = this->IsCurrent(T_COLON) || this->IsCurrent(G_BRACE_BLOCK);

				if(bdefinition)
				{
					//create a new node from the block
					stackedcontext<ConstructorDefinitionNode> newNode = NEWNODE(ConstructorDefinitionNode());
					newNode->CopyBasics(*Constructor);

					//optional initializer list
					if(this->IsCurrent(T_COLON))
					{
						stacked<ConstructorInitializerListNode> initializers = opNode::PushUntil<ConstructorInitializerListNode>(G_BRACE_BLOCK);
						
						newNode->SetInitializers(*initializers);
						newNode->AppendNode(initializers);
					}
					
					stacked<BraceBlockNode> Body = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);

					newNode->SetConstructor(*Constructor);
					newNode->SetBody(*Body);
					newNode->AppendNode(Constructor);
					newNode->AppendNode(Body);

					this->InsertNodeAtCurrent(newNode);		
				}
				//must be a function prototype
				else
				{
					//create a new node from the block
					stackedcontext<ConstructorPrototypeNode> newNode = NEWNODE(ConstructorPrototypeNode());
					newNode->CopyBasics(*Constructor);

					newNode->SetConstructor(*Constructor);
					newNode->AppendNode(Constructor);
					this->InsertNodeAtCurrent(newNode);		
				}
			}
		}
		LOOP_END;
	}

///==========================================
/// Interface for Destructors
///==========================================

	template<class Parent> inline bool DestructorDefinitions<Parent>::Parse()
	{
		PARSE_START;
		{
			FindDestructorDefinitions();
		}
		PARSE_END;
	}

	template<class Parent> inline void DestructorDefinitions<Parent>::FindDestructorDefinitions()
	{
		LOOP_START(G_DESTRUCTOR_DEFINITION);
		{
			HIT(G_DESTRUCTOR)
			{
				//find the constructor
				stacked<DestructorNode> Destructor = opNode::Expect<DestructorNode>(G_DESTRUCTOR);

				//cases:
				//~dtor() {}
				//~dtor()

				bool bdefinition = this->IsCurrent(G_BRACE_BLOCK);

				if(bdefinition)
				{
					//create a new node from the block
					stackedcontext<DestructorDefinitionNode> newNode = NEWNODE(DestructorDefinitionNode());
					newNode->CopyBasics(*Destructor);

					stacked<BraceBlockNode> Body = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);

					newNode->SetDestructor(*Destructor);
					newNode->AppendNode(Destructor);
					newNode->SetBody(*Body);
					newNode->AppendNode(Body);

					this->InsertNodeAtCurrent(newNode);		
				}
				//must be a function prototype
				else
				{
					//create a new node from the block
					stackedcontext<DestructorPrototypeNode> newNode = NEWNODE(DestructorPrototypeNode());
					newNode->CopyBasics(*Destructor);

					newNode->SetDestructor(*Destructor);
					newNode->AppendNode(Destructor);
					
					this->InsertNodeAtCurrent(newNode);		
				}
			}
		}
		LOOP_END;
	}

///==========================================
/// OPEnum interface
///==========================================

	template<class Parent> inline bool OPEnums<Parent>::Parse()
	{
		PARSE_START;
		{
			FindOPEnums();
		}
		PARSE_END;
	}

	template<class Parent> inline void OPEnums<Parent>::FindOPEnums()
	{
		LOOP_START(G_OPENUM);
		{
			HIT(T_OPENUM)
			{
				stackedcontext<OPEnumNode> newNode = opNode::Make<OPEnumNode>(T_OPENUM);

				stacked<TerminalNode> identifier = opNode::Expect<TerminalNode>(T_OPENUM);
				newNode->SetIdentifier(*identifier);
				newNode->AppendNode(identifier);

				this->EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);
				newNode->SetName(*name);
				newNode->AppendNode(name);

				this->EatWhitespaceAndComments();

				stacked<BraceBlockNode> bbn		= opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<OPEnumBodyNode> eobn	= opNode::Transform<OPEnumBodyNode>(bbn);
				newNode->SetBody(*eobn);
				newNode->AppendNode(eobn);

				this->EatWhitespaceAndComments();
				this->Erase(T_SEMICOLON);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// OPObject interface
///==========================================

	template<class Parent> inline bool OPObjects<Parent>::Parse()
	{
		PARSE_START;
		{
			FindOPObjects();
		}
		PARSE_END;
	}

	template<class Parent> inline void OPObjects<Parent>::FindOPObjects()
	{
		LOOP_START(G_OPOBJECT);
		{
			HIT(T_OPOBJECT)
			{
				stackedcontext<OPObjectNode> newNode = opNode::Make<OPObjectNode>(T_OPOBJECT);

				stacked<opNode> category = opNode::Expect(T_OPOBJECT);

				newNode->SetCategory(*category);
				
				//I don't want to save this, because untransformed one is disallowed
				category.Delete();

				this->EatWhitespaceAndComments();

				// is there middle modifiers?
				if (this->IsCurrent(G_CPLUSPLUS))
				{
					stacked<CPlusPlusNode> middleModifiers = opNode::Expect<CPlusPlusNode>(G_CPLUSPLUS);

					newNode->SetMiddleModifiers(*middleModifiers);
					newNode->AppendNode(middleModifiers);
				}

				this->EatWhitespaceAndComments();
				
				// add name
				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);
				newNode->SetName(*name);
				newNode->AppendNode(name);

				this->EatWhitespaceAndComments();

				// add inheritance
				stacked<OPObjectInheritanceNode> i = opNode::PushUntil<OPObjectInheritanceNode>(G_BRACE_BLOCK);
				i->SetObject(*newNode);

				if(!i->IsEmpty())
				{
					newNode->SetInheritance(*i);
					newNode->AppendNode(i);
				}
				else
					i.Delete();

				// add opclass block
				stacked<BraceBlockNode>	  bbn  = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<OPObjectBodyNode> ocbn = opNode::Transform<OPObjectBodyNode>(bbn);

				//set a backwards pointer
				ocbn->SetObject(*newNode);

				newNode->SetBody(*ocbn);
				newNode->AppendNode(ocbn);
	
				//get rid of trailing semicolon (required now!)
				this->EatWhitespaceAndComments();

				this->Erase(T_SEMICOLON);

// 				if(this->IsCurrent(T_SEMICOLON))
// 					this->Erase(T_SEMICOLON);

				this->InsertNodeAtCurrent(newNode);
			}
		}

		LOOP_END;
	}

///==========================================
/// States interface
///==========================================

	template<class Parent> inline bool States<Parent>::Parse()
	{
		PARSE_START;

		FindStates();

		PARSE_END;
	}

	template<class Parent> inline void States<Parent>::FindStates()
	{
		LOOP_START(G_STATE);

		HIT(T_STATE)
		{
			stackedcontext<StateNode> newNode = opNode::Make<StateNode>(T_STATE);

			this->Erase(T_STATE);

			// add name
			stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);
			newNode->SetName(*name);
			newNode->AppendNode(name);

			// add state block
			stacked<BraceBlockNode> bbn	= opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
			stacked<StateBlockNode> sbn = opNode::Transform<StateBlockNode>(bbn);
			newNode->SetBody(*sbn);
			newNode->AppendNode(sbn);

			//get rid of trailing semicolons (which are optional)
			this->EatWhitespaceAndComments();
			if(this->IsCurrent(T_SEMICOLON))
				this->Erase(T_SEMICOLON);

			this->InsertNodeAtCurrent(newNode);
		}

		LOOP_END;
	}

///==========================================
/// TemplateTypes
///==========================================

	template<class Parent> inline bool TemplateTypes<Parent>::Parse()
	{
		PARSE_START;
		{
			FindTemplateTypes();
		}
		PARSE_END;
	}

	template<class Parent> inline void TemplateTypes<Parent>::FindTemplateTypes()
	{
		LOOP_START(G_TEMPLATE_TYPE);
		{
			HIT(G_ANGLED_BLOCK)
			{
				stackedcontext<TemplateTypeNode> newNode = opNode::Make<TemplateTypeNode>(G_ANGLED_BLOCK);

				// Grab the braces.
				stacked<AngledBlockNode>      braces = opNode::Expect<AngledBlockNode>(G_ANGLED_BLOCK);
				stacked<TemplateTypeBodyNode> body   = opNode::Transform<TemplateTypeBodyNode>( braces );

				newNode->SetBody(*body);
				
				// Grab the id.
				stacked<TerminalNode> id = opNode::ReverseExpect<TerminalNode>(T_ID, *body);
				
				// Grab basics from the id.
				newNode->CopyBasics(*id);
				newNode->SetName(*id);
				
				newNode->AppendNode(id);
				newNode->AppendNode(body);				
				
				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// References header
///==========================================

	template<class Parent> inline bool References<Parent>::Parse()
	{
		PARSE_START;
		{
			FindReferences();
		}
		PARSE_END;
	}

	template<class Parent> inline void References<Parent>::FindReferences()
	{
		LOOP_START(G_REFERENCE)
		{
			HIT(T_AMPERSAND)
			{
				stackedcontext<ReferenceNode> newNode = opNode::Make<ReferenceNode>(T_AMPERSAND);

				stacked<TerminalNode> ampersand = opNode::Expect<TerminalNode>(T_AMPERSAND);
				
				stacked<opNode> Type = this->ReverseExpectOr(G_POINTER,
													   G_SCOPE,
													   G_TEMPLATE_TYPE,
													   T_ID,
													   T_BASIC_TYPE,
													   G_FUNDAMENTAL_TYPE,
													   *ampersand);

				newNode->SetType(*Type);
				newNode->AppendNode(Type);

				ampersand.Delete();

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// Arrays interface
///==========================================

	template<class Parent> inline bool Arrays<Parent>::Parse()
	{
		PARSE_START;
		{
			FindArrays();
		}
		PARSE_END;
	}

	template<class Parent> inline void Arrays<Parent>::FindArrays()
	{
		LOOP_START(G_ARRAY)
		{
			HIT(G_BRACKET_BLOCK)
			{
				//TODO: 1. collect all [][]
				//2. if we have an id before it, G_ARRAY
				//3. if we have a type before it, G_TYPE_ARRAY
				
				stackedcontext<opNode> newNode = opNode::Make<opNode>(G_BRACKET_BLOCK);
				
				opArray< stacked<BracketBlockNode> > brackets;

				while(this->IsCurrent(G_BRACKET_BLOCK))
				{
					stacked<BracketBlockNode> b = opNode::Expect<BracketBlockNode>(G_BRACKET_BLOCK);
					brackets.push_back(b);
				}

				if ( this->IsPrevious(G_FUNDAMENTAL_TYPE) ||
					 this->IsPrevious(T_BASIC_TYPE) ||
					 this->IsPrevious(G_POINTER) ||
					 this->IsPrevious(G_REFERENCE) ||
					 this->IsPrevious(G_TEMPLATE_TYPE) ||
					 this->IsPrevious(G_SCOPE) )
				{
					stackedcontext<TypeArrayNode> node = opNode::Transform<TypeArrayNode>(newNode);

					//get the type
					stacked<opNode> Type = opNode::ReverseExpectOr<opNode>(G_FUNDAMENTAL_TYPE,
																		   T_BASIC_TYPE,
																		   G_POINTER,
																		   G_REFERENCE,
																		   G_TEMPLATE_TYPE,
																		   G_SCOPE,
																		   *brackets[0]);

					node->SetType(*Type);
					node->AppendNode(Type);

					AppendBrackets(brackets,*node);
					this->InsertNodeAtCurrent(node);
				}
				else 
				{
					//G_ARRAY
					stackedcontext<ArrayNode> node = opNode::Transform<ArrayNode>(newNode);
					
					//get the name
					stacked<TerminalNode> Name = opNode::ReverseExpect<TerminalNode>(T_ID, *brackets[0]);

					node->SetName(*Name);
					node->AppendNode(Name);

					AppendBrackets(brackets,*node);
					this->InsertNodeAtCurrent(node);
				}
			}
		}
		LOOP_END;
	}

    template<class Parent>
	template<typename T>
	inline void Arrays<Parent>::AppendBrackets(opArray< stacked<BracketBlockNode> >& brackets, T* node)
	{
		int num = brackets.Size();
		for(int i = 0; i < num; i++)
		{
			node->AddBracket(*brackets[i]);
			node->AppendNode(brackets[i]);
		}
	}

///==========================================
/// Visibility interface
///==========================================

	template<class Parent> inline bool VisibilityLabels<Parent>::Parse()
	{
		PARSE_START;
		{
			FindVisibilityLabels();
		}
		PARSE_END;
	}

	template<class Parent> inline void VisibilityLabels<Parent>::FindVisibilityLabels()
	{
		FindVisibilityLabel<T_PUBLIC>();
		FindVisibilityLabel<T_PRIVATE>();
		FindVisibilityLabel<T_PROTECTED>();
	}

    template<class Parent>
	template< Token token >
	inline void VisibilityLabels<Parent>::FindVisibilityLabel()
	{
		LOOP_START(G_VISIBILITY_LABEL)
		{
			HIT(token)
			{
				if(this->IsCurrent(token) && this->Peek(T_COLON))
				{
					stackedcontext<VisibilityLabelNode> newNode = opNode::Make<VisibilityLabelNode>(token);
					
					stacked<TerminalNode> Label = opNode::Expect<TerminalNode>(token);
					this->Erase(T_COLON);

					newNode->SetLabel(*Label);
					
					Label.Delete();

					this->InsertNodeAtCurrent(newNode);
				}
				else
				{
					this->IncrementPosition();
				}
			}
		}
		LOOP_END;
	}

///==========================================
/// Scope interface
///==========================================

	template<class Parent> inline bool Scopes<Parent>::Parse()
	{
		PARSE_START;
		{
			FindScopes();
		}
		PARSE_END;
	}

	template<class Parent> inline void Scopes<Parent>::FindScopes()
	{
		LOOP_START(G_SCOPE)
		{
			HIT(T_SCOPE_RESOLUTION)
			{
				//handles...
				// global:
				//[::global]
				//::global::*
				//non-global:
				//name::name
				//name::*
				
				stackedcontext<ScopeNode> scope = opNode::Make<ScopeNode>(T_SCOPE_RESOLUTION);
				
				stacked<TerminalNode> scoperesolution = opNode::Expect<TerminalNode>(T_SCOPE_RESOLUTION);
				
				bool bGlobal = false;
				opArray< stacked<opNode> > scopes;
				
				//is it a global scope or not?
				if(this->IsPrevious(T_ID) || this->IsPrevious(G_TEMPLATE_TYPE))
				{
					// get the name/initial scope
					stacked<opNode> name = this->ReverseExpectOr(T_ID, G_TEMPLATE_TYPE, *scoperesolution);
					scopes.PushBack(name);
				}
				else
				{
					bGlobal = true;
				}
				
				scoperesolution.Delete();

				bool bContinue = true;
				
				if(!bGlobal)
				{
					//name::* case
					if(CheckScopePointer(scope,scopes,bGlobal))
					{
						//got scope pointer, quit
						bContinue = false;
					}
				}
				
				while (bContinue)
				{
					//grab first forward scope
					stacked<opNode> name = this->ExpectOr(T_ID, G_TEMPLATE_TYPE);
					scopes.PushBack(name);
					
					if(this->IsCurrent(T_SCOPE_RESOLUTION))
					{
						this->Erase(T_SCOPE_RESOLUTION);
						
						//check for ...::*
						if(CheckScopePointer(scope,scopes,bGlobal))
						{
							//got scope pointer, quit
							bContinue = false;
						}
					}
					else
					{
						// ...::name
						scope->SetGlobal(bGlobal);
						
						//add the scopes to this
						int numscopes = scopes.Size();
						for(int i = 0; i < numscopes; i++)
						{
							scope->AddScope(*scopes[i]);
							scope->AppendNode(scopes[i]);
						}
						
						this->InsertNodeAtCurrent(scope);
						bContinue = false;
					}
				}
			}
		}
		LOOP_END;
	}

	template<class Parent> inline bool Scopes<Parent>::CheckScopePointer( stacked<ScopeNode>& scope, opArray< stacked< opNode > >& scopes, bool bGlobal)
	{
		if(this->IsCurrent(T_STAR))
		{
			stacked<ScopePointerNode> scopepointer = opNode::Transform<ScopePointerNode>(scope);

			int numstars = 0;
			while(this->IsCurrent(T_STAR))
			{
				this->Erase(T_STAR);
				numstars++;
			}

			scopepointer->SetNumStars(numstars);
			scopepointer->SetGlobal(bGlobal);

			//add the scopes to this
			int numscopes = scopes.Size();
			for(int i = 0; i < numscopes; i++)
			{
				scopepointer->AddScope(*scopes[i]);
				scopepointer->AppendNode(scopes[i]);
			}

			this->InsertNodeAtCurrent(scopepointer);
			return true;
		}

		return false;
	}

///==========================================
/// PointerMembers Interface
///==========================================

	template<class Parent> inline bool PointerMembers<Parent>::Parse()
	{
		PARSE_START;
		{
			FindPointerMembers();
		}
		PARSE_END;
	}

	template<class Parent> inline void PointerMembers<Parent>::FindPointerMembers()
	{
		LOOP_START(G_POINTER_MEMBER);
		{
			HIT(G_SCOPE_POINTER)
			{
				stackedcontext<PointerMemberNode> newNode = opNode::Make<PointerMemberNode>(G_SCOPE_POINTER);
				
				stacked<ScopePointerNode> scopepointer = opNode::Expect<ScopePointerNode>(G_SCOPE_POINTER);
				
				newNode->SetScope(*scopepointer);
				
				stacked<opNode> type = opNode::ReverseExpectOr(T_ID,
															   G_SCOPE,
															   G_TEMPLATE_TYPE,
															   G_POINTER,
															   G_REFERENCE,
															   T_BASIC_TYPE,
															   G_FUNDAMENTAL_TYPE,
															   *scopepointer);
				
				newNode->AppendNode(scopepointer);

				newNode->SetType(*type);
				newNode->AppendNode(type);
				
				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// Typenames interface
///==========================================

	template<class Parent> inline bool Typenames<Parent>::Parse()
	{
		PARSE_START;
		{
			FindTypenames();
		}
		PARSE_END;
	}

	template<class Parent> inline void Typenames<Parent>::FindTypenames()
	{
		LOOP_START(G_TYPENAME)
		{
			HIT(T_TYPENAME)
			{
				stackedcontext<TypenameNode> newNode = opNode::Make<TypenameNode>(T_TYPENAME);

				this->Erase(T_TYPENAME);

				opNode* name = this->ExpectOr(T_ID,G_TEMPLATE_TYPE,G_SCOPE,G_POINTER,G_REFERENCE);

				newNode->SetName(name);
				newNode->AppendNode(name);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// Function pointers
///==========================================

	template<class Parent> inline bool FunctionPointers<Parent>::Parse()
	{
		PARSE_START;
		{
			FindFunctionPointers();
		}
		PARSE_END;
	}

	template<class Parent> inline void FunctionPointers<Parent>::FindFunctionPointers()
	{
		LOOP_START(G_FUNCTION_POINTER)
		{
			HIT(G_PAREN_BLOCK)
			{
				//function pointers are always double paren blocks
				if(!this->Peek(G_PAREN_BLOCK))
				{
					//not a function pointer, move past it
					this->IncrementPosition();
					continue;
				}

				stackedcontext<FunctionPointerNode> newNode = opNode::Make<FunctionPointerNode>(G_PAREN_BLOCK);
			
				stacked<ParenBlockNode> DeclParen		= opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
				stacked<FunctionPointerDeclNode> Decl	= opNode::Transform<FunctionPointerDeclNode>(DeclParen);
				newNode->SetDecl(*Decl);
				newNode->AppendNode(Decl);
		
				stacked<ParenBlockNode> Args			= opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
				newNode->SetArguments(*Args);
				newNode->AppendNode(Args);

				this->DecrementPosition();

				//TODO: we will want to have G_CONST_TYPE maybe
				stacked<opNode> ReturnType = this->ExpectOr(G_SCOPE,
													  G_TEMPLATE_TYPE,
													  G_POINTER,
													  G_REFERENCE,
													  T_ID,
													  T_BASIC_TYPE,
													  G_FUNDAMENTAL_TYPE);

				//TODO: we really need to parse Name (why? maybe not)
				//		we also need to look at args probably
			
				newNode->SetReturn(*ReturnType);
				newNode->PrependNode(ReturnType);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// OPDefines interface
///==========================================

	template<class Parent> inline bool OPDefines<Parent>::Parse()
	{
		PARSE_START;
		{
			FindOPDefines();
		}
		PARSE_END;
	}

	template<class Parent> inline void OPDefines<Parent>::FindOPDefines()
	{
		LOOP_START(G_OPDEFINE)
		{
			HIT(T_OPDEFINE)
			{
				stackedcontext<OPDefineNode> newNode = opNode::Make<OPDefineNode>(T_OPDEFINE);
				
				this->Erase(T_OPDEFINE);
				
				this->EatWhitespaceAndComments();
				
				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);
				newNode->SetName(*name);
				newNode->AppendNode(name);
				
				this->EatWhitespaceAndComments();

				if (this->IsCurrent(G_PAREN_BLOCK))
				{
					stacked<ParenBlockNode> arguments = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
					newNode->SetArguments(*arguments);
					newNode->AppendNode(arguments);
				}
				
				this->EatWhitespaceAndComments();
				
				stacked<BraceBlockNode> bbn		= opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<OPDefineBodyNode> body	= opNode::Transform<OPDefineBodyNode>(bbn);
				newNode->SetBody(*body);
				newNode->AppendNode(body);
				
				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// Define interface
///==========================================

	template<class Parent> inline bool Preprocessors<Parent>::Parse()
	{
		PARSE_START;
		{
			FindAllPounds();
		}
		PARSE_END;
	}

	template<class Parent> inline void Preprocessors<Parent>::FindAllPounds()
	{
		FindPoundDefines();
		FindPoundIf();
		FindPoundIfDef();
		FindPoundIfnDef();
		FindPoundEndIf();
		FindPoundInclude();
		FindPoundElse();
		FindPoundError();
		FindPoundImport();
		FindPoundLine();
		FindPoundPragma();
		FindPoundUnDef();
		FindPoundUsing();
		FindPoundWarning();
	}

	//TODO: a couple possible issues remain,
	//		we need to make sure this starts on a newline only
	//		and we need to know what happens if you have a #define name \, 
	//		followed immediately by another directive 
	template<class Parent> 
    template<Token Grammar, Token HitToken, class NodeType>
	inline void Preprocessors<Parent>::FindDirective(const string& directivename)
	{
		LOOP_START(Grammar)
		{
			HIT(HitToken)
			{
				stackedcontext<NodeType> newNode = opNode::Make<NodeType>(HitToken);

				this->Erase(HitToken);

				// check and see that this #____ token is the first thing
				// on the line (except for whitespace) and error if not
				if (this->IsPrevious(T_WHITESPACE))
					this->ReverseErase(T_WHITESPACE);

				if (!this->IsPrevious(T_NEWLINE) && this->GetPosition() != this->GetBegin())
					opError::MessageError(*newNode, "Preprocessor definitions must not be preceeded by any tokens on a line except whitespace.");

				newNode->SetDirectiveName(directivename);

				bool bDone = false;

				while (!bDone)
				{
					this->PushUntilOrAdd(*newNode, T_NEWLINE, T_COMMENT, T_CCOMMENT, T_EOF, T_CONTINUELINE);
					
					if(this->CurrentNode())
					{
						if(this->CurrentNode()->GetId() == T_NEWLINE
						|| this->CurrentNode()->GetId() == T_COMMENT
						|| this->CurrentNode()->GetId() == T_CCOMMENT
						|| this->CurrentNode()->GetId() == T_EOF)
							bDone = true;
						else
						{
							stacked<TerminalNode> continueline = opNode::Expect<TerminalNode>(T_CONTINUELINE);
							newNode->AppendNode(continueline);
						}
					}
					else
						bDone = true;
				}

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

	template<class Parent> inline void Preprocessors<Parent>::FindPoundDefines()
	{
		FindDirective<G_POUND_DEFINE,T_POUND_DEFINE,PoundDefineNode>("define");
	}

	template<class Parent> inline void Preprocessors<Parent>::FindPoundIf()
	{
		FindDirective<G_POUND_IF,T_POUND_IF,PoundIfNode>("if");
	}

	template<class Parent> inline void Preprocessors<Parent>::FindPoundIfDef()
	{
		FindDirective<G_POUND_IFDEF,T_POUND_IFDEF,PoundIfdefNode>("ifdef");
	}

	template<class Parent> inline void Preprocessors<Parent>::FindPoundIfnDef()
	{
		FindDirective<G_POUND_IFNDEF,T_POUND_IFNDEF,PoundIfndefNode>("ifndef");
	}

	template<class Parent> inline void Preprocessors<Parent>::FindPoundEndIf()
	{
		FindDirective<G_POUND_ENDIF,T_POUND_ENDIF,PoundEndifNode>("endif");
	}

	template<class Parent> inline void Preprocessors<Parent>::FindPoundInclude()
	{
		FindDirective<G_POUND_INCLUDE,T_POUND_INCLUDE,PoundIncludeNode>("include");
	}

	template<class Parent> inline void Preprocessors<Parent>::FindPoundElse()
	{
		FindDirective<G_POUND_ELSE,T_POUND_ELSE,PoundElseNode>("else");
	}

	template<class Parent> inline void Preprocessors<Parent>::FindPoundError()
	{
		FindDirective<G_POUND_ERROR,T_POUND_ERROR,PoundErrorNode>("error");
	}

	template<class Parent> inline void Preprocessors<Parent>::FindPoundImport()
	{
		FindDirective<G_POUND_IMPORT,T_POUND_IMPORT,PoundImportNode>("import");
	}

	template<class Parent> inline void Preprocessors<Parent>::FindPoundLine()
	{
		FindDirective<G_POUND_LINE,T_POUND_LINE,PoundLineNode>("line");
	}

	template<class Parent> inline void Preprocessors<Parent>::FindPoundPragma()
	{
		FindDirective<G_POUND_PRAGMA,T_POUND_PRAGMA,PoundPragmaNode>("pragma");
	}

	template<class Parent> inline void Preprocessors<Parent>::FindPoundUnDef()
	{
		FindDirective<G_POUND_UNDEF,T_POUND_UNDEF,PoundUndefNode>("undef");
	}

	template<class Parent> inline void Preprocessors<Parent>::FindPoundUsing()
	{
		FindDirective<G_POUND_USING,T_POUND_USING,PoundUsingNode>("using");
	}

	template<class Parent> inline void Preprocessors<Parent>::FindPoundWarning()
	{
		FindDirective<G_POUND_WARNING,T_POUND_WARNING,PoundWarningNode>("warning");
	}

///==========================================
/// OPIncludes
///==========================================

	//NOTE: this is now a preparse option
	template<class Parent> inline bool OPIncludes<Parent>::PreParse()
	{
		PREPARSE_START;
		{
			FindOPIncludes();
		}
		PREPARSE_END;
	}

	template<class Parent> inline void OPIncludes<Parent>::FindOPIncludes()
	{
		LOOP_START(G_OPINCLUDE)
		{
			HIT(T_OPINCLUDE)
			{
				stackedcontext<OPIncludeNode> newNode = opNode::Make<OPIncludeNode>(T_OPINCLUDE);

				this->Erase(T_OPINCLUDE);

				this->EatWhitespaceAndComments();

				stacked<TerminalNode> FileName = opNode::Expect<TerminalNode>(T_STRING);

				newNode->SetFileName(*FileName);
				newNode->AppendNode(FileName);

				OPIncludeNode* node = *newNode;
				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// ExpandCalls
///==========================================

	//NOTE: this is a pre-parse interface
	template<class Parent> inline bool ExpandCalls<Parent>::PreParse()
	{
		PREPARSE_START;
		{
			FindExpandCalls();
		}
		PREPARSE_END;
	}

	template<class Parent> inline void ExpandCalls<Parent>::FindExpandCalls()
	{
		LOOP_START(G_EXPAND_CALL);
		{
			HIT(T_EXPAND)
			{
				stackedcontext<ExpandCallNode> newNode = opNode::Make<ExpandCallNode>(T_EXPAND);

				this->Erase(T_EXPAND);

				this->EatWhitespaceAndComments();

				stacked<opNode> name = this->Expect(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				this->EatWhitespaceAndComments();

				if (this->IsCurrent(G_PAREN_BLOCK))
				{
					stacked<ParenBlockNode> args = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
					stacked<ExpandCallArgumentListNode> arguments = opNode::Transform<ExpandCallArgumentListNode>(args);

					newNode->SetArguments(*arguments);
					newNode->AppendNode(arguments);
				}

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// OPMacros interface
///==========================================

	template<class Parent> inline bool OPMacros<Parent>::PreParse()
	{
		PREPARSE_START;
		{
			FindOPMacros();
		}
		PREPARSE_END;
	}

	template<class Parent> inline void OPMacros<Parent>::FindOPMacros()
	{
		LOOP_START(G_OPMACRO)
		{
			HIT(T_OPMACRO)
			{
				stackedcontext<OPMacroNode> newNode = opNode::Make<OPMacroNode>(T_OPMACRO);
				
				this->Erase(T_OPMACRO);
				
				this->EatWhitespaceAndComments();
				
				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);
				newNode->SetName(*name);
				newNode->AppendNode(name);
				
				this->EatWhitespaceAndComments();
				
				if (this->IsCurrent(G_PAREN_BLOCK))
				{
					stacked<ParenBlockNode> args = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
					stacked<OPMacroArgumentListNode> arguments = opNode::Transform<OPMacroArgumentListNode>(args);

					newNode->SetArguments(*arguments);
					newNode->AppendNode(arguments);
				}
				
				this->EatWhitespaceAndComments();
				
				stacked<BraceBlockNode> bbn   = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<OPMacroBodyNode> body = opNode::Transform<OPMacroBodyNode>(bbn);
				newNode->SetBody(*body);
				newNode->AppendNode(body);
				
				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// CPlusPluses
///==========================================

	template<class Parent> inline bool CPlusPluses<Parent>::Parse()
	{
		PARSE_START;
		{
			FindCPlusPluses();
		}
		PARSE_END;
	}

	template<class Parent> inline void CPlusPluses<Parent>::FindCPlusPluses()
	{
		LOOP_START(G_CPLUSPLUS)
		{
			HIT(T_CPLUSPLUS)
			{
				stackedcontext<CPlusPlusNode> newNode = opNode::Make<CPlusPlusNode>(T_CPLUSPLUS);

				this->Erase(T_CPLUSPLUS);

				this->EatWhitespaceAndComments();

				stacked<BraceBlockNode>    bbn  = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<CPlusPlusBodyNode> body = opNode::Transform<CPlusPlusBodyNode>(bbn);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// BasicTypes
///==========================================

	template<class Parent> inline bool FundamentalTypes<Parent>::PreParse()
	{
		PREPARSE_START;
		{
			FindSigned();
			FindUnsigned();
		}
		PREPARSE_END;
	}

	template<class Parent> inline void FundamentalTypes<Parent>::FindSigned()
	{
		LOOP_START(G_FUNDAMENTAL_TYPE)
		{
			HIT(T_SIGNED)
			{
				stackedcontext<FundamentalTypeNode> newNode = opNode::Make<FundamentalTypeNode>(T_SIGNED);

				this->Erase(T_SIGNED);

				stacked<TerminalNode> type = opNode::Expect<TerminalNode>(T_BASIC_TYPE);

				// error if the type is not signable
				if (!opScanner::IsSignable(type->GetValue()))
				{
					opError::ExpectError("signable basic type", *type);
				}
				
				newNode->SetType(*type);
				newNode->AppendNode(type);

				newNode->SetIsSigned(true);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

	template<class Parent> inline void FundamentalTypes<Parent>::FindUnsigned()
	{
		LOOP_START(G_FUNDAMENTAL_TYPE)
		{
			HIT(T_UNSIGNED)
			{
				stackedcontext<FundamentalTypeNode> newNode = opNode::Make<FundamentalTypeNode>(T_UNSIGNED);

				this->Erase(T_UNSIGNED);

				stacked<TerminalNode> type = opNode::Expect<TerminalNode>(T_BASIC_TYPE);

				// error if the type is not unsignable
				if (!opScanner::IsSignable(type->GetValue()))
					opError::ExpectError("unsignable basic type", *type);
				
				newNode->SetType(*type);
				newNode->AppendNode(type);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// Constructors
///==========================================

	template<class Parent> inline void Constructors<Parent>::FindConstructors(const opString& classname)
	{
		LOOP_START(G_CONSTRUCTOR)
		{
			HIT(G_FUNCTION)
			{
				FunctionNode* function = node_cast<FunctionNode>(this->CurrentNode());
				TerminalNode* fname = node_cast<TerminalNode>(function->GetName());
				
				if(fname && fname->GetValue() == classname)
				{
					stackedcontext<ConstructorNode> ctor = opNode::Make<ConstructorNode>(G_FUNCTION);
					stacked<FunctionNode> f = opNode::Expect<FunctionNode>(G_FUNCTION);

					ctor->SetFunction(*f);
					ctor->AppendNode(f);

					this->InsertNodeAtCurrent(ctor);
				}
				else
					this->IncrementPosition();
			}
		}
		LOOP_END	
	}

///==========================================
/// Destructors
///==========================================

	template<class Parent> inline void Destructors<Parent>::FindDestructors(const opString& classname)
	{
		LOOP_START(G_DESTRUCTOR)
		{
			HIT(G_FUNCTION)
			{
				FunctionNode* function = node_cast<FunctionNode>(this->CurrentNode());
				TerminalNode* fname = node_cast<TerminalNode>(function->GetName());

				if(fname 
				&& fname->GetValue() == classname 
				&& this->IsPrevious(T_BITWISE_COMPLEMENT))
				{
					stackedcontext<DestructorNode> dtor = opNode::Make<DestructorNode>(G_FUNCTION);
					stacked<FunctionNode> f = opNode::Expect<FunctionNode>(G_FUNCTION);
					dtor->SetFunction(*f);
					
					this->ReverseErase(T_BITWISE_COMPLEMENT);

					//do we want ~ in the member name or not?
					//fname->PrependValue("~");

   					f->GetArguments()->CleanAll();

					// make sure the destructor doesn't have arguments..
					if (!f->GetArguments()->IsEmpty())
						opError::MessageError(*f, "Destructors must have a void parameter list.");

					dtor->AppendNode(f);

					this->InsertNodeAtCurrent(dtor);
				}
				else
					this->IncrementPosition();
			}
		}
		LOOP_END	
	}

///==========================================
/// Friends
///==========================================

	template<class Parent> inline bool Friends<Parent>::Parse()
	{
		PARSE_START;
		{
			FindFriends();
		}
		PARSE_END;
	}

	template<class Parent> inline void Friends<Parent>::FindFriends()
	{
		LOOP_START(G_FRIEND);
		{
			HIT(T_FRIEND)
			{
				stackedcontext<FriendNode> newNode = opNode::Make<FriendNode>(T_FRIEND);

				this->Erase(T_FRIEND);

				stacked<opNode> first = opNode::ExpectOr(T_ID,
														 G_SCOPE,
														 G_TEMPLATE_TYPE,
														 T_CLASS,
														 T_STRUCT,
														 G_FUNCTION_PROTOTYPE,
														 G_FUNCTION_DEFINITION);
				
				Token tok = first.GetNode()->GetId();

				// If we have something of this form:
				// 
				// friend class Id; or friend struct Id;
				//
				// We know this is a forward declaration.
				if ( tok == T_CLASS || tok == T_STRUCT )
				{
 					newNode->SetForwardDecl(*first);
 					newNode->AppendNode(first);
 
 					stacked<opNode> name = opNode::ExpectOr(T_ID,
						                                    G_SCOPE,
															G_TEMPLATE_TYPE);
 
 					newNode->SetFriend(*name);
 					newNode->AppendNode(name);

					this->Erase(T_SEMICOLON);

					newNode->SetSemicolon(true);
				}
				// This could just be a friend declaration without
				// the class/struct forward declaration part.
				//
				// friend Id;
				else if ( tok != G_FUNCTION_DEFINITION )
				{
					newNode->SetFriend(*first);
					newNode->AppendNode(first);

					this->Erase(T_SEMICOLON);

					newNode->SetSemicolon(true);
				}
				// If this is a function definition, we don't require a semicolon.
				else
				{
 					newNode->SetFriend(*first);
 					newNode->AppendNode(first);
				}
								
				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// Typedefs
///==========================================

	template<class Parent> inline bool Typedefs<Parent>::Parse()
	{
		PARSE_START;
		{
			FindTypedefs();
		}
		PARSE_END;
	}

	template<class Parent> inline void Typedefs<Parent>::FindTypedefs()
	{
		LOOP_START(G_TYPEDEF);
		{
			HIT(T_TYPEDEF)
			{
				stackedcontext<TypedefNode> newNode = opNode::Make<TypedefNode>(T_TYPEDEF);
				
				this->Erase(T_TYPEDEF);

				if (this->IsCurrent(T_VOLATILE)
				||  this->IsCurrent(T_CONST))
				{
					stacked<TerminalNode> modifier = opNode::ExpectOr<TerminalNode>(T_VOLATILE, T_CONST);

					newNode->SetModifier(*modifier);
					newNode->AppendNode(modifier);
				}

				stacked<opNode> type =  opNode::ExpectOr(T_ID, 
														 G_POINTER, 
														 G_REFERENCE, 
														 G_TEMPLATE_TYPE, 
														 G_SCOPE, 
														 T_BASIC_TYPE,
														 G_FUNDAMENTAL_TYPE,
														 G_TYPE_ARRAY,
														 G_ARRAY);
				
				newNode->SetType(*type);
				newNode->AppendNode(type);
				
				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);
				
				newNode->SetName(*name);
				newNode->AppendNode(name);

				// eat the semicolon
				this->Erase( T_SEMICOLON );

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

//==========================================
// TemplateDecls
//==========================================

	template<class Parent> inline bool TemplateDecls<Parent>::Parse()
	{
		PARSE_START;
		{
			FindTemplateDecls();
		}
		PARSE_END;
	}

	template<class Parent> inline void TemplateDecls<Parent>::FindTemplateDecls()
	{
		LOOP_START(G_TEMPLATE_DECL);
		{
			HIT(T_TEMPLATE)
			{
				stackedcontext<TemplateDeclNode> newNode = opNode::Make<TemplateDeclNode>(T_TEMPLATE);
				this->Erase(T_TEMPLATE);

				stacked<AngledBlockNode> braces = opNode::Expect<AngledBlockNode>(G_ANGLED_BLOCK);

				newNode->SetBraces(*braces);
				newNode->AppendNode(braces);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// CPPConstructs
///==========================================

	template<class Parent> inline bool CPPConstructs<Parent>::Parse()
	{
		PARSE_START;
		{
			FindCPPConstructs();
		}
		PARSE_END;
	}

	template<class Parent> inline void CPPConstructs<Parent>::FindCPPConstructs()
	{
		FindCPPConstructs<EnumNode,  G_ENUM,  T_ENUM>();
		FindCPPConstructs<UnionNode, G_UNION, T_UNION>();
		
		FindCPPConstructObjects<ClassNode,  G_CLASS,  T_CLASS>();
		FindCPPConstructObjects<StructNode, G_STRUCT, T_STRUCT>();
	}

	// Find cpp constructs (enum/union).
	template<class Parent> 
    template<class NodeClass, Token Grammar, Token Hit>
	inline void CPPConstructs<Parent>::FindCPPConstructs()
	{
		LOOP_START(Grammar);
		{
			HIT(Hit)
			{
				stackedcontext<NodeClass> newNode = opNode::Make<NodeClass>(Hit);

				this->Erase(Hit);

				// If we have a name, parse it.
				if ( this->IsCurrent(T_ID) )
				{
					stacked<TerminalNode> name = opNode::Expect<TerminalNode>( T_ID );

					newNode->SetName(*name);
					newNode->AppendNode(name);
				}

				// Now we expect a block.
				stacked<BraceBlockNode> bbn = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);

				newNode->SetBody(*bbn);
				newNode->AppendNode(bbn);

				this->EatWhitespaceAndComments();
				this->Erase(T_SEMICOLON);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

	// Find cpp object constructs (struct/class).
	template<class Parent> 
    template<class NodeClass, Token Grammar, Token Hit>
	inline void CPPConstructs<Parent>::FindCPPConstructObjects()
	{
		LOOP_START(Grammar);
		{
			HIT(Hit)
			{
				stackedcontext<NodeClass> newNode = opNode::Make<NodeClass>(Hit);

				this->Erase(Hit);

				// If we have a name, parse it.
				// Also, if we have a name, look for inheritance.
				if ( this->IsCurrent(T_ID) )
				{
					stacked<TerminalNode> name = opNode::Expect<TerminalNode>( T_ID );

					newNode->SetName(*name);
					newNode->AppendNode(name);

					// Look for inheritance.
					stacked<CPPConstructInheritanceNode> i = opNode::PushUntil<CPPConstructInheritanceNode>(G_BRACE_BLOCK);

					if ( !i->IsEmpty() )
					{
						newNode->SetInheritance(*i);
						newNode->AppendNode(i);
					}
					else
						i.Delete();
				}

				// Now we expect a block.
				stacked<BraceBlockNode> bbn = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);

				newNode->SetBody(*bbn);
				newNode->AppendNode(bbn);		

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// Templated
///==========================================

	template<class Parent> inline bool Templated<Parent>::Parse()
	{
		PARSE_START;
		{
			FindTemplated();
		}
		PARSE_END;
	}

	template<class Parent> inline void Templated<Parent>::FindTemplated()
	{
		LOOP_START(G_TEMPLATED);
		{
			HIT(G_TEMPLATE_DECL)
			{
				stackedcontext<TemplatedNode> newNode     = opNode::Make<TemplatedNode>(G_TEMPLATE_DECL);
				stacked<TemplateDeclNode>     theTemplate = opNode::Expect<TemplateDeclNode>(G_TEMPLATE_DECL);

				newNode->SetTheTemplate( *theTemplate );
				newNode->AppendNode( theTemplate );

				stacked<opNode> templated = opNode::ExpectOr(T_SEMICOLON,
													         G_CLASS,
													         G_STRUCT,
													         G_UNION,
													         G_FUNCTION_DEFINITION,
													         G_FUNCTION_PROTOTYPE,
													         G_CONSTRUCTOR_DEFINITION,
													         G_DESTRUCTOR_DEFINITION);

				newNode->SetTemplated( *templated );
				newNode->AppendNode( templated );

				this->InsertNodeAtCurrent( newNode );
			}
		}
		LOOP_END;
	}

} //end namespace interface

