///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: DialectStatementInterfaces.h
/// Date: 06/04/2007
///
/// Description:
///
/// Contains interfaces for dialect statements.
///****************************************************************

namespace interfaces
{

///==========================================
/// DialectStatementsBase
///==========================================

	// This method checks for a statement + modifiers based on the template args.
	template<class Parent> 
	template<Token grammar, class StatementNodeClass, class NodeClass>
    inline bool DialectStatementsBase<Parent>::HandleStatement(stacked<opNode>& stuff)
	{
		if (this->IsCurrent(grammar))
		{
			stackedcontext<StatementNodeClass> statement = opNode::Make<StatementNodeClass>(grammar);
			stacked<NodeClass>                 node      = opNode::Expect<NodeClass>(grammar);

			if (!stuff->IsEmpty())
			{
				stacked<DialectModifiersNode> modifiers = opNode::Transform<DialectModifiersNode>(stuff);

				statement->SetModifiers(*modifiers);
				statement->AppendNode(modifiers);
			}
			else
				stuff.Delete();

			statement->SetStatement(*node);
			statement->AppendNode(node);

			Statements.push_back(*statement);
			this->InsertNodeAtCurrent(statement);

			return true;
		}

		return false;
	}

	// Is the current statement a datamodifier statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::DataModifierStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_DATAMODIFIER, DataModifierStatementNode, DataModifierNode>(stuff);
	}

	// Is the current statement a functionmodifier statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::FunctionModifierStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_FUNCTIONMODIFIER, FunctionModifierStatementNode, FunctionModifierNode>(stuff);
	}

	// Is the current statement a category location statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::CategoryLocationStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_CATEGORY_LOCATION, CategoryLocationStatementNode, CategoryLocationNode>(stuff);
	}

	// Is the current statement a note statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::NoteStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_NOTE, NoteStatementNode, NoteNode>(stuff);
	}

	// Is the current statement a datamap statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::DatamapStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_CATEGORY_DATAMAP, CategoryDatamapStatementNode, CategoryDatamapNode>(stuff);
	}

	// Is the current statement a functionmap statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::FunctionmapStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_CATEGORY_FUNCTIONMAP, CategoryFunctionmapStatementNode, CategoryFunctionmapNode>(stuff);
	}

	// Is the current statement a disallow statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::DisallowStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_DISALLOW, DisallowStatementNode, DisallowNode>(stuff);
	}

	// Is the current statement an is statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::IsStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_IS, IsStatementNode, IsNode>(stuff);
	}

	// Is the current statement an enum location statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::EnumLocationStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_ENUMERATION_LOCATION, EnumerationLocationStatementNode, EnumerationLocationNode>(stuff);
	}

	// Is the current statement an enummap statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::EnumMapStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_ENUMERATION_MAP, EnumerationMapStatementNode, EnumerationMapNode>(stuff);
	}

	// Is the current statement a file declaration location statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::FileDeclarationLocationStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_FILE_DECLARATION_LOCATION, FileDeclarationLocationStatementNode, FileDeclarationLocationNode>(stuff);
	}

	// Is the current statement a note definition statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::NoteDefinitionStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_NOTE_DEFINITION, NoteDefinitionStatementNode, NoteDefinitionNode>(stuff);
	}

	// Is the current statement an opmacro statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::OPMacroStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_OPMACRO, DialectOPMacroStatementNode, OPMacroNode>(stuff);
	}

	// Is the current statement a category statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::CategoryStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_CATEGORY, CategoryStatementNode, CategoryNode>(stuff);
	}

	// Is the current statement a dialect namespace statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::DialectNamespaceStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_DIALECT_NAMESPACE, DialectNamespaceStatementNode, DialectNamespaceNode>(stuff);
	}

	// Is the current statement an opinclude statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::OPIncludeStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_OPINCLUDE, DialectOPIncludeStatementNode, OPIncludeNode>(stuff);
	}

	// Is the current statement a code statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::CodeStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_CODE, CodeStatementNode, CodeNode>(stuff);
	}

	// Is the current statement an enumeration statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::EnumerationStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_ENUMERATION, EnumerationStatementNode, EnumerationNode>(stuff);
	}

	// Is the current statement a file declaration statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::FileDeclarationStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_FILE_DECLARATION, FileDeclarationStatementNode, FileDeclarationNode>(stuff);
	}

	// Is the current statement an extension statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::ExtensionStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_EXTENSION, ExtensionStatementNode, ExtensionNode>(stuff);
	}

	// Is the current statement an extend point statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::ExtendPointStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_EXTEND_POINT, ExtendPointStatementNode, ExtendPointNode>(stuff);
	}

	// Is the current statement an opdefine statement?
	template<class Parent> inline bool DialectStatementsBase<Parent>::OPDefineStatement(stacked<opNode>& stuff)
	{
		return HandleStatement<G_OPDEFINE, DialectOPDefineStatementNode, OPDefineNode>(stuff);
	}

