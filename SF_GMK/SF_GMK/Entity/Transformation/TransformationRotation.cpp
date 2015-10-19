namespace sfgmk
{
		TransformationRotation::TransformationRotation(const float& _DegreePerSecond, const float& _Duration, const bool& _DieOnTheEnd)
			: EntityTransformation(_Duration, _DieOnTheEnd), m_fDegreePerSecond(_DegreePerSecond)
		{
		}

		TransformationRotation::~TransformationRotation()
		{
		}


		void TransformationRotation::update(const float& _TimeDelta, Entity* _Entity)
		{
			_Entity->setRotation(_Entity->getRotation() + m_fDegreePerSecond * _TimeDelta);
		}


		float& TransformationRotation::getRefDegreePerSecond()
		{
			return m_fDegreePerSecond;
		}
}