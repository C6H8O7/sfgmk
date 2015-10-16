/*--------------------------------------------------------------------------------------------------

	@author		KCC
	@date		25/06/2015
	@brief		Classe du joueur

--------------------------------------------------------------------------------------------------*/

#ifndef PLAYER_HPP
#define PLAYER_HPP


#define PLAYER_MAX_NUMBER 4
#define PLAYER_ROTATION_TOLERANCE 0.2f

enum ePLAYER_SHIP_TYPE
{
	eTypeI = 0,
	ePLAYER_SHIP_TYPE_NUMBER
};

class ShipType;
class ShipTypeI;

class Player : public sfgmk::EntityWithPv
{
	private:
		unsigned int m_uiPlayerNumber;

		ShipType* m_Ship;

		sf::Vector2f m_JoystickForce;
		float m_fSpeed;
		sf::Vector2f m_Direction;
		sf::Vector2f m_Velocity;

		sfgmk::Grid* m_GridPtr;

	public:
		Player(const unsigned int& _PlayerId, const ePLAYER_SHIP_TYPE& _Type, sfgmk::Grid* _GridPtr);
		~Player();

		void update(const float& _TimeDelta);

		void ControllerUpdate(const float& _TimeDelta);
		float Rotate(const float& _TimeDelta, float& _CurrentAngleDegree, float& _CurrentAngleRadian);
		void ComputeVelocity(const float& _TimeDelta, bool _Input);

		const sf::Vector2f& getDirection();
		const sf::Vector2f& getVelocity();

		sfgmk::Grid* getGridPtr();
};


#endif