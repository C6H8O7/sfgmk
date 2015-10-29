namespace sfgmk
{
	EntityWithPv::EntityWithPv(const sf::Vector3f& _Position, const int& _InitialPv, const int& _CurrentPv, const bool& _DieWhenPvNull)
		: Entity(_Position), m_iPv(_CurrentPv), m_iInitialPv(_InitialPv), m_bDieWhenPvNull(_DieWhenPvNull), m_Lifebar(NULL)
	{
	}

	EntityWithPv::~EntityWithPv()
	{
		SAFE_DELETE(m_Lifebar);
	}


	void EntityWithPv::update(const float& _TimeDelta)
	{
		Entity::update(_TimeDelta);

		if( m_Lifebar )
			m_Lifebar->update(m_iPv);

		if( m_iPv <= 0 && m_bDieWhenPvNull )
			m_bIsAlive = false;
	}


	const int& EntityWithPv::getPv()
	{
		return m_iPv;
	}

	void EntityWithPv::setPv(const int& _Pv)
	{
		m_iPv = _Pv;
	}

	const int& EntityWithPv::removePv(const int& _PvToRemove)
	{
		m_iPv -= _PvToRemove;
		return m_iPv;
	}


	const int& EntityWithPv::getInitialPv()
	{
		return m_iInitialPv;
	}

	void EntityWithPv::setInitialPv(const int& _Pv)
	{
		m_iInitialPv = _Pv;
	}


	const bool& EntityWithPv::getDieWhenPvNull()
	{
		return m_bDieWhenPvNull;
	}

	void EntityWithPv::setDieWhenPvNull(const bool& _Boolean)
	{
		m_bDieWhenPvNull = _Boolean;
	}


	bool EntityWithPv::addLifeBar()
	{
		if( !m_Lifebar )
		{
			m_Lifebar = new LifeBar(m_iInitialPv, m_iPv);
			return true;
		}
		else
			return false;
	}

	bool EntityWithPv::removeLifeBar()
	{
		if( m_Lifebar )
		{
			delete m_Lifebar;
			return true;
		}
		else
			return false;
	}
}