/*--------------------------------------------------------------------------------------------------

	@author		GMK (sources: Terence J. Grant / http://gamedevelopment.tutsplus.com/tutorials/make-a-neon-vector-shooter-for-ios-the-warping-grid--gamedev-14637
	@date		13/04/2015
	@brief		Classe représentant un ressort
				A spring connects two point masses, and, if stretched past its natural length, applies a force pulling the masses together. 
				Springs follow a modified version of Hooke's Law with damping:

				f = −kx − bv

				f is the force produced by the spring.
				k is the spring constant, or the stiffness of the spring.
				x is the distance the spring is stretched beyond its natural length.
				b is the damping factor.
				v is the velocity.

--------------------------------------------------------------------------------------------------*/

#ifndef SPRING_HPP
#define SPRING_HPP


namespace sfgmk
{
	class Spring
	{
		public:
			PointMass* m_End1;
			PointMass* m_End2;
			float m_fTargetLength;
			float m_fStiffness;
			float m_fDamping;

			bool m_bBehind;

		public:
			Spring(PointMass* _End1, PointMass* _End2, float _Stiffness, float _Damping);

			void update();
	};
}


#endif