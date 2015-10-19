/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		10/03/2015
	@brief		Déplacement suivant une courbe d'interpolation

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_TRANSFORMATION_INTERPOLATION_HPP
#define SFGMK_TRANSFORMATION_INTERPOLATION_HPP


namespace sfgmk
{
	class TransformationInterpolation : public EntityTransformation
	{
		private:
			math::CatmullRom* m_InterpolationCurb;

		public:
			TransformationInterpolation(math::CatmullRom* _Curb, const float& _Duration, const bool& _DieOnTheEnd = false);
			~TransformationInterpolation();

			void update(const float& _TimeDelta, Entity* _Entity);

			math::CatmullRom* getCurb();
	};
}


#endif