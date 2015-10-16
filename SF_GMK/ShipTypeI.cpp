ShipTypeI::ShipTypeI(const ePLAYER_SHIP_TYPE& _Type) : ShipType(_Type)
{
}

ShipTypeI::~ShipTypeI()
{
}


void ShipTypeI::update(const float& _TimeDelta)
{
	updateCooldowns(_TimeDelta);
}


bool ShipTypeI::shootPrimaryWeapon(Player* _Player)
{
	sf::Vector2f Scale = _Player->getScale();
	sf::Vector2f DecalVectors[2];
	Shot* NewShot = NULL;
	sfgmk::Sprite* Sprite = NULL;

	//Tourelle gauche
	DecalVectors[0] = sf::Vector2f(25.0f, 35.0f);
	DecalVectors[0] = sfgmk::MatrixRotation22(m_fPlayerCurrentRotationRadian) * DecalVectors[0];
	DecalVectors[0].x *= Scale.x;
	DecalVectors[0].y *= Scale.y;

	//Tourelle droite
	DecalVectors[1] = sf::Vector2f(25.0f, -35.0f);
	DecalVectors[1] = sfgmk::MatrixRotation22(m_fPlayerCurrentRotationRadian) * DecalVectors[1];
	DecalVectors[1].x *= Scale.x;
	DecalVectors[1].y *= Scale.y;

	for( int i(0); i < 2; i++ )
	{
		NewShot = new Laser(eLaser, sfgmk::Convert2dTo3d(_Player->getDirection()), _Player->getId(), _Player->getGridPtr(), eRedLaser);
		NewShot->setPosition(_Player->getPosition() + DecalVectors[i]);
		NewShot->setRelativOrigin(0.5f, 0.5f);
		NewShot->setRotation(m_fPlayerCurrentRotationDegree);
		ADD_ENTITY(NewShot);
	}

	return true;
}