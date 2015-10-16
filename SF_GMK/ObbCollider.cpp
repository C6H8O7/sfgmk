namespace sfgmk
{
	ObbCollider::ObbCollider(Entity* _Entity, sf::Vector2f _Size) : Collider(_Entity), m_Size(_Size)
	{
		m_Type = eCOLLIDER_TYPE::eOBB;

		computeRect();
	}

	ObbCollider::~ObbCollider()
	{
	}


	void ObbCollider::computeRect()
	{
		sf::FloatRect Rect = m_Entity->getSprite()->getLocalBounds();

		m_Min = sf::Vector2f(0.0f, 0.0f);
		m_Max = sf::Vector2f(Rect.width, Rect.height);
	}


	const sf::Vector2f& ObbCollider::getSize()
	{
		return m_Size;
	}

	sf::Vector2f ObbCollider::getWorldSize()
	{
		sf::Vector2f Scale = m_Entity->getScale();
		return sf::Vector2f(m_Size.x * Scale.x, m_Size.y * Scale.y);
	}

	const sf::Vector2f& ObbCollider::getMin()
	{
		return m_Min;
	}

	const sf::Vector2f& ObbCollider::getMax()
	{
		return m_Max;
	}


	sf::Vector2f ObbCollider::getRight()
	{
		float fAngle = DEG_TO_RAD(m_Entity->getRotation());
		return MatrixRotation22(fAngle) * X_UNIT_VECTOR_2D;
	}

	sf::Vector2f ObbCollider::getUp()
	{
		float fAngle = DEG_TO_RAD(m_Entity->getRotation());
		return MatrixRotation22(fAngle) * Y_UNIT_VECTOR_2D;
	}
}