///==========================================
/// DialectCategoryStatements
///==========================================

	template<class Parent> inline bool DialectCategoryStatements<Parent>::Parse()
	{
		PARSE_START;
		{
			FindDialectCategoryStatements();
		}
		PARSE_END;
	}

	template<class Parent> inline bool DialectCategoryStatements<Parent>::PostParse()
	{
		POSTPARSE_START;
		{
			this->AllowOnlyDialectCategoryStatements();
		}
		POSTPARSE_END;
	}

	// Finds dialect category statements.
	template<class Parent> inline void DialectCategoryStatements<Parent>::FindDialectCategoryStatements()
	{
		bool bFinished = false;

		while (!bFinished)
		{
			stacked<opNode> stuff = opNode::PushUntilOr<opNode>(T_SEMICOLON,
																G_DATAMODIFIER,
																G_FUNCTIONMODIFIER,
																G_CATEGORY_LOCATION,
																G_DISALLOW,
																false);

			bFinished = !(*stuff);

			if (bFinished)
				break;

			if      (this->DataModifierStatement(stuff));
			else if (this->FunctionModifierStatement(stuff));
			else if (this->CategoryLocationStatement(stuff));
			else if (this->DisallowStatement(stuff));
			else if (this->IsCurrent(T_SEMICOLON))
			{
				this->Erase(T_SEMICOLON);
				this->CollapseNodeAtCurrent(stuff);
			}
			else
			{
				this->CollapseNodeAtCurrent(stuff);

				bFinished = true;
			}
		}
	}

	// Makes sure that only valid dialect category statements
	// remain after statement parsing.
	template<class Parent> inline void DialectCategoryStatements<Parent>::AllowOnlyDialectCategoryStatements()
	{
		this->AllowOnly( G_DATAMODIFIER_STATEMENT,
				   G_FUNCTIONMODIFIER_STATEMENT,
				   G_CATEGORY_LOCATION_STATEMENT,
				   G_DISALLOW_STATEMENT );
	}

///==========================================
/// DialectEnumerationStatements
///==========================================

	template<class Parent> inline bool DialectEnumerationStatements<Parent>::Parse()
	{
		PARSE_START;
		{
			FindDialectEnumStatements();
		}
		PARSE_END;
	}

	template<class Parent> inline bool DialectEnumerationStatements<Parent>::PostParse()
	{
		POSTPARSE_START;
		{
			this->AllowOnlyDialectEnumStatements();
		}
		POSTPARSE_END;
	}

	// Finds dialect category statements.
	template<class Parent> inline void DialectEnumerationStatements<Parent>::FindDialectEnumStatements()
	{
		bool bFinished = false;

		while (!bFinished)
		{
			stacked<opNode> stuff = opNode::PushUntilOr<opNode>(T_SEMICOLON,
																G_ENUMERATION_LOCATION,
																G_DISALLOW,
																false);

			bFinished = !(*stuff);

			if (bFinished)
				break;

			if      (this->EnumLocationStatement(stuff));
			else if (this->DisallowStatement(stuff));
			else if (this->IsCurrent(T_SEMICOLON))
			{
				this->Erase(T_SEMICOLON);
				this->CollapseNodeAtCurrent(stuff);
			}
			else
			{
				this->CollapseNodeAtCurrent(stuff);

				bFinished = true;
			}
		}
	}

	// Makes sure that only valid dialect category statements
	// remain after statement parsing.
	template<class Parent> inline void DialectEnumerationStatements<Parent>::AllowOnlyDialectEnumStatements()
	{
		this->AllowOnly( G_ENUMERATION_LOCATION_STATEMENT,
			       G_DISALLOW_STATEMENT );
	}

