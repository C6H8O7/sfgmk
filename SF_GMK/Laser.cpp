Laser::Laser(const int& _Id, const sf::Vector3f& _Direction, const int& _ShooterId, sfgmk::Grid* _GridPtr, const eLASER_TYPE& _LaserType)
	: Shot(_Id, _Direction, LASER_SPEED, _ShooterId, _GridPtr), m_Type(_LaserType)
{
	switch( m_Type )
	{
		case eRedLaser:
			m_Sprite->setTexture(DATA_MANAGER->getTexture("redLaser"));
			break;

		default:
			break;
	}

	addObbCollider();
}

Laser::~Laser()
{
}