namespace sfgmk
{
	Mouse::Mouse() : m_iWheelState(0)
	{
		for( int i(0); i < sf::Mouse::ButtonCount; i++ )
			m_KeyStates[i] = KEY_UP;
	}

	Mouse::~Mouse()
	{
	}


	void Mouse::updateMouse()
	{
		for( int i(0); i < sf::Mouse::ButtonCount; i++ )
		{
			if( m_KeyStates[i] == KEY_PRESSED )
				m_KeyStates[i] = KEY_DOWN;
			else if( m_KeyStates[i] == KEY_RELEASED )
				m_KeyStates[i] = KEY_UP;
		}

		m_iWheelState = 0;
	}

	void Mouse::handleEvent(sf::Event _Event)
	{
		int key;

		switch( _Event.type )
		{
			case sf::Event::MouseButtonPressed:
				key = _Event.key.code;
				if( m_KeyStates[key] == sfgmk::KeyStates::KEY_UP )
					m_KeyStates[key] = sfgmk::KeyStates::KEY_PRESSED;
				break;

			case sf::Event::MouseButtonReleased:
				key = _Event.key.code;
				m_KeyStates[key] = sfgmk::KeyStates::KEY_RELEASED;
				break;

			case sf::Event::MouseWheelMoved:
				m_iWheelState = _Event.key.code;

			default:
				break;
		}
	}


	sf::Vector2i Mouse::getWindowPosition()
	{
		return sf::Mouse::getPosition(*GAME_MANAGER->getRenderWindow());
	}

	sf::Vector2f Mouse::getWorldPosition()
	{
		sf::Vector2f ViewPosition(GAME_MANAGER->getCurrentCamera()->getCenter());
		sf::Vector2f ViewSize(GAME_MANAGER->getCurrentCamera()->getSize());

		return sf::Vector2f(getWindowPosition()) * GAME_MANAGER->getCurrentCamera()->getZoomFactor() + ViewPosition - (ViewSize * 0.5f);
	}


	int Mouse::getButtonState(sf::Mouse::Button _Button)
	{
		return m_KeyStates[_Button];
	}

	int Mouse::getWheelState()
	{
		return m_iWheelState;
	}
}