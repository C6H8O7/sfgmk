/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		04/01/2015
	@brief		Rotation

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_TRANSFORMATION_ROTATION_HPP
#define SFGMK_TRANSFORMATION_ROTATION_HPP


namespace sfgmk
{
	class TransformationRotation : public EntityTransformation
	{
		private:
			float m_fDegreePerSecond;

		public:
			TransformationRotation(const float& _DegreePerSecond, const float& _Duration, const bool& _DieOnTheEnd = false);
			~TransformationRotation();

			void update(const float& _TimeDelta, Entity* _Entity);

			float& getRefDegreePerSecond();
	};
}


#endif