///==========================================
/// EnumerationLocationStatements
///==========================================

	template<class Parent> inline bool EnumerationLocationStatements<Parent>::Parse()
	{
		PARSE_START;
		{
			FindEnumerationLocationStatements();
		}
		PARSE_END;
	}

	template<class Parent> inline bool EnumerationLocationStatements<Parent>::PostParse()
	{
		POSTPARSE_START;
		{
			this->AllowOnlyEnumerationLocationStatements();
		}
		POSTPARSE_END;
	}

	// Finds dialect category statements.
	template<class Parent> inline void EnumerationLocationStatements<Parent>::FindEnumerationLocationStatements()
	{
		bool bFinished = false;

		while (!bFinished)
		{
			stacked<opNode> stuff = opNode::PushUntilOr<opNode>(T_SEMICOLON,
																G_ENUMERATION_MAP,
																G_NOTE,
																false);

			bFinished = !(*stuff);

			if (bFinished)
				break;

			if      (this->EnumMapStatement(stuff));
			else if (this->NoteStatement(stuff));
			else if (this->IsCurrent(T_SEMICOLON))
			{
				this->Erase(T_SEMICOLON);
				this->CollapseNodeAtCurrent(stuff);
			}
			else
			{
				this->CollapseNodeAtCurrent(stuff);

				bFinished = true;
			}
		}
	}

	// Makes sure that only valid dialect category statements
	// remain after statement parsing.
	template<class Parent> inline void EnumerationLocationStatements<Parent>::AllowOnlyEnumerationLocationStatements()
	{
		this->AllowOnly( G_ENUMERATIONMAP_STATEMENT,
			       G_NOTE_STATEMENT );
	}

///==========================================
/// CategoryLocationStatements
///==========================================

	template<class Parent> inline bool CategoryLocationStatements<Parent>::Parse()
	{
		PARSE_START;
		{
			FindCategoryLocationStatements();
		}
		PARSE_END;
	}

	template<class Parent> inline bool CategoryLocationStatements<Parent>::PostParse()
	{
		POSTPARSE_START;
		{
			this->AllowOnlyCategoryLocationStatements();
		}
		POSTPARSE_END;
	}

	// Finds dialect location statements.
	template<class Parent> inline void CategoryLocationStatements<Parent>::FindCategoryLocationStatements()
	{
		bool bFinished = false;

		while (!bFinished)
		{
			stacked<opNode> stuff = opNode::PushUntilOr<opNode>(T_SEMICOLON,
																G_NOTE,
																G_CATEGORY_DATAMAP,
																G_CATEGORY_FUNCTIONMAP,
																false);

			bFinished = !(*stuff);

			if (bFinished)
				break;

			if      (this->NoteStatement(stuff));
			else if (this->DatamapStatement(stuff));
			else if (this->FunctionmapStatement(stuff));
			else if (this->IsCurrent(T_SEMICOLON))
			{
				this->Erase(T_SEMICOLON);
				this->CollapseNodeAtCurrent(stuff);
			}
			else
			{
				this->CollapseNodeAtCurrent(stuff);
				
				bFinished = true;
			}
		}
	}

	// Makes sure that only valid dialect location statements
	// remain after statement parsing.
	template<class Parent> inline void CategoryLocationStatements<Parent>::AllowOnlyCategoryLocationStatements()
	{
		this->AllowOnly( G_NOTE_STATEMENT,
				   G_CATEGORY_DATAMAP_STATEMENT,
				   G_CATEGORY_FUNCTIONMAP_STATEMENT );
	}

///==========================================
/// DialectCriteriaStatements
///==========================================

	template<class Parent> inline bool DialectCriteriaStatements<Parent>::Parse()
	{
		PARSE_START;
		{
			this->FindCriteriaStatements();
		}
		PARSE_END;
	}

	template<class Parent> inline bool DialectCriteriaStatements<Parent>::PostParse()
	{
		POSTPARSE_START;
		{
			this->AllowOnlyDialectCriteriaStatements();
		}
		POSTPARSE_END;
	}

	// Finds dialect criteria statements.
	template<class Parent> inline void DialectCriteriaStatements<Parent>::FindCriteriaStatements()
	{
		bool bFinished = false;

		while (!bFinished)
		{
			stacked<opNode> stuff = opNode::PushUntilOr<opNode>(T_SEMICOLON,
																G_IS,
																false);

			bFinished = !(*stuff);

			if (bFinished)
				break;

			if      (this->IsStatement(stuff));
			else if (this->IsCurrent(T_SEMICOLON))
			{
				this->Erase(T_SEMICOLON);
				this->CollapseNodeAtCurrent(stuff);
			}
			else
			{
				this->CollapseNodeAtCurrent(stuff);

				bFinished = true;
			}
		}
	}

	// Makes sure that only valid dialect criteria statements
	// remain after statement parsing.
	template<class Parent> inline void DialectCriteriaStatements<Parent>::AllowOnlyDialectCriteriaStatements()
	{
		this->AllowOnly( G_IS_STATEMENT );
	}

