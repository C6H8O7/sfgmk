namespace sfgmk
{
	namespace engine
	{
		Joystick::Joystick() : m_iId(0), m_iJoystickTolerance(JOYSTICK_TOLERANCE), m_iTriggerTolerance(TRIGGER_TOLERANCE), m_fVibrationTimer(0.0f)
		{
			m_Vibration.wLeftMotorSpeed = 0;
			m_Vibration.wRightMotorSpeed = 0;

			for( int i(0); i < sf::Mouse::ButtonCount; i++ )
				m_KeyStates[i] = KEY_UP;
		}

		Joystick::~Joystick()
		{
		}


		void Joystick::setId(int _Id)
		{
			m_iId = _Id;
		}


		void Joystick::updateJoystick()
		{
			//Update boutons
			for( int i(0); i < sf::Joystick::ButtonCount; i++ )
			{
				if( m_KeyStates[i] == KEY_PRESSED )
					m_KeyStates[i] = KEY_DOWN;
				else if( m_KeyStates[i] == KEY_RELEASED )
					m_KeyStates[i] = KEY_UP;
			}

			//Update vibrations controller
			if( m_fVibrationTimer > 0 )
				m_fVibrationTimer -= CORE->getTimeDelta();
			else if( m_fVibrationTimer <= 0 )
			{
				m_fVibrationTimer = 0;
				stopVibration();
			}
		}

		void Joystick::handleEvent(sf::Event _Event)
		{
			int key;

			switch( _Event.type )
			{
				case sf::Event::JoystickButtonPressed:
					key = _Event.joystickButton.button;
					if( m_KeyStates[key] == KeyStates::KEY_UP )
						m_KeyStates[key] = KeyStates::KEY_PRESSED;
					break;

				case sf::Event::JoystickButtonReleased:
					key = _Event.joystickButton.button;
					m_KeyStates[key] = KeyStates::KEY_RELEASED;
					break;

				default:
					break;
			}
		}


		int Joystick::getButtonState(int _Button)
		{
			return m_KeyStates[_Button];
		}

		float Joystick::getAxis(sf::Joystick::Axis _Axis)
		{
			return sf::Joystick::getAxisPosition(m_iId, _Axis);
		}


		int Joystick::getJoystickTolerance()
		{
			return m_iJoystickTolerance;
		}

		void Joystick::setJoystickTolerance(int _Value)
		{
			m_iJoystickTolerance = _Value;
		}

		int Joystick::getTriggerTolerance()
		{
			return m_iTriggerTolerance;
		}

		void Joystick::setTriggerTolerance(int _Value)
		{
			m_iJoystickTolerance = _Value;
		}


		void Joystick::setVibration(float _Time, sf::Vector2i _Vibration)
		{
			//Initialisation
			ZeroMemory(&m_Vibration, sizeof(XINPUT_VIBRATION));
			m_Vibration.wLeftMotorSpeed = _Vibration.x;
			m_Vibration.wRightMotorSpeed = _Vibration.y;
			m_fVibrationTimer = _Time;

			if( m_Vibration.wLeftMotorSpeed < 0 )
				m_Vibration.wLeftMotorSpeed = 0;
			if( m_Vibration.wRightMotorSpeed < 0 )
				m_Vibration.wRightMotorSpeed = 0;

			//Set vibration
			XInputSetState(m_iId, &m_Vibration);
		}

		void Joystick::stopVibration()
		{
			m_Vibration.wLeftMotorSpeed = 0;
			m_Vibration.wRightMotorSpeed = 0;
			XInputSetState(m_iId, &m_Vibration);
		}
	}
}