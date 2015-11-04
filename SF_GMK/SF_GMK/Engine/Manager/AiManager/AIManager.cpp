namespace sfgmk
{
	namespace engine
	{
		AIStateMachine::AIStateMachine() : m_iNextState(NULL)
		{
		}

		AIStateMachine::~AIStateMachine()
		{
		}


		void AIStateMachine::update()
		{
			m_StatesFunctionsArray[m_iState].CurrentFunc[m_StatesFunctionsArray[m_iState].iAdvance]->Execute();

			if( m_iNextState )
			{
				m_StatesFunctionsArray[m_iState].CurrentFunc[m_StatesFunctionsArray[m_iState].iAdvance]->Execute();
			}
		}
	}
}