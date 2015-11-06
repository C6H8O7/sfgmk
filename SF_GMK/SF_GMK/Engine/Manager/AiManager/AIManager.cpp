namespace sfgmk
{
	namespace engine
	{
		AIStateMachine::AIStateMachine(Entity* _Parent, int _InitState) : m_iState(_InitState), m_iNextState(NULL), m_EntityParent(_Parent)
		{
		}

		AIStateMachine::~AIStateMachine()
		{
		}


		void AIStateMachine::process(const float& _TimeDelta)
		{
			//Execute la fonction correspondant à la progression actuelle (enter / update / exit) dans l'état courant (idle / move...)
			m_StatesFunctionsArray[m_iState].CurrentFunc[m_StatesFunctionsArray[m_iState].StateProgress]->Execute();
		}


		bool AIStateMachine::addState(int _StateId, FoncterTemplate* _NewFunction)
		{
			//Alloue de l'espace dans le tableau si nécessaire
			if( m_StatesFunctionsArray.getSize() < _StateId + 1 )
				m_StatesFunctionsArray.doubleSize();

			stAIStateFunctions* NewState = new stAIStateFunctions;
			NewState->StateProgress = eEnter;
			memcpy(NewState->CurrentFunc, _NewFunction, sizeof(FoncterTemplate));

			m_StatesFunctionsArray.pushBack(*NewState);

			return true;
		}
	}
}