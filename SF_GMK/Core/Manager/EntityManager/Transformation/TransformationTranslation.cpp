namespace sfgmk
{
	TransformationTranslation::TransformationTranslation(const sf::Vector2f& _Direction, const float& _Speed, const float& _Duration, const bool& _DieOnTheEnd)
		: EntityTransformation(_Duration, _DieOnTheEnd), m_Direction(sf::Vector3f(_Direction.x, _Direction.y, 0.0f)), m_fSpeed(_Speed)
	{
	}

	TransformationTranslation::TransformationTranslation(const sf::Vector3f& _Direction, const float& _Speed, const float& _Duration, const bool& _DieOnTheEnd)
		: EntityTransformation(_Duration, _DieOnTheEnd), m_Direction(_Direction), m_fSpeed(_Speed)
	{
	}

	TransformationTranslation::~TransformationTranslation()
	{
	}


	void TransformationTranslation::update(const float& _TimeDelta, Entity* _Entity)
	{
		_Entity->move(m_Direction * m_fSpeed * _TimeDelta);
	}
}