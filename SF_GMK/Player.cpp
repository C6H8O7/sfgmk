Player::Player(const unsigned int& _PlayerId, const ePLAYER_SHIP_TYPE& _Type, sfgmk::Grid* _GridPtr) : EntityWithPv((int)_PlayerId),
	m_uiPlayerNumber(_PlayerId), m_JoystickForce(sf::Vector2f(0.0f, 0.0f)), m_fSpeed(0.0f), m_Direction(sf::Vector2f(0.0f, 0.0f)), m_Velocity(sf::Vector2f(0.0f, 0.0f)), m_GridPtr(_GridPtr)
{
	m_Sprite = new sfgmk::Sprite();
	
	switch( _Type )
	{
		case ePLAYER_SHIP_TYPE::eTypeI:
			m_Ship = new ShipTypeI(_Type);
			m_Sprite->setTexture(DATA_MANAGER->getTexture("playerTypeI"));
			setScale(sf::Vector2f(0.75f, 0.75f));
			break;

		default:
			std::cout << "Erreur: type  vaisseau inconnu" << std::endl;
			break;
	}
}

Player::~Player()
{
}


void Player::update(const float& _TimeDelta)
{
	//Update du vaisseau
	m_Ship->update(_TimeDelta);

	//Update spécifique du joueur
	ControllerUpdate(_TimeDelta);

	//Update de la classe mère
	EntityWithPv::update(_TimeDelta);

	//Déformations de la grille
	m_GridPtr->applyExplosiveForce(m_fSpeed, getPosition3D(), 30.0f);
}


void Player::ControllerUpdate(const float& _TimeDelta)
{
	bool bInput(false);
	float fCurrentAngleDegree = getRotation();
	float fCurrentAngleRadian = DEG_TO_RAD(fCurrentAngleDegree);

	//Lecture position actuelle joystick gauche
	m_JoystickForce.x = SFGMK_JOYSTICK_GET_AXIS(m_uiPlayerNumber, JOYSTICK_LEFT_HORIZONTAL);
	m_JoystickForce.y = SFGMK_JOYSTICK_GET_AXIS(m_uiPlayerNumber, JOYSTICK_LEFT_VERTICAL);

	//Rotation
	if( (bInput = (ABS(m_JoystickForce.x) > JOYSTICK_TOLERANCE || ABS(m_JoystickForce.y) > JOYSTICK_TOLERANCE)) )
		Rotate(_TimeDelta, fCurrentAngleDegree, fCurrentAngleRadian);

	m_Direction = sfgmk::Calc_UnitVector((sf::Vector2f(cosf(fCurrentAngleRadian), sinf(fCurrentAngleRadian))));

	//Déplacement
	ComputeVelocity(_TimeDelta, bInput);

	//Tir
	if( SFGMK_JOYSTICK_GET_BUTTON(m_uiPlayerNumber, BUTTON_A) == sfgmk::KEY_DOWN )
		m_Ship->shoot(ePrimaryWeapon, this, fCurrentAngleDegree, fCurrentAngleRadian);
}

float Player::Rotate(const float& _TimeDelta, float& _CurrentAngleDegree, float& _CurrentAngleRadian)
{
	//float fCurrentAngleDegree = _CurrentAngleDegree;
	float fJoystickAngle = INPUT_MANAGER->getJoystickAngle(m_uiPlayerNumber, JOYSTICK_LEFT_VERTICAL, JOYSTICK_LEFT_HORIZONTAL);
	
	//Rotation
	if( ABS(_CurrentAngleRadian - fJoystickAngle) > PLAYER_ROTATION_TOLERANCE )
	{
		float fSpeed = m_Ship->Get_Stats().fRotation * _TimeDelta;

		if( _CurrentAngleRadian < fJoystickAngle )
			(fJoystickAngle - _CurrentAngleRadian) < ((2.0f * PI) - fJoystickAngle + _CurrentAngleRadian) ? fSpeed = _CurrentAngleDegree + fSpeed : fSpeed = _CurrentAngleDegree - fSpeed;
		else if( _CurrentAngleRadian > fJoystickAngle )
			(_CurrentAngleRadian - fJoystickAngle) < ((2.0f * PI) - _CurrentAngleRadian + fJoystickAngle) ? fSpeed = _CurrentAngleDegree - fSpeed : fSpeed = _CurrentAngleDegree + fSpeed;

		_CurrentAngleDegree = fSpeed;
		setRotation(_CurrentAngleDegree);
		_CurrentAngleRadian = DEG_TO_RAD(_CurrentAngleDegree);
	}

	return _CurrentAngleRadian;
}

void Player::ComputeVelocity(const float& _TimeDelta, bool _Input)
{
	sSHIP_STAT Stats = m_Ship->Get_Stats();

	//Le joueur agit
	if( _Input )
	{
		float fComputedSpeed(0.0f), fTargetSpeed = ABS(m_JoystickForce.x) >= ABS(m_JoystickForce.y) ? m_JoystickForce.x : m_JoystickForce.y;
		if( fTargetSpeed < 0.0f )
			fTargetSpeed = -fTargetSpeed;

		fComputedSpeed = Stats.fAcceleration * (Stats.fAccelerationReactivity + m_fSpeed / JOYSTICK_MAX_VALUE);
		m_fSpeed < fTargetSpeed ? m_fSpeed += fComputedSpeed : m_fSpeed -= fComputedSpeed;
	}

	//Pas d'action joueur, le vaisseau ralentit
	else
		m_fSpeed -= Stats.fAcceleration * Stats.fDecelerationReactivity * (m_fSpeed / JOYSTICK_MAX_VALUE);
	
	//Déplacement selon la vélocité calculée
	m_Velocity = m_Direction * m_fSpeed * Stats.fSpeed;
	move(m_Velocity * _TimeDelta);
}


const sf::Vector2f& Player::getDirection()
{
	return m_Direction;
}

const sf::Vector2f& Player::getVelocity()
{
	return m_Velocity;
}


sfgmk::Grid* Player::getGridPtr()
{
	return m_GridPtr;
}