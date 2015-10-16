namespace sfgmk
{
	DeadTimer::DeadTimer(float _timeInSeconds, float _differedStart)
	{
		m_fLimit = _timeInSeconds + _differedStart;
	}

	DeadTimer::~DeadTimer()
	{
	}

	bool DeadTimer::update(Sprite& _sprite, float _timeDelta)
	{
		m_fTimer += _timeDelta;

		if( m_fTimer >= m_fLimit )
		{
			_sprite.GetSpriteData().alive = false;
			return false;
		}
		return true;
	}
}