namespace sfgmk
{
	namespace engine
	{
		DebugManager::DebugManager()
		{
			Selector *selector = new Selector();
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