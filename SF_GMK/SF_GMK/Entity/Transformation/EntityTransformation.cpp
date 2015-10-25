namespace sfgmk
{
	EntityTransformation::EntityTransformation(const float& _Duration, const bool& _StopOntheEnd, const bool& _DieOnTheEnd)
		: m_fTimer(0.0f), m_fDuration(_Duration), m_bStopOnTheEnd(_StopOntheEnd), m_bDieOnTheEnd(_DieOnTheEnd)
	{
	}

	EntityTransformation::~EntityTransformation()
	{
	}


	bool EntityTransformation::checkTimer(const float& _TimeDelta, Entity* _Entity)
	{
		m_fTimer += _TimeDelta;
		if( m_fTimer >= m_fDuration )
		{
			if( m_bDieOnTheEnd )
				_Entity->setIsAlive(false);
		
			if( m_bStopOnTheEnd )
				return false;
		}

		return true;
	}
}