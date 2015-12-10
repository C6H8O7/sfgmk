namespace sfgmk
{
	namespace engine
	{
		InputManager::InputManager()
		{
			for( int i(0); i < sf::Joystick::Count; i++ )
				m_Joystick[i].setId(i);
		}

		InputManager::~InputManager()
		{
		}


		void InputManager::update()
		{
			m_Keyboard.updateKeyboard();

			m_Mouse.updateMouse();

			for( int i(0); i < sf::Joystick::Count; i++ )
			{
				if( sf::Joystick::isConnected(i) )
					m_Joystick[i].updateJoystick();
			}

			handleEvents();
		}

		void InputManager::handleEvents()
		{
			while( GRAPHIC_MANAGER->getRenderWindow()->pollEvent(m_Event) )
			{
				if( m_Event.type == sf::Event::Closed )
					GRAPHIC_MANAGER->getRenderWindow()->close();

				m_Keyboard.handleEvent(m_Event);

				m_Mouse.handleEvent(m_Event);

				for( int i(0); i < sf::Joystick::Count; i++ )
				{
					if( sf::Joystick::isConnected(i) )
						m_Joystick[i].handleEvent(m_Event);
				}
			}
		}


		Keyboard& InputManager::getKeyboard()
		{
			return m_Keyboard;
		}

		Mouse& InputManager::getMouse()
		{
			return m_Mouse;
		}

		Joystick& InputManager::getJoystick(int _Index)
		{
			return m_Joystick[_Index];
		}


		sf::Vector2f InputManager::getJoystickUnitVector(int _Id, sf::Joystick::Axis _AxisVertical, sf::Joystick::Axis _AxisHorizontal)
		{
			float fAngle = INPUT_MANAGER->getJoystickAngle(0, JOYSTICK_LEFT_VERTICAL, JOYSTICK_LEFT_HORIZONTAL);
			math::Matrix22 Rotation(cos(fAngle), -sin(fAngle), sin(fAngle), cos(fAngle));

			sf::Vector2f Direction = Rotation * sf::Vector2f(1.0f, 0.0f);
			Direction = math::Calc_UnitVector(Direction);
			Direction.x *= ABS(JOYSTICK_GET_AXIS(_Id, _AxisHorizontal));
			Direction.y *= ABS(JOYSTICK_GET_AXIS(_Id, _AxisVertical));

			return Direction;
		}

		float InputManager::getJoystickAngle(int _Id, sf::Joystick::Axis _AxisVertical, sf::Joystick::Axis _AxisHorizontal)
		{
			float fAngle = atan2f(JOYSTICK_GET_AXIS(_Id, _AxisVertical), JOYSTICK_GET_AXIS(_Id, _AxisHorizontal));

			//Permet d'avoir un angle entre 0 et 360, et pas entre -180 et 180
			if( fAngle < 0.0f )
				fAngle = 2.0f * GMK_PI + fAngle;

			return fAngle;
		}


		void InputManager::setGeneralVibration(float _Time, sf::Vector2i _Vibration)
		{
			for( int i(0); i < sf::Joystick::Count; i++ )
			{
				if( sf::Joystick::isConnected(i) )
					m_Joystick[i].setVibration(_Time, _Vibration);
			}
		}
	}
}