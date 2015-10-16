namespace sfgmk
{
	SphereCollider::SphereCollider(Entity* _Entity, float _Radius) : Collider(_Entity), m_fRadius(_Radius)
	{
		m_Type = eCOLLIDER_TYPE::eSphere;
	}

	SphereCollider::~SphereCollider()
	{
	}


	const float& SphereCollider::getRadius()
	{
		return m_fRadius;
	}

	float SphereCollider::getWorldRadius()
	{
		sf::Vector2f Scale = m_Entity->getScale();
		return m_fRadius * MAX(Scale.x, Scale.y);
	}

	void SphereCollider::setRadius(const float& _Radius)
	{
		m_fRadius = _Radius;
	}
}