/*--------------------------------------------------------------------------------------------------

	@author		KCC
	@date		25/06/2015
	@brief		Classe mère des tirs

--------------------------------------------------------------------------------------------------*/

#ifndef SHOT_HPP
#define SHOT_HPP


enum ePLAYER_WEAPON
{
	ePrimaryWeapon = 0,
	ePLAYER_WEAPON_NUMBER
};

class Shot : public sfgmk::Entity
{
	private:
		sf::Vector3f m_Direction;
		float m_fSpeed;

		int m_iShooter;

		sfgmk::Grid* m_GridPtr;
		float m_fRepelForce;
		float m_fRepelSpeed;

	public:
		Shot(const int& _Id, const sf::Vector3f& _Direction, const float& _Speed, const int& _ShooterId, sfgmk::Grid* _GridPtr);
		virtual ~Shot();

		void Shot::update(const float& _TimeDelta);
};


#endif