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

	inline bool Categories::Parse()
	{
		PARSE_START;
		{
			FindCategories();
		}
		PARSE_END;
	}

	inline void Categories::FindCategories()
	{
		LOOP_START(G_CATEGORY)
		{
			HIT(T_CATEGORY)
			{
				stackedcontext<CategoryNode> newNode = opNode::Make<CategoryNode>(T_CATEGORY);
				
				Erase(T_CATEGORY);

				EatWhitespaceAndComments();
				
				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				EatWhitespaceAndComments();
				
				newNode->SetName(*name);
				newNode->AppendNode(name);
				
				stacked<BraceBlockNode>   bbn  = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<CategoryBodyNode> body = opNode::Transform<CategoryBodyNode>(bbn);
				
				newNode->SetBody(*body);
				newNode->AppendNode(body);
				
				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// CodeLocations Interface
///==========================================

	inline bool CodeLocations::PreParse()
	{
		PREPARSE_START;
		{
			FindCodeLocations();
		}
		PREPARSE_END;
	}

	inline void CodeLocations::FindCodeLocations()
	{
		LOOP_START(G_CODE)
		{
			HIT(T_CODE)
			{
				stackedcontext<CodeNode> newNode = opNode::Make<CodeNode>(T_CODE);

				Erase(T_CODE);

				EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				EatWhitespaceAndComments();

				newNode->SetName(*name);
				newNode->AppendNode(name);

				stacked<BraceBlockNode> bbn = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<CodeBodyNode> body = opNode::Transform<CodeBodyNode>(bbn);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// Locations Interface
///==========================================

	inline bool CategoryLocations::Parse()
	{
		PARSE_START;
		{
			FindCategoryLocations();
		}
		PARSE_END;
	}

	inline void CategoryLocations::FindCategoryLocations()
	{
		LOOP_START(G_CATEGORY_LOCATION)
		{
			HIT(T_LOCATION)
			{
				stackedcontext<CategoryLocationNode> newNode = opNode::Make<CategoryLocationNode>(T_LOCATION);
				
				Erase(T_LOCATION);
				
				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);
				
				stacked<BraceBlockNode> bbn = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<CategoryLocationBodyNode> body = opNode::Transform<CategoryLocationBodyNode>(bbn);
				
				newNode->SetBody(*body);
				newNode->AppendNode(body);
				
				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END
	}

///==========================================
/// Disallow Interface
///==========================================

	inline bool Disallows::Parse()
	{
		PARSE_START;
		{
			FindDisallows();
		}
		PARSE_END;
	}

	inline void Disallows::FindDisallows()
	{
		LOOP_START(G_DISALLOW)
		{
			HIT(T_DISALLOW)
			{
				stackedcontext<DisallowNode> newNode = opNode::Make<DisallowNode>(T_DISALLOW);

				Erase(T_DISALLOW);

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				stacked<BraceBlockNode> bbn = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<DisallowBodyNode> body = opNode::Transform<DisallowBodyNode>(bbn);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END
	}

///==========================================
/// Maps Interface
///==========================================

    inline bool CategoryMaps::Parse()
	{
		PARSE_START;
		{
			FindCategoryMaps();
		}
		PARSE_END;
	}

	inline void CategoryMaps::FindCategoryMaps()
	{
		FindCategoryMap<G_CATEGORY_DATAMAP,     T_DATAMAP,     CategoryDatamapNode>();
		FindCategoryMap<G_CATEGORY_FUNCTIONMAP, T_FUNCTIONMAP, CategoryFunctionmapNode>();
	}
	
	template<Token grammar, Token token, class NodeClass>
	inline void CategoryMaps::FindCategoryMap()
	{
		LOOP_START(grammar)
		{
			HIT(token)
			{
				stackedcontext<NodeClass> newNode = opNode::Make<NodeClass>(token);
				
				Erase(token);
				
				stacked<TerminalNode>        name = opNode::Expect<TerminalNode>(T_ID);
				
				newNode->SetName(*name);
				newNode->AppendNode(name);
				
				if(IsCurrent(G_BRACE_BLOCK))
				{
					stacked<BraceBlockNode>      bbn  = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
					stacked<CategoryMapBodyNode> body = opNode::Transform<CategoryMapBodyNode>(bbn);

					newNode->SetBody(*body);
					newNode->AppendNode(body);
				}


				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// DataModifiers Interface
///==========================================

	inline bool DataModifiers::Parse()
	{
		PARSE_START;
		{
			FindDataModifiers();
		}
		PARSE_END;
	}

	inline void DataModifiers::FindDataModifiers()
	{
		LOOP_START(G_DATAMODIFIER)
		{
			HIT(T_DATAMODIFIER)
			{
				stackedcontext<DataModifierNode> newNode = opNode::Make<DataModifierNode>(T_DATAMODIFIER);
				
				Erase(T_DATAMODIFIER);

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);
				
				newNode->SetName(*name);
				newNode->AppendNode(name);

				if(IsCurrent(G_PAREN_BLOCK))
				{
					stacked<ParenBlockNode> paren = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
					stacked<ModifierArgumentNode> args = opNode::Transform<ModifierArgumentNode>(paren);

					newNode->SetArgument(*args);
					newNode->AppendNode(args);
				}

				Erase(T_SEMICOLON);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// FunctionModifiers Interface
///==========================================

	inline bool FunctionModifiers::Parse()
	{
		PARSE_START;
		{
			FindFunctionModifiers();
		}
		PARSE_END;
	}

	inline void FunctionModifiers::FindFunctionModifiers()
	{
		LOOP_START(G_FUNCTIONMODIFIER)
		{
			HIT(T_FUNCTIONMODIFIER)
			{
				stackedcontext<FunctionModifierNode> newNode = opNode::Make<FunctionModifierNode>(T_FUNCTIONMODIFIER);

				Erase(T_FUNCTIONMODIFIER);

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				if(IsCurrent(G_PAREN_BLOCK))
				{
					stacked<ParenBlockNode> paren = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
					stacked<ModifierArgumentNode> args = opNode::Transform<ModifierArgumentNode>(paren);

					newNode->SetArgument(*args);
					newNode->AppendNode(args);
				}
				
				Erase(T_SEMICOLON);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// CriteriaExpressions
///==========================================

	inline bool CriteriaExpressions::Parse()
	{
		PARSE_START;
		{
			FindCriteriaExpressions();
		}
		PARSE_END;
	}

	inline void CriteriaExpressions::FindCriteriaExpressions()
	{
		FindIs<G_IS, T_IS, IsNode, IsBodyNode>();
//		FindIs<G_ISNOT, T_ISNOT, IsnotNode, IsnotBodyNode>();
//		FindEverything();
	}

	template<Token grammar, Token token, class NodeClass, class NodeBodyClass>
	inline void CriteriaExpressions::FindIs()
	{
		LOOP_START(grammar)
		{
			HIT(token)
			{
				stackedcontext<NodeClass> newNode = opNode::Make<NodeClass>(token);
				
				Erase(token);

				stacked<NodeBodyClass> body = opNode::PushUntil<NodeBodyClass>(T_SEMICOLON);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				Erase(T_SEMICOLON);
				
				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END
	}

///==========================================
/// Notes
///==========================================

	inline bool Notes::Parse()
	{
		PARSE_START;
		{
			FindNotes();
		}
		PARSE_END;
	}

	inline void Notes::FindNotes()
	{
		LOOP_START(G_NOTE)
		{
			HIT(T_NOTE)
			{
				stackedcontext<NoteNode> newNode = opNode::Make<NoteNode>(T_NOTE);

				Erase(T_NOTE);

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				if(IsCurrent(G_BRACE_BLOCK))
				{
					stacked<BraceBlockNode> bbn = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
					stacked<NoteBodyNode> body = opNode::Transform<NoteBodyNode>(bbn);

					newNode->SetBody(*body);
					newNode->AppendNode(body);
				}
				else
					Erase(T_SEMICOLON);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// NoteDefinitions
///==========================================

	inline bool NoteDefinitions::Parse()
	{
		PARSE_START;
		{
			FindNoteDefinitions();
		}
		PARSE_END;
	}

	inline void NoteDefinitions::FindNoteDefinitions()
	{
		LOOP_START(G_NOTE_DEFINITION)
		{
			HIT(T_NOTE)
			{
				stackedcontext<NoteDefinitionNode> newNode = opNode::Make<NoteDefinitionNode>(T_NOTE);
				
				Erase(T_NOTE);

				EatWhitespaceAndComments();

				stacked<ScopeNode> pathnode = opNode::Expect<ScopeNode>(G_SCOPE);

				newNode->SetPath(*pathnode);
				newNode->AppendNode(pathnode);

				EatWhitespaceAndComments();

				stacked<ParenBlockNode>       ppn  = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
				stacked<NoteArgumentListNode> args = opNode::Transform<NoteArgumentListNode>(ppn);

				newNode->SetArguments(*args);
				newNode->AppendNode(args);

				EatWhitespaceAndComments();

				stacked<BraceBlockNode> body = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				
				newNode->SetBody(*body);
				newNode->AppendNode(body);
				
				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// Enumerations Interface
///==========================================

	inline bool Enumerations::Parse()
	{
		PARSE_START;
		{
			FindEnumerations();
		}
		PARSE_END;
	}

	inline void Enumerations::FindEnumerations()
	{
		LOOP_START(G_ENUMERATION)
		{
			HIT(T_ENUMERATION)
			{
				stackedcontext<EnumerationNode> newNode = opNode::Make<EnumerationNode>(T_ENUMERATION);

				Erase(T_ENUMERATION);

				EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				EatWhitespaceAndComments();

				newNode->SetName(*name);
				newNode->AppendNode(name);

				stacked<BraceBlockNode>      bbn  = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<EnumerationBodyNode> body = opNode::Transform<EnumerationBodyNode>(bbn);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// EnumerationLocations
///==========================================

	inline bool EnumerationLocations::Parse()
	{
		PARSE_START;
		{
			FindEnumerationLocations();
		}
		PARSE_END;
	}

	inline void EnumerationLocations::FindEnumerationLocations()
	{
		LOOP_START(G_ENUMERATION_LOCATION)
		{
			HIT(T_LOCATION)
			{
				stackedcontext<EnumerationLocationNode> newNode = opNode::Make<EnumerationLocationNode>(T_LOCATION);

				Erase(T_LOCATION);

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				stacked<BraceBlockNode>              bbn  = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<EnumerationLocationBodyNode> body = opNode::Transform<EnumerationLocationBodyNode>(bbn);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END
	}

///==========================================
/// EnumerationMaps
///==========================================

	inline bool EnumerationMaps::Parse()
	{
		PARSE_START;
		{
			FindEnumerationMaps();
		}
		PARSE_END;
	}

	inline void EnumerationMaps::FindEnumerationMaps()
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

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

///==========================================
/// Dialect Modifiers
///==========================================

	template<Token grammar, class ModifierClass>
	inline void DialectModifiers::FindDialectModifier()
	{
		LOOP_START(grammar)
		{
			HIT(T_ID)
			{
				TerminalNode* t = node_cast<TerminalNode>(CurrentNode());

				if (t->GetValue() == ModifierClass::StaticModifierName())
				{
					stackedcontext<ModifierClass> newNode = opNode::Make<ModifierClass>(T_ID);

					Erase(T_ID);
					
					InsertNodeAtCurrent(newNode);
				}
				else
					IncrementPosition();
			}
		}
		LOOP_END;
	}

	template<Token grammar, class ModifierClass>
	inline void DialectModifiers::FindValuedDialectModifier()
	{
		LOOP_START(grammar)
		{
			HIT(T_ID)
			{
				TerminalNode* t = node_cast<TerminalNode>(CurrentNode());

				if (t->GetValue() == ModifierClass::StaticModifierName())
				{
					stackedcontext<ModifierClass> newNode = opNode::Make<ModifierClass>(T_ID);

					Erase(T_ID);

					stacked<ParenBlockNode> value = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);

					newNode->SetValue(*value);
					newNode->AppendNode(value);

					InsertNodeAtCurrent(newNode);
				}
				else
					IncrementPosition();
			}
		}
		LOOP_END;
	}

///==========================================
/// CriteriaValueModifiers
///==========================================

	inline bool CriteriaValueModifiers::Parse()
	{
		PARSE_START;
		{
			FindCriteriaValueModifiers();
		}	
		PARSE_END;
	}

	inline void CriteriaValueModifiers::FindCriteriaValueModifiers()
	{
		LOOP_START(G_CRITERIA_VALUE_MODIFIER)
		{
			HIT(G_PAREN_BLOCK)
			{
				if (IsPrevious(T_ID))
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

					InsertNodeAtCurrent(newNode);
				}
				else
					IncrementPosition();
			}
		}
		LOOP_END;
	}

///==========================================
/// CriteriaGroups
///==========================================

	inline bool CriteriaGroups::Parse()
	{
		PARSE_START;
		{
			FindCriteriaGroups();
		}
		PARSE_END;
	}

	inline void CriteriaGroups::FindCriteriaGroups()
	{
		LOOP_START(G_CRITERIA_GROUP)
		{
			HIT(G_PAREN_BLOCK)
			{
				stacked<ParenBlockNode> paren = opNode::Expect<ParenBlockNode>(G_PAREN_BLOCK);
				stacked<CriteriaGroupNode> newNode = opNode::Transform<CriteriaGroupNode>(paren);
				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}
};

//==========================================
// Extensions
//==========================================

	inline bool Extensions::Parse()
	{
		PARSE_START;
		{
			FindExtensions();
		}
		PARSE_END;
	}

	inline void Extensions::FindExtensions()
	{
		LOOP_START(G_EXTENSION)
		{
			HIT(T_EXTENSION)
			{
				stackedcontext<ExtensionNode> newNode = opNode::Make<ExtensionNode>(T_EXTENSION);

				Erase(T_EXTENSION);

				EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				EatWhitespaceAndComments();

				newNode->SetName(*name);
				newNode->AppendNode(name);

				Erase(T_SEMICOLON);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

//==========================================
// ExtendPoints
//==========================================

	inline bool ExtendPoints::Parse()
	{
		PARSE_START;
		{
			FindExtendPoints();
		}
		PARSE_END;
	}

	inline void ExtendPoints::FindExtendPoints()
	{
		LOOP_START(G_EXTEND_POINT)
		{
			HIT(T_EXTEND_POINT)
			{
				stackedcontext<ExtendPointNode> newNode = opNode::Make<ExtendPointNode>(T_EXTEND_POINT);

				Erase(T_EXTEND_POINT);

				EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				EatWhitespaceAndComments();

				newNode->SetName(*name);
				newNode->AppendNode(name);

				stacked<BraceBlockNode> body = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

//==========================================
// ExtensionPoints
//==========================================

	inline bool ExtensionPoints::Parse()
	{
		PARSE_START;
		{
			FindExtensionPoints();
		}
		PARSE_END;
	}

	inline void ExtensionPoints::FindExtensionPoints()
	{
		LOOP_START(G_EXTENSION_POINT)
		{
			HIT(T_EXTENSION_POINT)
			{
				stackedcontext<ExtensionPointNode> newNode = opNode::Make<ExtensionPointNode>(T_EXTENSION_POINT);

				Erase(T_EXTENSION_POINT);

				EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

//==========================================
// FileDeclarationLocations
//==========================================

	inline bool FileDeclarationLocations::Parse()
	{
		PARSE_START;
		{
			FindFileDeclarationLocations();
		}
		PARSE_END;
	}

	inline void FileDeclarationLocations::FindFileDeclarationLocations()
	{
		LOOP_START(G_FILE_DECLARATION_LOCATION)
		{
			HIT(T_LOCATION)
			{
				stackedcontext<FileDeclarationLocationNode> newNode = opNode::Make<FileDeclarationLocationNode>(T_LOCATION);

				Erase(T_LOCATION);

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END
	}

//==========================================
// FileDeclarations
//==========================================

	inline bool FileDeclarations::Parse()
	{
		PARSE_START;
		{
			FindFileDeclarations();
		}
		PARSE_END;
	}

	inline void FileDeclarations::FindFileDeclarations()
	{
		LOOP_START(G_FILE_DECLARATION)
		{
			HIT(T_FILE_DECLARATION)
			{
				stackedcontext<FileDeclarationNode> newNode = opNode::Make<FileDeclarationNode>(T_FILE_DECLARATION);

				Erase(T_FILE_DECLARATION);

				EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				EatWhitespaceAndComments();

				stacked<BraceBlockNode>          bbn  = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);
				stacked<FileDeclarationBodyNode> body = opNode::Transform<FileDeclarationBodyNode>(bbn);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

//==========================================
// DialectNamespaces
//==========================================

	inline bool DialectNamespaces::Parse()
	{
		PARSE_START;
		{
			FindDialectNamespaces();
		}
		PARSE_END;
	}

	inline void DialectNamespaces::FindDialectNamespaces()
	{
		LOOP_START(G_DIALECT_NAMESPACE)
		{
			HIT(T_NAMESPACE)
			{
				stackedcontext<DialectNamespaceNode> newNode = opNode::Make<DialectNamespaceNode>(T_NAMESPACE);

				Erase(T_NAMESPACE);

				EatWhitespaceAndComments();

				stacked<TerminalNode> name = opNode::Expect<TerminalNode>(T_ID);

				newNode->SetName(*name);
				newNode->AppendNode(name);

				EatWhitespaceAndComments();

				stacked<BraceBlockNode> body = opNode::Expect<BraceBlockNode>(G_BRACE_BLOCK);

				newNode->SetBody(*body);
				newNode->AppendNode(body);

				InsertNodeAtCurrent(newNode);
			}
		}
		LOOP_END;
	}

} // end namespace interfaces
