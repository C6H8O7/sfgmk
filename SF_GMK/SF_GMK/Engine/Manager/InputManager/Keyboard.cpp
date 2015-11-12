namespace sfgmk
{
	namespace engine
	{
		Keyboard::Keyboard()
		{
			for( int i = 0; i < sf::Keyboard::KeyCount; i++ )
				m_KeyStates[i] = KEY_UP;
		}

		Keyboard::~Keyboard()
		{
		}


		void Keyboard::updateKeyboard()
		{
			for( int i = 0; i < sf::Keyboard::KeyCount; i++ )
			{
				if( m_KeyStates[i] == KEY_PRESSED )
					m_KeyStates[i] = KEY_DOWN;
				else if( m_KeyStates[i] == KEY_RELEASED )
					m_KeyStates[i] = KEY_UP;
			}

			m_LastChar = 0;
		}

		void Keyboard::handleEvent(sf::Event _event)
		{
			int key;

			switch( _event.type )
			{
				case sf::Event::TextEntered:
					key = _event.text.unicode;
					if( key >= 0x20 && key <= 0x7F )
						wcstombs(&m_LastChar, (const wchar_t*)&key, 1);
					break;

				case sf::Event::KeyPressed:
					key = _event.key.code;
					if( m_KeyStates[key] == KEY_UP )
						m_KeyStates[key] =KEY_PRESSED;
					break;

				case sf::Event::KeyReleased:
					key = _event.key.code;
					m_KeyStates[key] = KEY_RELEASED;
					break;
			}
		}


		char Keyboard::getLastChar()
		{
			return m_LastChar;
		}

		int Keyboard::getKeyState(sf::Keyboard::Key _key)
		{
			return m_KeyStates[_key];
		}
	}
}