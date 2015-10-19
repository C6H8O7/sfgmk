namespace sfgmk
{
		EntityWithPv::EntityWithPv(const bool& _DieWhenPvNull) : m_iPv(ENTITY_DEFAULT_PV), m_bDieWhenPvNull(_DieWhenPvNull)
		{
		}

		EntityWithPv::EntityWithPv(const int& _Id, const bool& _DieWhenPvNull) : EntityWithPv(_DieWhenPvNull)
		{
			m_iId = _Id;
		}

		EntityWithPv::~EntityWithPv()
		{
		}


		void EntityWithPv::update(const float& _TimeDelta)
		{
			if( m_iPv <= 0 && m_bDieWhenPvNull )
				m_bIsAlive = false;

			Entity::update(_TimeDelta);
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


		const bool& EntityWithPv::getDieWhenPvNull()
		{
			return m_bDieWhenPvNull;
		}

		void EntityWithPv::setDieWhenPvNull(const bool& _Boolean)
		{
			m_bDieWhenPvNull = _Boolean;
		}
}