/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		02/01/2015
	@brief		Classe permettant d'appliquer des transformations prédéfinies sur les sprites des entity

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_ENTITY_TRANSFORMATION_HPP
#define SFGMK_ENTITY_TRANSFORMATION_HPP


namespace sfgmk
{
		class EntityTransformation
		{
			protected:
			float m_fTimer;
			float m_fDuration;
			bool m_bStopOnTheEnd;
			bool m_bDieOnTheEnd;

			public:
			EntityTransformation(const float& _Duration, const bool& _StopOntheEnd = false, const bool& _DieOnTheEnd = false);
			virtual ~EntityTransformation();

			virtual void update(const float& _TimeDelta, Entity* _Entity) = 0;
			bool checkTimer(const float& _TimeDelta, Entity* _Entity);
		};
}


#endif