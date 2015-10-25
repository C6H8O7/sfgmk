/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		30/06/2015
	@brief		Translation

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_TRANSFORMATION_TRANSLATION_HPP
#define SFGMK_TRANSFORMATION_TRANSLATION_HPP


namespace sfgmk
{
	class SFGMK_API TransformationTranslation : public EntityTransformation
	{
		private:
			sf::Vector3f m_Direction;
			float m_fSpeed;

		public:
			TransformationTranslation(const sf::Vector2f& _Direction, const float& _Speed, const float& _Duration, const bool& _StopOntheEnd = false, const bool& _DieOnTheEnd = false);
			TransformationTranslation(const sf::Vector3f& _Direction, const float& _Speed, const float& _Duration, const bool& _StopOntheEnd = false, const bool& _DieOnTheEnd = false);
			~TransformationTranslation();

			void update(const float& _TimeDelta, Entity* _Entity);
	};
}


#endif