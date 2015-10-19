namespace sfgmk
{
	TransformationBezier::TransformationBezier(math::Bezier* _Curb, const float& _Duration, const bool& _DieOnTheEnd) : EntityTransformation(_DieOnTheEnd)
	{
		m_BezierCurb = _Curb;
		m_fDuration = _Duration;
	}

	TransformationBezier::~TransformationBezier()
	{
		delete m_BezierCurb;
	}


	void TransformationBezier::update(const float& _TimeDelta, Entity* _Entity)
	{
		m_BezierCurb->update(_TimeDelta);
		_Entity->setPosition(m_BezierCurb->getCurrentPosition());
	}


	math::Bezier* TransformationBezier::getCurb()
	{
		return m_BezierCurb;
	}
}