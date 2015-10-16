namespace sfgmk
{
	StateMachine::StateMachine() : m_iStateCreated(0), m_iStateToLoad(0)
	{
	}

	StateMachine::~StateMachine()
	{
	}


	State *StateMachine::CreateState(int _id)
	{
		StateMachine::StateCreationConv conv;
		conv._voidptr = StateBank::getSingleton()->m_States[_id].first;

		m_iStateCreated = _id;

		return conv._statefunc();
	}


	void StateMachine::init(int _id)
	{
		m_CurrentState = CreateState(_id);
		m_CurrentState->init();
	}

	void StateMachine::update()
	{
		m_CurrentState->update();
	}

	bool StateMachine::changeState(int _id, int _StateToLoadId)
	{
		//Id du prochain state si on est en train d'utiliser un state loading
		m_iStateToLoad = _StateToLoadId;
		
		State* CurrentState = m_CurrentState;
		StateOptions& Options = CurrentState->getOptions();
		bool bDeinit(false);

		if( !Options.isContinuing )
		{
			if( Options.goToParent )
			{
				if( !CurrentState->getParent() )
					return false;

				m_CurrentState = CurrentState->getParent();
				m_CurrentState->getChild() = 0;
			}
		}

		bool bGotoParent(false);
		if( !(bGotoParent = Options.goToParent) )
		{
			State *newState = CreateState(_id);
			m_CurrentState = newState;
			
			if( Options.getNewChild )
			{
				CurrentState->getChild() = newState;
				newState->getParent() = CurrentState;
			}
			else if( Options.isContinuing )
			{
				if( CurrentState->getParent() )
					newState->getParent() = CurrentState->getParent();
			}
		}

		if( !Options.isContinuing && !Options.deinitAfter )
		{
			bDeinit = true;
			CurrentState->deinit();
			delete CurrentState;
		}

		if( !bGotoParent )
			m_CurrentState->init();

		if( !Options.isContinuing && !bDeinit )
		{
			bDeinit = true;
			CurrentState->deinit();
			delete CurrentState;
		}
	
		return true;
	}


	void StateMachine::draw()
	{
		State *state = m_CurrentState;

		while( state->getParent() )
			state = state->getParent();

		while( state )
		{
			state->draw();
			state = state->getChild();
		}
	}


	State* StateMachine::Get_CurrentState()
	{
		return m_CurrentState;
	}


	const int& StateMachine::getStateCreatedId()
	{
		return m_iStateCreated;
	}

	const int& StateMachine::getStateToLoadId()
	{
		return m_iStateToLoad;
	}
};