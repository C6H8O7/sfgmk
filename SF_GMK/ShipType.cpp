ShipType::ShipType(const ePLAYER_SHIP_TYPE& _Type) : m_Type(_Type), m_fPlayerCurrentRotationDegree(0.0f), m_fPlayerCurrentRotationRadian(0.0f)
{
	sfgmk::initArrayValue(m_Cooldowns, ePLAYER_WEAPON_NUMBER, 0.0f);

	m_Stats = ((StateGame*)(CURRENT_STATE))->Get_ShipsStats(eTypeI);
}

ShipType::~ShipType()
{
}


void ShipType::updateCooldowns(const float& _TimeDelta)
{
	for( int i(0); i < ePLAYER_WEAPON::ePLAYER_WEAPON_NUMBER; i++ )
	{
		if( m_Cooldowns[i] > 0.0f )
			m_Cooldowns[i] -= _TimeDelta;
	}
}

bool ShipType::shoot(const ePLAYER_WEAPON& _Weapon, Player* _Player, const float& _CurrentAngleDegree, const float& _CurrentAngleRadian)
{
	m_fPlayerCurrentRotationDegree = _CurrentAngleDegree;
	m_fPlayerCurrentRotationRadian = _CurrentAngleRadian;

	if( m_Cooldowns[_Weapon] <= 0.0f )
	{
		bool bShot = shootPrimaryWeapon(_Player);

		if( bShot )
		{
			m_Cooldowns[_Weapon] = m_Stats.fCooldownPrimaryWeapon;
			return true;
		}
	}

	return false;
}


const sSHIP_STAT& ShipType::Get_Stats()
{
	return m_Stats;
}