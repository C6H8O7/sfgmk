namespace sfgmk
{
	Translation::Translation(float _degrees, float _speed)
	{
		float rad = PI / 180.0f * _degrees;
		
		m_Vx = cosf(rad); 
		m_Vy = sinf(rad);

		m_Speed = _speed;
	}

	Translation::~Translation()
	{
	}

	bool Translation::update(Sprite& _sprite, float _timeDelta)
	{
		float totalTime = 2.0f;
		m_fTimer += _timeDelta;

		SpriteData& spriteData = _sprite.GetSpriteData();

		float factor = sinf(PI / 4.0f - (m_fTimer / totalTime) * (PI / 4.0f));
		float speed = m_Speed * factor;
		
		float x = spriteData.X + speed * m_Vx * _timeDelta;
		float y = spriteData.Y + speed * m_Vy * _timeDelta;

		_sprite.setPosition(x, y);
		
		//spriteData.translationX += m_Vx * m_Speed * m_fTimer;
		//spriteData.translationY += m_Vy * m_Speed * m_fTimer;

		spriteData.color = sf::Color(_sprite.getColor().r, _sprite.getColor().g, _sprite.getColor().b, _sprite.getColor().a - (UINT8)(m_fTimer / totalTime * 255.0f));

		if(m_fTimer > totalTime)
		{
			spriteData.alive = false;
			return false;
		}
		
		return true;
	}
}