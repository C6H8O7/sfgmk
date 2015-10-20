/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		09/03/2015
	@brief		Class mère des différentes courbes d'interpolation

--------------------------------------------------------------------------------------------------*/


#ifndef CURB_HPP
#define CURB_HPP


namespace sfgmk
{
	namespace math
	{
		class SFGMK_API Curb
		{
			protected:
				sf::Vector3f* m_Points;
				sf::Vector3f m_CurrentPos;
				float m_fTotalTime;
				float m_fCurrentTime;
				float m_fTimeRatio;

			public:
				Curb(float _Duration);
				virtual ~Curb();

				virtual void update(float _TimeDelta) = 0;

				virtual void computeCoords() = 0;

				sf::Vector3f* getPoints();
				sf::Vector3f getCurrentPosition();
				float getTotalTime();
				float getCurrentTime();
				float getTimeRatio();
		};
	}
}


#endif