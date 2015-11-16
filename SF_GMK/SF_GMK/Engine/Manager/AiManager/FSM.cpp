namespace sfgmk
{
	namespace engine
	{
		std::string AIStateMachine::m_StateProgressString[eAIState_Progress_NUMBER] = {"Enter", "Update", "Exit"};

		AIStateMachine::AIStateMachine(Entity* _Parent, int _InitState) : m_iState(_InitState), m_iNextState(0), m_EntityParent(_Parent)
		{
		}

		AIStateMachine::~AIStateMachine()
		{
			m_StatesFunctionsArray.deleteAndClear();
		}


		void AIStateMachine::process(const float& _TimeDelta)
		{
			//Execute la fonction correspondant à la progression actuelle (enter / update / exit) dans l'état courant (idle / move...)
			m_StatesFunctionsArray[m_iState]->CurrentFunc->Execute(m_StatesFunctionsArray[m_iState]->StateProgress);
		}

		void AIStateMachine::progress()
		{
			if( m_StatesFunctionsArray[m_iState]->StateProgress == eEnter )
				m_StatesFunctionsArray[m_iState]->StateProgress = eUpdate;
			else if( m_StatesFunctionsArray[m_iState]->StateProgress == eUpdate )
				m_StatesFunctionsArray[m_iState]->StateProgress = eExit;
			else if( m_StatesFunctionsArray[m_iState]->StateProgress == eExit )
			{
				m_StatesFunctionsArray[m_iState]->StateProgress = eEnter;
				changeState(m_iNextState);
			}
		}

		void AIStateMachine::changeState()
		{
			changeState(m_iNextState);
		}

		void AIStateMachine::changeState(const int& _NextState)
		{
			m_StatesFunctionsArray[m_iState]->StateProgress = eEnter;
			m_iState = _NextState;
		}

		void AIStateMachine::setNextState(const int& _NextState)
		{
			m_iNextState = _NextState;
		}


		bool AIStateMachine::addState(int _StateId, FoncterTemplate* _NewFunction, const std::string& _StateName)
		{
			stAIStateFunctions* NewState = new stAIStateFunctions;
			NewState->StateProgress = eEnter;
			NewState->CurrentFunc = _NewFunction;
			NewState->sStateName = _StateName;
			NewState->sStateName.resize(STATE_NAME_STRING_LENGTH);
		
			SAFE_DELETE(m_StatesFunctionsArray[_StateId]);
			m_StatesFunctionsArray[_StateId] = NewState;

			return true;
		}


		const std::string& AIStateMachine::getCurrentStateName()
		{
			return m_StatesFunctionsArray[m_iState]->sStateName;
		}

		const std::string& AIStateMachine::getCurrentProgressString()
		{
			return m_StateProgressString[m_StatesFunctionsArray[m_iState]->StateProgress];
		}
	}
}