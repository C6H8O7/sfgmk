/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		02/03/2015
	@brief		Courbe de Bézier

--------------------------------------------------------------------------------------------------*/


#ifndef BEZIER_HPP
#define BEZIER_HPP


namespace sfgmk
{
	namespace math
	{
		class Bezier : public Curb
		{
			public:
				Bezier(sf::Vector3f _Origin = sf::Vector3f(0.0f, 0.0f, 0.0f), sf::Vector3f Arrival = sf::Vector3f(0.0f, 0.0f, 0.0f), sf::Vector3f _ControlPoint1 = sf::Vector3f(0.0f, 0.0f, 0.0f), sf::Vector3f _ControlPoint2 = sf::Vector3f(0.0f, 0.0f, 0.0f), float _Time = 1.0f);
				Bezier(sf::Vector3f _Points[4], float _Time = 1.0f);
				~Bezier();

				void update(float _TimeDelta);

				void computeCoords();
		};
	}
}


#endif