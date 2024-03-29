namespace sfgmk
{
	TransformationInterpolation::TransformationInterpolation(math::CatmullRom* _Curb, const float& _Duration, const bool& _StopOntheEnd, const bool& _DieOnTheEnd) : EntityTransformation(_Duration, _StopOntheEnd, _DieOnTheEnd)
	{
		m_InterpolationCurb = _Curb;
		m_fDuration = _Duration;
	}

	TransformationInterpolation::~TransformationInterpolation()
	{
		delete m_InterpolationCurb;
	}


	void TransformationInterpolation::update(const float& _TimeDelta, Entity* _Entity)
	{
		m_InterpolationCurb->update(_TimeDelta);
		_Entity->setPosition(m_InterpolationCurb->getCurrentPosition());
	}


	math::CatmullRom* TransformationInterpolation::getCurb()
	{
		return m_InterpolationCurb;
	}
}