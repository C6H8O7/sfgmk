namespace sfgmk
{
	namespace engine
	{
		DebugManager::DebugManager()
		{
			ADD_ENTITY(&m_Selector);
		}

		DebugManager::~DebugManager()
		{
			
		}

		void DebugManager::update(const float& _TimeDelta)
		{
			//Console
			if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::Quote) == sfgmk::engine::KeyStates::KEY_PRESSED )
				m_Console.setActive();

			m_Console.update(_TimeDelta);
		}
	}
}