namespace sfgmk
{
	namespace engine
	{
		StateMachineManager::StateMachineManager() : m_StateMachine(0)
		{
			m_StateMachine = new StateMachine();
		}

		StateMachineManager::~StateMachineManager()
		{
			SAFE_DELETE(m_StateMachine);
		}

		void StateMachineManager::update()
		{
			return m_StateMachine->update();
		}

		void StateMachineManager::draw()
		{
			return m_StateMachine->draw();
		}

		StateMachine* StateMachineManager::getStateMachine()
		{
			return m_StateMachine;
		}
	}
}