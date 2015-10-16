/*--------------------------------------------------------------------------------------------------

	@author		KCC
	@date		04/07/2015
	@brief		Classe héritant de Shot, permettant d'instancier un laser de plusieurs types

--------------------------------------------------------------------------------------------------*/

#ifndef LASER_HPP
#define LASER_HPP


#define LASER_SPEED 1000.0f

enum eLASER_TYPE
{
	eRedLaser = 0,
	eLASER_TYPE_NUMBER
};

class Laser : public Shot
{
	private:
		eLASER_TYPE m_Type;

	public:
		Laser(const int& _Id, const sf::Vector3f& _Direction, const int& _ShooterId, sfgmk::Grid* _GridPtr, const eLASER_TYPE& _LaserType);
		~Laser();
};


#endif