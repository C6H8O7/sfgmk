namespace sfgmk
{
	namespace engine
	{
		Camera::Camera() : m_fZoomFactor(1.0f), m_bLinked(false), m_LinkedEntity(NULL), m_LinkOffset(0.0f, 0.0f), m_bFreeMove(false),
			m_Speed(sf::Vector2f(FREE_CAMERA_DEFAULT_SPEED, FREE_CAMERA_DEFAULT_SPEED)),
			m_bVibrate(false), m_VibrationRange(0.0f, 0.0f), m_VibrationAttenuation(0.0f, 0.0f), m_fVibrationTimer(0.0f), m_RelativOrigin(sf::Vector2f(0.0f, 0.0f))
		{
			sf::Vector2u CurrentResolution = GRAPHIC_MANAGER->getRenderWindow()->getSize();

			this->setCenter(sf::Vector2f(CurrentResolution.x * 0.5f, CurrentResolution.y * 0.5f));
			this->setSize(sf::Vector2f(CurrentResolution));
		}

		Camera::~Camera()
		{
		}


		void Camera::update(float _TimeDelta)
		{
			//Caméra libre
			if( m_bFreeMove )
				freeMove(_TimeDelta);

			//Suivi d'une entité
			else if( m_bLinked )
				this->setCenter(sf::Vector2f(m_LinkedEntity->getPosition().x, m_LinkedEntity->getPosition().y) + m_LinkOffset);

			//Vibration caméra
			if( m_bVibrate )
				vibrate(_TimeDelta);

			//Update de l'origine actuelle du repère(fenêtre)
			m_RelativOrigin = sf::Vector2f(this->getCenter().x - this->getSize().x * 0.5f, this->getCenter().y - this->getSize().y * 0.5f);

			//Rectangle de vue
			m_Rect = sf::Rect<float>(sf::Vector2f(getCenter().x - getSize().x * 0.5f, getCenter().y - getSize().y * 0.5f), getSize());
		}


		sf::Vector2f Camera::getRelativOrigin()
		{
			return m_RelativOrigin;
		}

		sf::Vector2f Camera::getRelativOriginWithZoomCompensation()
		{
			return sf::Vector2f(this->getCenter().x - this->getSize().x * m_fZoomFactor * 0.5f, this->getCenter().y - this->getSize().y * m_fZoomFactor * 0.5f);
		}

		float Camera::getZoomFactor()
		{
			return m_fZoomFactor;
		}

		void Camera::zoom(float factor)
		{
			m_fZoomFactor *= 1.0f / factor;
			((sf::View*)(this))->zoom(factor);
		}


		void Camera::linkToEntity(Entity* _Entity, const sf::Vector2f& _Offset)
		{
			m_bLinked = true;
			m_LinkedEntity = _Entity;
			m_LinkOffset = _Offset;
		}

		Entity* Camera::getLinkedEntity()
		{
			return m_LinkedEntity;
		}

		void Camera::unlink(bool _DestroyEntity)
		{
			m_bLinked = false;

			if( _DestroyEntity )
				delete m_LinkedEntity;

			m_LinkedEntity = NULL;
			m_LinkOffset = sf::Vector2f(0.0f, 0.0f);
		}

		bool Camera::getFreeMove()
		{
			return m_bFreeMove;
		}

		bool Camera::setFreeMove()
		{
			m_bFreeMove = !m_bFreeMove;

			return m_bFreeMove;
		}

		void Camera::freeMove(float _TimeDelta)
		{
			//Clavier / Souris
				//Déplacements
					//Gauche
					if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::Left) == KeyStates::KEY_DOWN )
						this->move(-m_Speed.x * _TimeDelta, 0);

					//Droite
					else if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::Right) == KeyStates::KEY_DOWN )
						this->move(m_Speed.x * _TimeDelta, 0);

					//Haut
					if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::Up) == KeyStates::KEY_DOWN )
						this->move(0, -m_Speed.y * _TimeDelta);

					//Bas
					else if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::Down) == KeyStates::KEY_DOWN )
						this->move(0, m_Speed.y * _TimeDelta);

					//Zoom
						//Arriere
						if( MOUSE.getWheelState() == -1 )
							this->zoom(2.0f);

						//Avant
						else if( MOUSE.getWheelState() == 1 )
							this->zoom(0.5f);

			//Manette (si un joystick connecté)
				//Déplacements
				if( sf::Joystick::isConnected(0) )
				{
					sf::Vector2f JoystickPow(ABS(JOYSTICK_GET_AXIS(0, JOYSTICK_LEFT_HORIZONTAL)), ABS(JOYSTICK_GET_AXIS(0, JOYSTICK_LEFT_VERTICAL)));

					if( JoystickPow.x > JOYSTICK_TOLERANCE || JoystickPow.y > JOYSTICK_TOLERANCE )
					{
						float fJoystickAngle = INPUT_MANAGER->getJoystickAngle(0, JOYSTICK_LEFT_VERTICAL, JOYSTICK_LEFT_HORIZONTAL);
						sf::Vector2f Result = math::Matrix22(cos(fJoystickAngle), -sin(fJoystickAngle), sin(fJoystickAngle), cos(fJoystickAngle)) * sf::Vector2f(1.0f, 0.0f);

						this->move(Result.x * m_Speed.x * _TimeDelta, Result.y * m_Speed.y * _TimeDelta);
					}
				}
		}


		void Camera::setVibration(float _Time, const sf::Vector2f& _Range, const sf::Vector2f& _Attenuation)
		{
			m_bVibrate = true;
			m_fVibrationTimer = _Time;
			m_VibrationRange = _Range;
			m_VibrationAttenuation = _Attenuation;
		}

		void Camera::stopVibration()
		{
			m_bVibrate = false;
			m_fVibrationTimer = 0.0f;
			m_VibrationRange = sf::Vector2f(0.0f, 0.0f);
			m_VibrationAttenuation = sf::Vector2f(0.0f, 0.0f);
		}

		void Camera::vibrate(float _TimeDelta)
		{
			this->move((float)RAND((int)-m_VibrationRange.x, (int)m_VibrationRange.y), (float)RAND(-(int)m_VibrationRange.y, (int)m_VibrationRange.y));

			m_fVibrationTimer -= _TimeDelta;
			m_VibrationRange -= m_VibrationAttenuation * _TimeDelta;

			if( m_fVibrationTimer <= 0 )
				stopVibration();
		}


		const sf::Rect<float>& Camera::getRect()
		{
			return m_Rect;
		}
	}
}