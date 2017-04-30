namespace interfaces {
///==========================================
/// ModifierSupport interface
///==========================================

template <class Parent>
inline void ModifierSupport<Parent>::Init() {
    InitModifierSupport();
}

// looks at auto and specified modifiers
template <class Parent>
inline bool ModifierSupport<Parent>::HasModifier(const opString& modifiername) {
    if (TerminalNode* node = FetchBasicModifier(modifiername)) {
        return true;
    }

    if (modifiers) {
        if (modifiers->HasModifier(modifiername)) return true;
    }

    if (GetVisibility(modifiername)) return true;

    // TODO: should we maybe add these modifiers to this node?

    // handle parent modifiers...
    if (!node_cast<OPObjectNode>(this)) {
        if (OPObjectNode* p = this->template FindParent<OPObjectNode>()) {
            if (p->HasModifier(modifiername)) return true;
        }
    }

    return false;
}

// only looks at special modifiers
template <class Parent>
inline bool ModifierSupport<Parent>::HasModifier(Token modifiertoken) {
    if (modifiers) {
        if (modifiers->HasModifier(modifiertoken)) return true;
    }

    // this handles visibility...
    if (automodifiers) {
        if (automodifiers->HasModifier(modifiertoken)) return true;
    }

    // handle parent modifiers...
    if (!node_cast<OPObjectNode>(this)) {
        if (OPObjectNode* p = this->template FindParent<OPObjectNode>()) {
            if (p->HasModifier(modifiertoken)) return true;
        }
    }

    return false;
}

template <class Parent>
inline opNode* ModifierSupport<Parent>::GetVisibility(const opString& name) {
    return NULL;
}

// calls to GetValuedModifier will look at current modifiers, but also
// attempt to fetch automatically generated modifiers
template <class Parent>
inline ValuedModifierNode* ModifierSupport<Parent>::GetValuedModifier(
    const opString& modifiername) {
    if (ValuedModifierNode* node = FetchValueModifier(modifiername)) {
        return node;
    }

    if (modifiers) {
        if (ValuedModifierNode* node =
                modifiers->GetValuedModifier(modifiername))
            return node;
    }

    // handle parent modifiers...
    if (!node_cast<OPObjectNode>(this)) {
        if (OPObjectNode* p = this->template FindParent<OPObjectNode>()) {
            return p->GetValuedModifier(modifiername);
        }
    }

    return NULL;
}

template <class Parent>
inline opNode* ModifierSupport<Parent>::FetchModifier(const opString& name) {
    return ModifierSupportBase::FetchModifier(name);
}

template <class Parent>
inline void ModifierSupport<Parent>::FetchAllModifiers() {
    return ModifierSupportBase::FetchAllModifiers();
}

template <class Parent>
inline void ModifierSupport<Parent>::CreateModifiersNode() {
    if (automodifiers) return;

    stacked<AutoModifiersNode> modnode = NEWNODE(AutoModifiersNode);

    modnode->CopyBasics(this);

    automodifiers = *modnode;

    this->AppendNode(modnode);
}

///==========================================
/// ConditionalSupport interface
///==========================================

template <class Parent>
inline void ConditionalSupport<Parent>::Init() {
    Condition = NULL;
}

// returns how many levels printed
template <class Parent>
inline int ConditionalSupport<Parent>::PrintConditions(
    opSectionStream& section) {
    if (Condition) return Condition->PrintCondition(section);
    return 0;
}

template <class Parent>
inline void ConditionalSupport<Parent>::PrintConditionEnd(
    int number, opSectionStream& stream) {
    if (!number) return;

    stream << endl;

    for (int i = 0; i < number; i++) stream << "#endif" << endl;
}

template <class Parent>
inline void ConditionalSupport<Parent>::SetCondition(
    PreprocessorStatementNode* condition) {
    Condition = condition;
}

}  // end of namespace interfaces
