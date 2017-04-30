namespace interfaces {

///==========================================
/// ModifierSupport interface
///==========================================

template <class Parent>
class ModifierSupport : public Parent, public ModifierSupportBase {
   public:
    IMPLEMENTS_INTERFACE(ModifierSupport);

    void Init();

    // looks at auto and specified modifiers
    virtual bool HasModifier(const opString& modifiername);

    // only looks at special modifiers
    virtual bool HasModifier(Token modifiertoken);
    virtual opNode* GetVisibility(const opString& name);

    // calls to GetValuedModifier will look at current modifiers, but also
    // attempt to fetch automatically generated modifiers
    virtual ValuedModifierNode* GetValuedModifier(const opString& modifiername);
    opNode* FetchModifier(const opString& name);
    void FetchAllModifiers();

   protected:
    void CreateModifiersNode();
};

///==========================================
/// ConditionalSupport interface
///==========================================

template <class Parent>
class ConditionalSupport : public Parent {
   public:
    IMPLEMENTS_INTERFACE(ConditionalSupport);

    void Init();

    // returns how many levels printed
    int PrintConditions(opSectionStream& section);
    void PrintConditionEnd(int number, opSectionStream& stream);
    void SetCondition(PreprocessorStatementNode* condition);

   private:
    PreprocessorStatementNode* Condition;
};

}  // end of namespace interfaces