//==========================================
// DialectFileDeclarationLocationStatements
//==========================================

	template<class Parent> inline bool FileDeclarationLocationStatements<Parent>::Parse()
	{
		PARSE_START;
		{
			this->FindFileDeclarationLocationStatements();
		}
		PARSE_END;
	}

	template<class Parent> inline bool FileDeclarationLocationStatements<Parent>::PostParse()
	{
		POSTPARSE_START;
		{
			this->AllowOnlyFileDeclarationLocationStatements();
		}
		POSTPARSE_END;
	}

	// Finds dialect location statements.
	template<class Parent> inline void FileDeclarationLocationStatements<Parent>::FindFileDeclarationLocationStatements()
	{
		bool bFinished = false;

		while (!bFinished)
		{
			stacked<opNode> stuff = opNode::PushUntilOr<opNode>(T_SEMICOLON,
																G_FILE_DECLARATION_LOCATION,
																false);

			bFinished = !(*stuff);

			if (bFinished)
				break;

			if (this->FileDeclarationLocationStatement(stuff));
			else if (this->IsCurrent(T_SEMICOLON))
			{
				this->Erase(T_SEMICOLON);
				this->CollapseNodeAtCurrent(stuff);
			}
			else
			{
				this->CollapseNodeAtCurrent(stuff);

				bFinished = true;
			}
		}
	}

	// Makes sure that only valid dialect location statements
	// remain after statement parsing.
	template<class Parent> inline void FileDeclarationLocationStatements<Parent>::AllowOnlyFileDeclarationLocationStatements()
	{
		this->AllowOnly( G_FILE_DECLARATION_LOCATION_STATEMENT );
	}

///==========================================
/// GlobalDialectStatements
///==========================================

	template<class Parent> inline bool GlobalDialectStatements<Parent>::Parse()
	{
		PARSE_START;
		{
			this->FindGlobalDialectStatements();
		}
		PARSE_END;
	}

	template<class Parent> inline bool GlobalDialectStatements<Parent>::PostParse()
	{
		POSTPARSE_START;
		{
			this->AllowOnlyGlobalDialectStatements();
		}
		POSTPARSE_END;
	}

	// Finds statements in the global dialect context.
	template<class Parent> inline void GlobalDialectStatements<Parent>::FindGlobalDialectStatements()
	{
		bool bFinished = false;

		while (!bFinished)
		{
			stacked<opNode> stuff = opNode::PushUntilOr<opNode>( T_SEMICOLON,
				                                                 G_NOTE_DEFINITION,
																 G_OPMACRO,
																 G_CATEGORY,
																 G_DIALECT_NAMESPACE,
																 G_OPINCLUDE,
																 G_CODE,
																 G_ENUMERATION,
																 G_FILE_DECLARATION,
																 G_EXTENSION,
																 G_EXTEND_POINT,
																 G_OPDEFINE,
																 false);

			bFinished = !(*stuff);

			if (bFinished)
				break;

			if      (this->NoteDefinitionStatement(stuff));
			else if (this->OPMacroStatement(stuff));
			else if (this->CategoryStatement(stuff));
			else if (this->DialectNamespaceStatement(stuff));
			else if (this->OPIncludeStatement(stuff));
			else if (this->CodeStatement(stuff));
			else if (this->EnumerationStatement(stuff));
			else if (this->FileDeclarationStatement(stuff));
			else if (this->ExtensionStatement(stuff));
			else if (this->ExtendPointStatement(stuff));
			else if (this->OPDefineStatement(stuff));
			else if (this->IsCurrent(T_SEMICOLON))
			{
				this->Erase(T_SEMICOLON);
				this->CollapseNodeAtCurrent(stuff);
			}
			else
			{
				this->CollapseNodeAtCurrent(stuff);

				bFinished = true;
			}
		}
	}

	// Makes sure that only valid global dialect statements are allowed.
	template<class Parent> inline void GlobalDialectStatements<Parent>::AllowOnlyGlobalDialectStatements()
	{
		this->AllowOnly( G_NOTE_DEFINITION_STATEMENT,
				   G_DIALECT_OPMACRO_STATEMENT,
				   G_CATEGORY_STATEMENT,
				   G_DIALECT_NAMESPACE_STATEMENT,
				   G_DIALECT_OPINCLUDE_STATEMENT,
				   G_CODE_STATEMENT,
				   G_ENUMERATION_STATEMENT,
				   G_FILE_DECLARATION_STATEMENT,
				   G_EXTENSION_STATEMENT,
				   G_EXTEND_POINT_STATEMENT,
				   G_DIALECT_OPDEFINE_STATEMENT );
	}

} // end namespace interfaces
