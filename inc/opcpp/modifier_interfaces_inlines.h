namespace interfaces
{
    ///==========================================
    /// ModifierSupport interface
    ///==========================================

	inline void ModifierSupport::Init()
	{
		InitModifierSupport();
	}

	// looks at auto and specified modifiers
	inline virtual bool ModifierSupport::HasModifier(const opString& modifiername)
	{
		if(TerminalNode* node = FetchBasicModifier(modifiername))
		{
			return true;
		}
		
		if(modifiers)
		{
			if(modifiers->HasModifier(modifiername))
				return true;
		}
		
		if(GetVisibility(modifiername))
			return true;
		
		//TODO: should we maybe add these modifiers to this node?
		
		//handle parent modifiers...
		if( !node_cast<OPObjectNode>(this) )
		{
			if(OPObjectNode* p = this->FindParent<OPObjectNode>())
			{
				if(p->HasModifier(modifiername))
					return true;
			}
		}
		
		return false;
	}

	// only looks at special modifiers
	inline virtual bool ModifierSupport::HasModifier(Token modifiertoken)
	{
		if(modifiers)
		{
			if(modifiers->HasModifier(modifiertoken))
				return true;
		}
		
		//this handles visibility...
		if(automodifiers)
		{
			if(automodifiers->HasModifier(modifiertoken))
				return true;
		}
		
		//handle parent modifiers...
		if( !node_cast<OPObjectNode>(this) )
		{
			if(OPObjectNode* p = this->FindParent<OPObjectNode>())
			{
				if(p->HasModifier(modifiertoken))
					return true;
			}
		}
		
		return false;
	}

	inline virtual opNode* ModifierSupport::GetVisibility(const opString& name)
	{
		return NULL;
	}

	// calls to GetValuedModifier will look at current modifiers, but also
	// attempt to fetch automatically generated modifiers
	inline virtual ValuedModifierNode* ModifierSupport::GetValuedModifier(const opString& modifiername)
	{
		if(ValuedModifierNode* node = FetchValueModifier(modifiername))
		{
			return node;
		}		
		
		if(modifiers)
		{
			if(ValuedModifierNode* node = modifiers->GetValuedModifier(modifiername))
				return node;
		}

		//handle parent modifiers...
		if( !node_cast<OPObjectNode>(this) )
		{
			if(OPObjectNode* p = this->FindParent<OPObjectNode>())
			{
				return p->GetValuedModifier(modifiername);
			}
		}
		
		return NULL;
	}

	inline opNode* ModifierSupport::FetchModifier(const opString& name)
	{
		return ModifierSupportBase::FetchModifier(name);
	}

	inline void ModifierSupport::FetchAllModifiers()
	{
		return ModifierSupportBase::FetchAllModifiers();
	}

	inline void ModifierSupport::CreateModifiersNode()
	{
		if(automodifiers)
			return;

		stacked<AutoModifiersNode> modnode = NEWNODE(AutoModifiersNode);

		modnode->CopyBasics(this);

		automodifiers = *modnode;

		AppendNode(modnode);
	}

    ///==========================================
    /// ConditionalSupport interface
    ///==========================================

	inline void ConditionalSupport::Init()
	{
		Condition = NULL;
	}
	
	//returns how many levels printed
	inline int ConditionalSupport::PrintConditions(opSectionStream& section)
	{
		if(Condition)
			return Condition->PrintCondition(section);
		return 0;
	}
	
	inline void ConditionalSupport::PrintConditionEnd(int number, opSectionStream& stream)
	{
		if(!number)
			return;
		
		stream << endl;

		for(int i = 0; i < number; i++)
			stream << "#endif" << endl;
	}
	
	inline void ConditionalSupport::SetCondition(PreprocessorStatementNode* condition)
	{
		Condition = condition;
	}

}//end of namespace interfaces

