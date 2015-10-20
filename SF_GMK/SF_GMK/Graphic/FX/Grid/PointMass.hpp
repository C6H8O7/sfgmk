/*--------------------------------------------------------------------------------------------------

	@author		GMK (sources: Terence J. Grant / http://gamedevelopment.tutsplus.com/tutorials/make-a-neon-vector-shooter-for-ios-the-warping-grid--gamedev-14637
	@date		13/04/2015
	@brief		Classe représentant un point affecté d'une masse, sur lequel seront appliqués des forces

--------------------------------------------------------------------------------------------------*/

#ifndef POINT_MASS_HPP
#define POINT_MASS_HPP


namespace sfgmk
{
#define POINT_MASS_DEFAULT_DAMPING 0.98f

	class SFGMK_API PointMass
	{
		protected:
			sf::Vector3f m_Acceleration;
			float m_fDamping;

		public:
			sf::Vector3f m_Position;
			sf::Vector3f m_Velocity;
			float m_fInverseMass;

			PointMass();
			PointMass(const sf::Vector3f& _Position, float _InvMass);

			void applyForce(const sf::Vector3f& _Force);
			void increaseDamping(float _Factor);

			void update();
	};
}


#endif