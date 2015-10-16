/*--------------------------------------------------------------------------------------------------

	@author		KCC
	@date		01/07/2015
	@brief		Classe du ship de type I

--------------------------------------------------------------------------------------------------*/

#ifndef SHIP_TYPE_I_HPP
#define SHIP_TYPE_I_HPP


class ShipTypeI : public ShipType
{
	public:
		ShipTypeI(const ePLAYER_SHIP_TYPE& _Type);
		~ShipTypeI();

		void update(const float& _TimeDelta);

		bool shootPrimaryWeapon(Player* _Player);
};


#endif