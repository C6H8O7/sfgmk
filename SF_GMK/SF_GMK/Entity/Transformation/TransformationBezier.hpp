/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		04/03/2015
	@brief		D�placement suivant une courbe de B�zier

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_TRANSFORMATION_BEZIER_HPP
#define SFGMK_TRANSFORMATION_BEZIER_HPP


namespace sfgmk
{
		class TransformationBezier : public EntityTransformation
		{
			private:
			math::Bezier* m_BezierCurb;

			public:
			TransformationBezier(math::Bezier* _Curb, const float& _Duration, const bool& _DieOnTheEnd = false);
			~TransformationBezier();

			void update(const float& _TimeDelta, Entity* _Entity);

			math::Bezier* getCurb();
		};
}


#endif