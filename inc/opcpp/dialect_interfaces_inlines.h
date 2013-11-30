///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: DialectInterfaces.h
/// Date: 03/24/2007
///
/// Description:
///
/// Interfaces for dialect nodes.
///****************************************************************

namespace interfaces
{

///==========================================
/// Categories Interface
///==========================================

	template<class Parent> inline bool Categories<Parent>::Parse()
	{
		PARSE_START;
		{
			FindCategories();
		}
		PARSE_END;
	}

	template<class Parent> inline void Categories<Parent>::FindCategories()
	{
		LOOP_START(G_CATEGORY)
		{
			HIT(T_CATEGORY)
			{
				stackedcontext<CategoryNode> newNode = opNode::Make<CategoryNode>(T_CATEGORY);
				
				this->Erase(T_CATEGORY);

				this->EatWhitespaceAndComments();
				
				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				this->EatWhitespaceAndComments();
				
				newNode->SetName(*name);
				newNode->AppendNode(name);
				
				stacked<BraceBlockNode>   bbn  = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<CategoryBodyNode> body = opNode::Transform<CategoryBodyNode>(bbn);
				
				newNode->SetBody(*body);
				newNode->AppendNode(body);
				
				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// CodeLocations Interface
///==========================================

	template<class Parent> inline bool CodeLocations<Parent>::PreParse()
	{
		PREPARSE_START;
		{
			FindCodeLocations();
		}
		PREPARSE_END;
	}

	template<class Parent> inline void CodeLocations<Parent>::FindCodeLocations()
	{
		LOOP_START(G_CODE)
		{
			HIT(T_CODE)
			{
				stackedcontext<CodeNode> newNode = opNode::Make<CodeNode>(T_CODE);

				this->Erase(T_CODE);

				this->EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				this->EatWhitespaceAndComments();

				newNode->SetName(*name);
				newNode->AppendNode(name);

				stacked<BraceBlockNode> bbn = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<CodeBodyNode> body = opNode::Transform<CodeBodyNode>(bbn);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// Locations Interface
///==========================================

	template<class Parent> inline bool CategoryLocations<Parent>::Parse()
	{
		PARSE_START;
		{
			FindCategoryLocations();
		}
		PARSE_END;
	}

	template<class Parent> inline void CategoryLocations<Parent>::FindCategoryLocations()
	{
		LOOP_START(G_CATEGORY_LOCATION)
		{
			HIT(T_LOCATION)
			{
				stackedcontext<CategoryLocationNode> newNode = opNode::Make<CategoryLocationNode>(T_LOCATION);
				
				this->Erase(T_LOCATION);
				
				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);
				
				stacked<BraceBlockNode> bbn = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<CategoryLocationBodyNode> body = opNode::Transform<CategoryLocationBodyNode>(bbn);
				
				newNode->SetBody(*body);
				newNode->AppendNode(body);
				
				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END
	}

///==========================================
/// Disallow Interface
///==========================================

	template<class Parent> inline bool Disallows<Parent>::Parse()
	{
		PARSE_START;
		{
			FindDisallows();
		}
		PARSE_END;
	}

	template<class Parent> inline void Disallows<Parent>::FindDisallows()
	{
		LOOP_START(G_DISALLOW)
		{
			HIT(T_DISALLOW)
			{
				stackedcontext<DisallowNode> newNode = opNode::Make<DisallowNode>(T_DISALLOW);

				this->Erase(T_DISALLOW);

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				stacked<BraceBlockNode> bbn = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<DisallowBodyNode> body = opNode::Transform<DisallowBodyNode>(bbn);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END
	}

///==========================================
/// Maps Interface
///==========================================

    template<class Parent> inline bool CategoryMaps<Parent>::Parse()
	{
		PARSE_START;
		{
			FindCategoryMaps();
		}
		PARSE_END;
	}

	template<class Parent> inline void CategoryMaps<Parent>::FindCategoryMaps()
	{
		FindCategoryMap<G_CATEGORY_DATAMAP,     T_DATAMAP,     CategoryDatamapNode>();
		FindCategoryMap<G_CATEGORY_FUNCTIONMAP, T_FUNCTIONMAP, CategoryFunctionmapNode>();
	}

	template<class Parent> 	
	template<Token grammar, Token token, class NodeClass>
    inline void CategoryMaps<Parent>::FindCategoryMap()
	{
		LOOP_START(grammar)
		{
			HIT(token)
			{
				stackedcontext<NodeClass> newNode = opNode::Make<NodeClass>(token);
				
				this->Erase(token);
				
				stacked<TerminalNode>        name = opNode::Expect<TerminalNode>(T_ID);
				
				newNode->SetName(*name);
				newNode->AppendNode(name);
				
				if(this->IsCurrent(G_BRACE_BLOCK))
				{
					stacked<BraceBlockNode>      bbn  = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
					stacked<CategoryMapBodyNode> body = opNode::Transform<CategoryMapBodyNode>(bbn);

					newNode->SetBody(*body);
					newNode->AppendNode(body);
				}


				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// DataModifiers Interface
///==========================================

	template<class Parent> inline bool DataModifiers<Parent>::Parse()
	{
		PARSE_START;
		{
			FindDataModifiers();
		}
		PARSE_END;
	}

	template<class Parent> inline void DataModifiers<Parent>::FindDataModifiers()
	{
		LOOP_START(G_DATAMODIFIER)
		{
			HIT(T_DATAMODIFIER)
			{
				stackedcontext<DataModifierNode> newNode = opNode::Make<DataModifierNode>(T_DATAMODIFIER);
				
				this->Erase(T_DATAMODIFIER);

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);
				
				newNode->SetName(*name);
				newNode->AppendNode(name);

				if(this->IsCurrent(G_PAREN_BLOCK))
				{
					stacked<ParenBlockNode> paren = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
					stacked<ModifierArgumentNode> args = opNode::Transform<ModifierArgumentNode>(paren);

					newNode->SetArgument(*args);
					newNode->AppendNode(args);
				}

				this->Erase(T_SEMICOLON);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// FunctionModifiers Interface
///==========================================

	template<class Parent> inline bool FunctionModifiers<Parent>::Parse()
	{
		PARSE_START;
		{
			FindFunctionModifiers();
		}
		PARSE_END;
	}

	template<class Parent> inline void FunctionModifiers<Parent>::FindFunctionModifiers()
	{
		LOOP_START(G_FUNCTIONMODIFIER)
		{
			HIT(T_FUNCTIONMODIFIER)
			{
				stackedcontext<FunctionModifierNode> newNode = opNode::Make<FunctionModifierNode>(T_FUNCTIONMODIFIER);

				this->Erase(T_FUNCTIONMODIFIER);

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				if(this->IsCurrent(G_PAREN_BLOCK))
				{
					stacked<ParenBlockNode> paren = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
					stacked<ModifierArgumentNode> args = opNode::Transform<ModifierArgumentNode>(paren);

					newNode->SetArgument(*args);
					newNode->AppendNode(args);
				}
				
				this->Erase(T_SEMICOLON);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// CriteriaExpressions
///==========================================

	template<class Parent> inline bool CriteriaExpressions<Parent>::Parse()
	{
		PARSE_START;
		{
			FindCriteriaExpressions();
		}
		PARSE_END;
	}

	template<class Parent> inline void CriteriaExpressions<Parent>::FindCriteriaExpressions()
	{
		FindIs<G_IS, T_IS, IsNode, IsBodyNode>();
//		FindIs<G_ISNOT, T_ISNOT, IsnotNode, IsnotBodyNode>();
//		FindEverything();
	}

	template<class Parent> 
	template<Token grammar, Token token, class NodeClass, class NodeBodyClass>
    inline void CriteriaExpressions<Parent>::FindIs()
	{
		LOOP_START(grammar)
		{
			HIT(token)
			{
				stackedcontext<NodeClass> newNode = opNode::Make<NodeClass>(token);
				
				this->Erase(token);

				stacked<NodeBodyClass> body = opNode::PushUntil<NodeBodyClass>(T_SEMICOLON);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				this->Erase(T_SEMICOLON);
				
				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END
	}

///==========================================
/// Notes
///==========================================

	template<class Parent> inline bool Notes<Parent>::Parse()
	{
		PARSE_START;
		{
			FindNotes();
		}
		PARSE_END;
	}

	template<class Parent> inline void Notes<Parent>::FindNotes()
	{
		LOOP_START(G_NOTE)
		{
			HIT(T_NOTE)
			{
				stackedcontext<NoteNode> newNode = opNode::Make<NoteNode>(T_NOTE);

				this->Erase(T_NOTE);

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				if(this->IsCurrent(G_BRACE_BLOCK))
				{
					stacked<BraceBlockNode> bbn = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
					stacked<NoteBodyNode> body = opNode::Transform<NoteBodyNode>(bbn);

					newNode->SetBody(*body);
					newNode->AppendNode(body);
				}
				else
					this->Erase(T_SEMICOLON);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// NoteDefinitions
///==========================================

	template<class Parent> inline bool NoteDefinitions<Parent>::Parse()
	{
		PARSE_START;
		{
			FindNoteDefinitions();
		}
		PARSE_END;
	}

	template<class Parent> inline void NoteDefinitions<Parent>::FindNoteDefinitions()
	{
		LOOP_START(G_NOTE_DEFINITION)
		{
			HIT(T_NOTE)
			{
				stackedcontext<NoteDefinitionNode> newNode = opNode::Make<NoteDefinitionNode>(T_NOTE);
				
				this->Erase(T_NOTE);

				this->EatWhitespaceAndComments();

				stacked<ScopeNode> pathnode = opNode::Expect<ScopeNode>(G_SCOPE);

				newNode->SetPath(*pathnode);
				newNode->AppendNode(pathnode);

				this->EatWhitespaceAndComments();

				stacked<ParenBlockNode>       ppn  = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
				stacked<NoteArgumentListNode> args = opNode::Transform<NoteArgumentListNode>(ppn);

				newNode->SetArguments(*args);
				newNode->AppendNode(args);

				this->EatWhitespaceAndComments();

				stacked<BraceBlockNode> body = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				
				newNode->SetBody(*body);
				newNode->AppendNode(body);
				
				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// Enumerations Interface
///==========================================

	template<class Parent> inline bool Enumerations<Parent>::Parse()
	{
		PARSE_START;
		{
			FindEnumerations();
		}
		PARSE_END;
	}

	template<class Parent> inline void Enumerations<Parent>::FindEnumerations()
	{
		LOOP_START(G_ENUMERATION)
		{
			HIT(T_ENUMERATION)
			{
				stackedcontext<EnumerationNode> newNode = opNode::Make<EnumerationNode>(T_ENUMERATION);

				this->Erase(T_ENUMERATION);

				this->EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				this->EatWhitespaceAndComments();

				newNode->SetName(*name);
				newNode->AppendNode(name);

				stacked<BraceBlockNode>      bbn  = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<EnumerationBodyNode> body = opNode::Transform<EnumerationBodyNode>(bbn);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// EnumerationLocations
///==========================================

	template<class Parent> inline bool EnumerationLocations<Parent>::Parse()
	{
		PARSE_START;
		{
			FindEnumerationLocations();
		}
		PARSE_END;
	}

	template<class Parent> inline void EnumerationLocations<Parent>::FindEnumerationLocations()
	{
		LOOP_START(G_ENUMERATION_LOCATION)
		{
			HIT(T_LOCATION)
			{
				stackedcontext<EnumerationLocationNode> newNode = opNode::Make<EnumerationLocationNode>(T_LOCATION);

				this->Erase(T_LOCATION);

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				stacked<BraceBlockNode>              bbn  = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<EnumerationLocationBodyNode> body = opNode::Transform<EnumerationLocationBodyNode>(bbn);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END
	}

///==========================================
/// EnumerationMaps
///==========================================

	template<class Parent> inline bool EnumerationMaps<Parent>::Parse()
	{
		PARSE_START;
		{
			FindEnumerationMaps();
		}
		PARSE_END;
	}

	template<class Parent> inline void EnumerationMaps<Parent>::FindEnumerationMaps()
	{
		LOOP_START(G_ENUMERATION_MAP)
		{
			HIT(T_ENUMMAP)
			{
				stackedcontext<EnumerationMapNode> newNode = opNode::Make<EnumerationMapNode>(T_ENUMMAP);

				opNode::Erase(T_ENUMMAP);

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// Dialect Modifiers
///==========================================

	template<class Parent> 
	template<Token grammar, class ModifierClass>
    inline void DialectModifiers<Parent>::FindDialectModifier()
	{
		LOOP_START(grammar)
		{
			HIT(T_ID)
			{
				TerminalNode* t = node_cast<TerminalNode>(this->CurrentNode());

				if (t->GetValue() == ModifierClass::StaticModifierName())
				{
					stackedcontext<ModifierClass> newNode = opNode::Make<ModifierClass>(T_ID);

					this->Erase(T_ID);
					
					this->InsertNodeAtCurrent(newNode);
				}
				else
					this->IncrementPosition();
			}
		}
		LOOP_END;
	}

	template<class Parent> 
	template<Token grammar, class ModifierClass>
    inline void DialectModifiers<Parent>::FindValuedDialectModifier()
	{
		LOOP_START(grammar)
		{
			HIT(T_ID)
			{
				TerminalNode* t = node_cast<TerminalNode>(this->CurrentNode());

				if (t->GetValue() == ModifierClass::StaticModifierName())
				{
					stackedcontext<ModifierClass> newNode = opNode::Make<ModifierClass>(T_ID);

					this->Erase(T_ID);

					stacked<ParenBlockNode> value = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);

					newNode->SetValue(*value);
					newNode->AppendNode(value);

					this->InsertNodeAtCurrent(newNode);
				}
				else
					this->IncrementPosition();
			}
		}
		LOOP_END;
	}

///==========================================
/// CriteriaValueModifiers
///==========================================

	template<class Parent> inline bool CriteriaValueModifiers<Parent>::Parse()
	{
		PARSE_START;
		{
			FindCriteriaValueModifiers();
		}	
		PARSE_END;
	}

	template<class Parent> inline void CriteriaValueModifiers<Parent>::FindCriteriaValueModifiers()
	{
		LOOP_START(G_CRITERIA_VALUE_MODIFIER)
		{
			HIT(G_PAREN_BLOCK)
			{
				if (this->IsPrevious(T_ID))
				{
					stackedcontext<CriteriaValueModifierNode> newNode = opNode::Make<CriteriaValueModifierNode>(G_PAREN_BLOCK);

					stacked<ParenBlockNode>		  paren = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
					stacked<CriteriaArgumentNode> arg   = opNode::Transform<CriteriaArgumentNode>(paren);

					newNode->SetArgument(*arg);

					stacked<TerminalNode> id = opNode::ReverseExpect<TerminalNode>(T_ID, *arg);

					// Grab basics from the id.
					newNode->CopyBasics(*id);
					newNode->SetName(*id);

					newNode->AppendNode(id);
					newNode->AppendNode(arg);

					this->InsertNodeAtCurrent(newNode);
				}
				else
					this->IncrementPosition();
			}
		}
		LOOP_END;
	}

///==========================================
/// CriteriaGroups
///==========================================

	template<class Parent> inline bool CriteriaGroups<Parent>::Parse()
	{
		PARSE_START;
		{
			FindCriteriaGroups();
		}
		PARSE_END;
	}

	template<class Parent> inline void CriteriaGroups<Parent>::FindCriteriaGroups()
	{
		LOOP_START(G_CRITERIA_GROUP)
		{
			HIT(G_PAREN_BLOCK)
			{
				stacked<ParenBlockNode> paren = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
				stacked<CriteriaGroupNode> newNode = opNode::Transform<CriteriaGroupNode>(paren);
				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

//==========================================
// Extensions
//==========================================

	template<class Parent> inline bool Extensions<Parent>::Parse()
	{
		PARSE_START;
		{
			FindExtensions();
		}
		PARSE_END;
	}

	template<class Parent> inline void Extensions<Parent>::FindExtensions()
	{
		LOOP_START(G_EXTENSION)
		{
			HIT(T_EXTENSION)
			{
				stackedcontext<ExtensionNode> newNode = opNode::Make<ExtensionNode>(T_EXTENSION);

				this->Erase(T_EXTENSION);

				this->EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				this->EatWhitespaceAndComments();

				newNode->SetName(*name);
				newNode->AppendNode(name);

				this->Erase(T_SEMICOLON);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

//==========================================
// ExtendPoints
//==========================================

	template<class Parent> inline bool ExtendPoints<Parent>::Parse()
	{
		PARSE_START;
		{
			FindExtendPoints();
		}
		PARSE_END;
	}

	template<class Parent> inline void ExtendPoints<Parent>::FindExtendPoints()
	{
		LOOP_START(G_EXTEND_POINT)
		{
			HIT(T_EXTEND_POINT)
			{
				stackedcontext<ExtendPointNode> newNode = opNode::Make<ExtendPointNode>(T_EXTEND_POINT);

				this->Erase(T_EXTEND_POINT);

				this->EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				this->EatWhitespaceAndComments();

				newNode->SetName(*name);
				newNode->AppendNode(name);

				stacked<BraceBlockNode> body = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

//==========================================
// ExtensionPoints
//==========================================

	template<class Parent> inline bool ExtensionPoints<Parent>::Parse()
	{
		PARSE_START;
		{
			FindExtensionPoints();
		}
		PARSE_END;
	}

	template<class Parent> inline void ExtensionPoints<Parent>::FindExtensionPoints()
	{
		LOOP_START(G_EXTENSION_POINT)
		{
			HIT(T_EXTENSION_POINT)
			{
				stackedcontext<ExtensionPointNode> newNode = opNode::Make<ExtensionPointNode>(T_EXTENSION_POINT);

				this->Erase(T_EXTENSION_POINT);

				this->EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

//==========================================
// FileDeclarationLocations
//==========================================

	template<class Parent> inline bool FileDeclarationLocations<Parent>::Parse()
	{
		PARSE_START;
		{
			FindFileDeclarationLocations();
		}
		PARSE_END;
	}

	template<class Parent> inline void FileDeclarationLocations<Parent>::FindFileDeclarationLocations()
	{
		LOOP_START(G_FILE_DECLARATION_LOCATION)
		{
			HIT(T_LOCATION)
			{
				stackedcontext<FileDeclarationLocationNode> newNode = opNode::Make<FileDeclarationLocationNode>(T_LOCATION);

				this->Erase(T_LOCATION);

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END
	}

//==========================================
// FileDeclarations
//==========================================

	template<class Parent> inline bool FileDeclarations<Parent>::Parse()
	{
		PARSE_START;
		{
			FindFileDeclarations();
		}
		PARSE_END;
	}

	template<class Parent> inline void FileDeclarations<Parent>::FindFileDeclarations()
	{
		LOOP_START(G_FILE_DECLARATION)
		{
			HIT(T_FILE_DECLARATION)
			{
				stackedcontext<FileDeclarationNode> newNode = opNode::Make<FileDeclarationNode>(T_FILE_DECLARATION);

				this->Erase(T_FILE_DECLARATION);

				this->EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				this->EatWhitespaceAndComments();

				stacked<BraceBlockNode>          bbn  = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<FileDeclarationBodyNode> body = opNode::Transform<FileDeclarationBodyNode>(bbn);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

//==========================================
// DialectNamespaces
//==========================================

	template<class Parent> inline bool DialectNamespaces<Parent>::Parse()
	{
		PARSE_START;
		{
			FindDialectNamespaces();
		}
		PARSE_END;
	}

	template<class Parent> inline void DialectNamespaces<Parent>::FindDialectNamespaces()
	{
		LOOP_START(G_DIALECT_NAMESPACE)
		{
			HIT(T_NAMESPACE)
			{
				stackedcontext<DialectNamespaceNode> newNode = opNode::Make<DialectNamespaceNode>(T_NAMESPACE);

				this->Erase(T_NAMESPACE);

				this->EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				this->EatWhitespaceAndComments();

				stacked<BraceBlockNode> body = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				this->InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

} // end namespace interfaces
