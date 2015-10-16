/*--------------------------------------------------------------------------------------------------

	@author		KCC
	@date		01/07/2015
	@brief		Classe mère des types de vaisseaux (virtuelle)

--------------------------------------------------------------------------------------------------*/

#ifndef SHIP_TYPE_HPP
#define SHIP_TYPE_HPP


#define SHIPS_STATS_PATH std::string("data/ships/")

struct sSHIP_STAT
{
	unsigned int iLife;

	float fSpeed;
	float fAcceleration;
	float fAccelerationReactivity;
	float fDecelerationReactivity;
	float fRotation;

	float fCooldownPrimaryWeapon;
};

class ShipType
{
	protected:
		ePLAYER_SHIP_TYPE m_Type;
		sSHIP_STAT m_Stats;

		float m_Cooldowns[ePLAYER_WEAPON::ePLAYER_WEAPON_NUMBER];

		float m_fPlayerCurrentRotationDegree;
		float m_fPlayerCurrentRotationRadian;
		
	public:
		ShipType(const ePLAYER_SHIP_TYPE& _Type);
		virtual ~ShipType();

		virtual void update(const float& _TimeDelta) = 0;

		void updateCooldowns(const float& _TimeDelta);
		bool shoot(const ePLAYER_WEAPON& _Weapon, Player* _Player, const float& _CurrentAngleDegree, const float& _CurrentAngleRadian);
		virtual bool shootPrimaryWeapon(Player* _Player) = 0;

		const sSHIP_STAT& Get_Stats();
};


#endif