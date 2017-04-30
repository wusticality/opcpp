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

namespace interfaces {

///
/// BasicStatementsBase
///

///==========================================
/// UnknownStatement
///==========================================

// is current an unknown statement? transform stuff
template <class Parent>
inline bool BasicStatementsBase<Parent>::UnknownStatement(
    stacked<opNode>& stuff) {
    if (this->IsCurrent(T_SEMICOLON)) {
        stacked<StatementNode> statement =
            opNode::Transform<StatementNode>(stuff);

        if (statement->IsEmpty()) statement->CopyBasics(this->CurrentNode());

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

// is current an openum statement? transform stuff
template <class Parent>
inline bool BasicStatementsBase<Parent>::OPEnumStatement(
    stacked<opNode>& stuff) {
    if (this->IsCurrent(G_OPENUM)) {
        stackedcontext<OPEnumStatementNode> statement =
            opNode::Make<OPEnumStatementNode>(G_OPENUM);

        stacked<OPEnumNode> openum = opNode::Expect<OPEnumNode>(G_OPENUM);

        if (!stuff->IsEmpty()) {
            stacked<ModifiersNode> modifiers =
                opNode::Transform<ModifiersNode>(stuff);
            statement->SetModifiers(*modifiers);
            statement->AppendNode(modifiers);
        } else
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

// is current an opclass statement? transform stuff
template <class Parent>
inline bool BasicStatementsBase<Parent>::OPObjectStatement(
    stacked<opNode>& stuff) {
    if (this->IsCurrent(G_OPOBJECT)) {
        stackedcontext<OPObjectStatementNode> statement =
            opNode::Make<OPObjectStatementNode>(G_OPOBJECT);

        stacked<OPObjectNode> object = opNode::Expect<OPObjectNode>(G_OPOBJECT);

        if (!stuff->IsEmpty()) {
            stacked<ModifiersNode> modifiers =
                opNode::Transform<ModifiersNode>(stuff);
            statement->SetModifiers(*modifiers);
            statement->AppendNode(modifiers);
        } else
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

// is current an opstate statement? transform stuff
template <class Parent>
inline bool BasicStatementsBase<Parent>::StateStatement(
    stacked<opNode>& stuff) {
    if (this->IsCurrent(G_STATE)) {
        stackedcontext<StateStatementNode> statement =
            opNode::Make<StateStatementNode>(G_STATE);

        stacked<StateNode> state = opNode::Expect<StateNode>(G_STATE);

        if (!stuff->IsEmpty()) {
            stacked<ModifiersNode> modifiers =
                opNode::Transform<ModifiersNode>(stuff);
            statement->SetModifiers(*modifiers);
            statement->AppendNode(modifiers);
        } else
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

// is current a visibility statement? transform stuff
template <class Parent>
inline bool BasicStatementsBase<Parent>::VisibilityStatement(
    stacked<opNode>& stuff) {
    if (this->IsCurrent(G_VISIBILITY_LABEL)) {
        // ok, we need to turn this into a visibility statement
        // we actually need to error if we had something in this... it should be
        // caught by AllowOnly!  looks...ok (would rather collapse stuff here)

        stackedcontext<VisibilityStatementNode> visstatement =
            opNode::Make<VisibilityStatementNode>(G_VISIBILITY_LABEL);

        stacked<VisibilityLabelNode> vislabel =
            opNode::Expect<VisibilityLabelNode>(G_VISIBILITY_LABEL);

        visstatement->SetLabel(*vislabel);
        visstatement->AppendNode(vislabel);

        this->CollapseNodeAtCurrent(stuff);

        this->InsertNodeAtCurrent(visstatement);
        return true;
    }
    return false;
}

// we don't gather a list of visibility statements since they're order dependent

///==========================================
/// FunctionDefinitionStatement
///==========================================

// is current a function definition? transform stuff
template <class Parent>
inline bool BasicStatementsBase<Parent>::FunctionDefinitionStatement(
    stacked<opNode>& stuff) {
    if (this->IsCurrent(G_FUNCTION_DEFINITION)) {
        // ok, we need to turn this into a function statement
        // how?
        stackedcontext<FunctionDefinitionStatementNode> statement =
            opNode::Make<FunctionDefinitionStatementNode>(
                G_FUNCTION_DEFINITION);

        stacked<FunctionDefinitionNode> functiondefinition =
            opNode::Expect<FunctionDefinitionNode>(G_FUNCTION_DEFINITION);

        if (!stuff->IsEmpty()) {
            stacked<ModifiersNode> modifiers =
                opNode::Transform<ModifiersNode>(stuff);
            statement->SetModifiers(*modifiers);
            statement->AppendNode(modifiers);
        } else
            stuff.Delete();

        // need to do something w/ this
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

template <class Parent>
inline bool BasicStatementsBase<Parent>::ConstructorStatement(
    stacked<opNode>& stuff) {
    if (this->IsCurrent(G_CONSTRUCTOR_DEFINITION)) {
        // ok, we need to turn this into a function statement
        // how?
        stackedcontext<ConstructorDefinitionStatementNode> statement =
            opNode::Make<ConstructorDefinitionStatementNode>(
                G_CONSTRUCTOR_DEFINITION);

        stacked<ConstructorDefinitionNode> constructordefinition =
            opNode::Expect<ConstructorDefinitionNode>(G_CONSTRUCTOR_DEFINITION);

        if (!stuff->IsEmpty()) {
            stacked<ModifiersNode> modifiers =
                opNode::Transform<ModifiersNode>(stuff);
            statement->SetModifiers(*modifiers);
            statement->AppendNode(modifiers);
        } else
            stuff.Delete();

        // need to do something w/ this
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

template <class Parent>
inline bool BasicStatementsBase<Parent>::DestructorStatement(
    stacked<opNode>& stuff) {
    if (this->IsCurrent(G_DESTRUCTOR_DEFINITION)) {
        // ok, we need to turn this into a function statement
        // how?
        stackedcontext<DestructorDefinitionStatementNode> statement =
            opNode::Make<DestructorDefinitionStatementNode>(
                G_DESTRUCTOR_DEFINITION);

        stacked<DestructorDefinitionNode> destructordefinition =
            opNode::Expect<DestructorDefinitionNode>(G_DESTRUCTOR_DEFINITION);

        if (!stuff->IsEmpty()) {
            stacked<ModifiersNode> modifiers =
                opNode::Transform<ModifiersNode>(stuff);
            statement->SetModifiers(*modifiers);
            statement->AppendNode(modifiers);
        } else
            stuff.Delete();

        // need to do something w/ this
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

template <class Parent>
inline bool BasicStatementsBase<Parent>::ConditionalPreprocessorStatement(
    stacked<opNode>& stuff) {
    if (this->IsCurrent(G_POUND_ELIF) || this->IsCurrent(G_POUND_ELSE) ||
        this->IsCurrent(G_POUND_ENDIF) || this->IsCurrent(G_POUND_IF) ||
        this->IsCurrent(G_POUND_IFDEF) || this->IsCurrent(G_POUND_IFNDEF)) {
        Token id = this->CurrentNode()->GetId();
        stackedcontext<PreprocessorStatementNode> statement =
            opNode::Make<PreprocessorStatementNode>(id);
        stacked<PreprocessorNode> preprocessor =
            opNode::Expect<PreprocessorNode>(id);

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

template <class Parent>
inline bool BasicStatementsBase<Parent>::CPPConstructStatement(
    stacked<opNode>& stuff) {
    if (this->IsCurrent(G_CLASS) || this->IsCurrent(G_STRUCT) ||
        this->IsCurrent(G_ENUM) || this->IsCurrent(G_UNION)) {
        Token id = this->CurrentNode()->GetId();
        stackedcontext<CPPConstructStatementNode> statement =
            opNode::Make<CPPConstructStatementNode>(id);
        stacked<CPPConstructNode> construct =
            opNode::Expect<CPPConstructNode>(id);

        if (!stuff->IsEmpty()) {
            stacked<ModifiersNode> modifiers =
                opNode::Transform<ModifiersNode>(stuff);
            statement->SetModifiers(*modifiers);
            statement->AppendNode(modifiers);
        } else
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

template <class Parent>
inline bool BasicStatementsBase<Parent>::FriendStatement(
    stacked<opNode>& stuff) {
    if (this->IsCurrent(G_FRIEND)) {
        stacked<FriendStatementNode> statement =
            opNode::Make<FriendStatementNode>(G_FRIEND);
        stacked<FriendNode> thefriend = opNode::Expect<FriendNode>(G_FRIEND);

        statement->SetFriend(*thefriend);
        statement->AppendNode(thefriend);

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

template <class Parent>
inline bool BasicStatementsBase<Parent>::TypedefStatement(
    stacked<opNode>& stuff) {
    if (this->IsCurrent(G_TYPEDEF)) {
        stackedcontext<TypedefStatementNode> statement =
            opNode::Make<TypedefStatementNode>(G_TYPEDEF);
        stacked<TypedefNode> thetypedef =
            opNode::Expect<TypedefNode>(G_TYPEDEF);

        if (!stuff->IsEmpty()) {
            stacked<ModifiersNode> modifiers =
                opNode::Transform<ModifiersNode>(stuff);
            statement->SetModifiers(*modifiers);
            statement->AppendNode(modifiers);
        } else
            stuff.Delete();

        statement->SetTypedef(*thetypedef);
        statement->AppendNode(thetypedef);

        this->InsertNodeAtCurrent(statement);

        return true;
    }

    return false;
}

///==========================================
/// TemplateStatements
///==========================================

template <class Parent>
inline bool BasicStatementsBase<Parent>::TemplateStatement(
    stacked<opNode>& stuff) {
    if (this->IsCurrent(G_TEMPLATED)) {
        stackedcontext<TemplateStatementNode> statement =
            opNode::Make<TemplateStatementNode>(G_TEMPLATED);
        stacked<TemplatedNode> templated =
            opNode::Expect<TemplatedNode>(G_TEMPLATED);

        if (!stuff->IsEmpty()) {
            stacked<ModifiersNode> modifiers =
                opNode::Transform<ModifiersNode>(stuff);

            statement->SetModifiers(*modifiers);
            statement->AppendNode(modifiers);
        } else
            stuff.Delete();

        statement->SetTemplated(*templated);
        statement->AppendNode(templated);

        this->InsertNodeAtCurrent(statement);

        return true;
    }

    return false;
}

///
/// BasicStatements
///

template <class Parent>
inline bool BasicStatements<Parent>::Parse() {
    PARSE_START;
    { FindBasicStatements(); }
    PARSE_END;
}

template <class Parent>
inline bool BasicStatements<Parent>::PostParse() {
    POSTPARSE_START;
    { this->AllowOnlyBasicStatements(); }
    POSTPARSE_END;
}

// Only allow the following statements in opobjects.
template <class Parent>
inline void BasicStatements<Parent>::AllowOnlyBasicStatements() {
    this->AllowOnly(
        G_STATEMENT, G_OPENUM_STATEMENT, G_OPOBJECT_STATEMENT,
        G_FUNCTION_DEFINITION_STATEMENT, G_VISIBILITY_STATEMENT,
        G_CONSTRUCTOR_DEFINITION_STATEMENT, G_DESTRUCTOR_DEFINITION_STATEMENT,
        G_TYPEDEF_STATEMENT, G_FRIEND_STATEMENT, G_PREPROCESSOR_STATEMENT,
        G_CPPCONSTRUCT_STATEMENT, G_CPLUSPLUS, G_TEMPLATE_STATEMENT);
}

template <class Parent>
inline stacked<opNode> BasicStatements<Parent>::PushUntilStatementEnd() {
    return opNode::PushUntilOr<opNode>(
        T_SEMICOLON, G_OPOBJECT, G_OPENUM, G_FUNCTION_DEFINITION,
        G_VISIBILITY_LABEL, G_CONSTRUCTOR_DEFINITION, G_DESTRUCTOR_DEFINITION,
        G_TEMPLATED, G_TYPEDEF, G_FRIEND, G_CLASS, G_STRUCT, G_ENUM, G_UNION,
        G_POUND_ELIF, G_POUND_ELSE, G_POUND_ENDIF, G_POUND_IF, G_POUND_IFDEF,
        G_POUND_IFNDEF, false);
}

// Find all statements except preprocessor ones.
template <class Parent>
inline void BasicStatements<Parent>::FindBasicStatements() {
    bool bFinished = false;

    while (!bFinished) {
        // if ( TemplateStatement() )
        //	continue;

        stacked<opNode> stuff = PushUntilStatementEnd();

        bFinished = !(*stuff);

        if (bFinished) break;

        if (this->FunctionDefinitionStatement(stuff))
            ;
        else if (this->VisibilityStatement(stuff))
            ;
        else if (this->OPObjectStatement(stuff))
            ;
        else if (this->OPEnumStatement(stuff))
            ;
        else if (this->ConstructorStatement(stuff))
            ;
        else if (this->DestructorStatement(stuff))
            ;
        else if (this->TemplateStatement(stuff))
            ;
        else if (this->TypedefStatement(stuff))
            ;
        else if (this->FriendStatement(stuff))
            ;
        else if (this->CPPConstructStatement(stuff))
            ;
        else if (this->ConditionalPreprocessorStatement(stuff))
            ;
        else if (this->UnknownStatement(stuff))
            ;
        else {
            this->CollapseNodeAtCurrent(stuff);

            // I think this function should never error.
            bFinished = true;
        }
    }
}

///
/// BasicStateStatements
///

// TODO: split up the utility function interface
//		so we don't have all these extra gathering vectors.

template <class Parent>
inline void BasicStateStatements<Parent>::FindStateStatements() {
    // for now we only look for function definitions
    // later we can add a new statement type (G_STATEMENT_FUNCTIONSONLY)
    bool bfinished = false;
    while (!bfinished) {
        stacked<opNode> stuff = opNode::PushUntilOr<opNode>(
            G_FUNCTION_DEFINITION, G_STATE, T_SEMICOLON, false);

        bfinished = !(*stuff);
        if (bfinished) break;

        // handle null statements...
        if (this->IsCurrent(T_SEMICOLON)) {
            this->Erase(T_SEMICOLON);
            this->CollapseNodeAtCurrent(stuff);
        } else if (this->FunctionDefinitionStatement(stuff))
            ;
        else if (this->StateStatement(stuff))
            ;
        else {
            this->CollapseNodeAtCurrent(stuff);

            // I think this function should never error.
            bfinished = true;
        }
    }
}

template <class Parent>
inline void BasicStateStatements<Parent>::AllowOnlyStateStatements() {
    this->AllowOnly(G_FUNCTION_DEFINITION_STATEMENT, G_STATE_STATEMENT);
}

///==========================================
/// UsingStatements
///==========================================

template <class Parent>
inline bool UsingStatements<Parent>::Parse() {
    PARSE_START;
    { FindUsingStatements(); }
    PARSE_END;
}

template <class Parent>
inline bool UsingStatements<Parent>::FindUsingStatements() {
    INSPECT_START(G_USING_STATEMENT);
    {
        if (this->PeekStart(G_USING)) {
            stacked<UsingStatementNode> newNode =
                opNode::PushUntilEnd<UsingStatementNode>();

            this->SetInnerStatement(*newNode);
            this->AppendNode(newNode);
        }
    }
    INSPECT_END;
}

///
/// NullStatements
///

template <class Parent>
inline bool NullStatements<Parent>::Parse() {
    PARSE_START;
    { FindNullStatements(); }
    PARSE_END;
}

template <class Parent>
inline bool NullStatements<Parent>::FindNullStatements() {
    INSPECT_START(G_NULL_STATEMENT);
    {
        if (this->IsEmpty()) {
            stacked<NullStatementNode> newNode = NEWNODE(NullStatementNode());

            newNode->CopyBasics(this);

            this->SetInnerStatement(*newNode);
            this->AppendNode(newNode);

            return true;
        }
    }
    INSPECT_END;
}

///
/// FuncPointerStatements
///

template <class Parent>
inline bool FuncPointerStatements<Parent>::Parse() {
    PARSE_START;
    { FindFuncPointerStatements(); }
    PARSE_END;
}

template <class Parent>
inline bool FuncPointerStatements<Parent>::FindFuncPointerStatements() {
    INSPECT_START(G_FUNCTION_POINTER_STATEMENT);
    {
        if (this->PeekEnd(G_FUNCTION_POINTER)) {
            stackedcontext<FuncPointerStatementNode> newNode =
                NEWNODE(FuncPointerStatementNode());
            newNode->CopyBasics(this);

            stacked<ModifiersNode> modifiers =
                opNode::PushUntil<ModifiersNode>(G_FUNCTION_POINTER);
            stacked<FunctionPointerNode> fpn =
                opNode::Expect<FunctionPointerNode>(G_FUNCTION_POINTER);

            if (!modifiers->IsEmpty()) {
                modifiers->CopyBasics(*fpn);
                newNode->SetModifiers(*modifiers);
                newNode->AppendNode(modifiers);
            } else
                modifiers.Delete();

            newNode->SetFunctionPointer(*fpn);
            newNode->AppendNode(fpn);

            this->SetInnerStatement(*newNode);
            this->AppendNode(newNode);

            return true;
        }
    }
    INSPECT_END;
}

///
/// FuncPrototypeStatements
///

template <class Parent>
inline bool FuncPrototypeStatements<Parent>::Parse() {
    PARSE_START;
    { FindFuncPrototypeStatements(); }
    PARSE_END;
}

template <class Parent>
inline bool FuncPrototypeStatements<Parent>::FindFuncPrototypeStatements() {
    INSPECT_START(G_FUNCTION_PROTOTYPE_STATEMENT);
    {
        if (this->PeekEnd(G_FUNCTION_PROTOTYPE)) {
            stackedcontext<FuncPrototypeStatementNode> newNode =
                NEWNODE(FuncPrototypeStatementNode());
            newNode->CopyBasics(this);

            stacked<ModifiersNode> modifiers =
                opNode::PushUntil<ModifiersNode>(G_FUNCTION_PROTOTYPE);
            stacked<FunctionPrototypeNode> fpn =
                opNode::Expect<FunctionPrototypeNode>(G_FUNCTION_PROTOTYPE);

            if (!modifiers->IsEmpty()) {
                modifiers->CopyBasics(*fpn);
                newNode->SetModifiers(*modifiers);
                newNode->AppendNode(modifiers);
            } else
                modifiers.Delete();

            newNode->SetFunctionPrototype(*fpn);
            newNode->AppendNode(fpn);

            this->SetInnerStatement(*newNode);
            this->AppendNode(newNode);

            return true;
        }
    }
    INSPECT_END;
}

///
/// ConstructorPrototypeStatements
///

template <class Parent>
inline bool
ConstructorPrototypeStatements<Parent>::FindConstructorPrototypeStatements() {
    INSPECT_START(G_CONSTRUCTOR_PROTOTYPE_STATEMENT);
    {
        if (this->PeekEnd(G_CONSTRUCTOR_PROTOTYPE)) {
            stackedcontext<ConstructorPrototypeStatementNode> newNode =
                NEWNODE(ConstructorPrototypeStatementNode());

            newNode->CopyBasics(this);

            stacked<ModifiersNode> modifiers =
                opNode::PushUntil<ModifiersNode>(G_CONSTRUCTOR_PROTOTYPE);
            stacked<ConstructorPrototypeNode> cpn =
                opNode::Expect<ConstructorPrototypeNode>(
                    G_CONSTRUCTOR_PROTOTYPE);

            if (!modifiers->IsEmpty()) {
                modifiers->CopyBasics(*cpn);
                newNode->SetModifiers(*modifiers);
                newNode->AppendNode(modifiers);
            } else
                modifiers.Delete();

            newNode->SetConstructorPrototype(*cpn);
            newNode->AppendNode(cpn);

            this->SetInnerStatement(*newNode);
            this->AppendNode(newNode);

            return true;
        }
    }
    INSPECT_END;
}

///
/// DestructorPrototypeStatements
///

template <class Parent>
inline bool
DestructorPrototypeStatements<Parent>::FindDestructorPrototypeStatements() {
    INSPECT_START(G_DESTRUCTOR_PROTOTYPE_STATEMENT);
    {
        if (this->PeekEnd(G_DESTRUCTOR_PROTOTYPE)) {
            stackedcontext<DestructorPrototypeStatementNode> newNode =
                NEWNODE(DestructorPrototypeStatementNode());

            newNode->CopyBasics(this);

            stacked<ModifiersNode> modifiers =
                opNode::PushUntil<ModifiersNode>(G_DESTRUCTOR_PROTOTYPE);
            stacked<DestructorPrototypeNode> cpn =
                opNode::Expect<DestructorPrototypeNode>(G_DESTRUCTOR_PROTOTYPE);

            if (!modifiers->IsEmpty()) {
                modifiers->CopyBasics(*cpn);
                newNode->SetModifiers(*modifiers);
                newNode->AppendNode(modifiers);
            } else
                modifiers.Delete();

            newNode->SetDestructorPrototype(*cpn);
            newNode->AppendNode(cpn);

            this->SetInnerStatement(*newNode);
            this->AppendNode(newNode);

            return true;
        }
    }
    INSPECT_END;
}

///
/// DataStatements
///

template <class Parent>
inline bool DataStatements<Parent>::Parse() {
    PARSE_START;
    { FindDataStatements(); }
    PARSE_END;
}

template <class Parent>
inline bool DataStatements<Parent>::FindDataStatements() {
    INSPECT_START(G_DATA_STATEMENT);
    { FindDataStatementOnly(); }
    INSPECT_END;
}

template <class Parent>
inline bool DataStatements<Parent>::FindDataStatementOnly() {
    if (!this->HasNumChildren(2)) return false;

    stackedcontext<DataStatementNode> newNode = NEWNODE(DataStatementNode());
    newNode->CopyBasics(this->FirstChild());

    stacked<DataDeclarationNode> declaration =
        opNode::PushUntil<DataDeclarationNode>(T_ASSIGN, false);

    newNode->SetDeclaration(*declaration);
    newNode->AppendNode(declaration);

    if (this->IsCurrent(T_ASSIGN)) {
        this->Erase(T_ASSIGN);
        stacked<DataInitializationNode> initialization =
            opNode::PushUntilEnd<DataInitializationNode>();

        newNode->SetInitialization(*initialization);
        newNode->AppendNode(initialization);
    }

    this->SetInnerStatement(*newNode);
    this->AppendNode(newNode);

    return true;
}

template <class Parent>
inline bool DataStatementOnly<Parent>::Parse() {
    PARSE_START;
    {
        if (!this->FindDataStatementOnly()) {
            opError::MessageError(this,
                                  "Unnamed data declarations not allowed.");
            // GenericError("unnamed data declarations not allowed");
        }
    }
    PARSE_END;
}

///==========================================
/// ConditionalPreprocessorStatements
///==========================================

template <class Parent>
inline bool ConditionalPreprocessorStatements<Parent>::Parse() {
    PARSE_START;
    { FindConditionalPreprocessorStatements(); }
    PARSE_END;
}

template <class Parent>
inline void ConditionalPreprocessorStatements<
    Parent>::FindConditionalPreprocessorStatements() {
    FindConditionalPreprocessorStatement<G_POUND_ELIF>();
    FindConditionalPreprocessorStatement<G_POUND_ELSE>();
    FindConditionalPreprocessorStatement<G_POUND_ENDIF>();
    FindConditionalPreprocessorStatement<G_POUND_IF>();
    FindConditionalPreprocessorStatement<G_POUND_IFDEF>();
    FindConditionalPreprocessorStatement<G_POUND_IFNDEF>();
}

template <class Parent>
template <Token hittoken>
inline void ConditionalPreprocessorStatements<
    Parent>::FindConditionalPreprocessorStatement() {
    LOOP_START(G_PREPROCESSOR_STATEMENT);
    {
        HIT(hittoken) {
            stackedcontext<PreprocessorStatementNode> newNode =
                opNode::Make<PreprocessorStatementNode>(hittoken);

            stacked<PreprocessorNode> preprocessor =
                opNode::Expect<PreprocessorNode>(hittoken);

            newNode->SetPreprocessor(*preprocessor);
            newNode->AppendNode(preprocessor);

            this->InsertNodeAtCurrent(newNode);
        }
    }
    LOOP_END;
}

}  // end namespace interfaces
