namespace sfgmk
{
	Circle::Circle(float _radius, float _degreesSeconds) : sfgmk::SpriteComponent()
	{
		m_fRadius = _radius;
		m_AngularSpeed = _degreesSeconds;
	}

	Circle::~Circle()
	{
	}

	bool Circle::update(Sprite& _sprite, float _timeDelta)
	{
		m_fTimer += _timeDelta;

		float dx = m_fRadius * cos(m_fTimer * (PI / 180.0f) * m_AngularSpeed);
		float dy = m_fRadius * sin(m_fTimer * (PI / 180.0f) * m_AngularSpeed);

		_sprite.GetSpriteData().translationX += dx;
		_sprite.GetSpriteData().translationY += dy;

		return true;
	}
}