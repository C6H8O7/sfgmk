namespace sfgmk
{
	Rotation::Rotation(float _degreesSeconds) : sfgmk::SpriteComponent()
	{
		m_AngularSpeed = _degreesSeconds;
	}

	Rotation::~Rotation()
	{

	}

	bool Rotation::update(Sprite& _sprite, float _timeDelta)
	{
		m_fTimer += _timeDelta;
		_sprite.GetSpriteData().rotationOffset += m_fTimer * m_AngularSpeed;

		return true;
	}
}