namespace sfgmk
{
	Collider::Collider(Entity* _Entity) : m_bActive(true), m_bCollide(false), m_Entity(_Entity)
	{
		PHYSIC_MANAGER->addCollider(this);
	}

	Collider::~Collider()
	{
		PHYSIC_MANAGER->removeCollider(this);
	}


	const eCOLLIDER_TYPE& Collider::getType()
	{
		return m_Type;
	}

	const bool& Collider::isActive()
	{
		return m_bActive;
	}

	void Collider::setActive(bool _Boolean)
	{
		m_bActive = _Boolean;
	}

	const bool& Collider::Collide()
	{
		return m_bCollide;
	}

	void Collider::setCollide(bool _Boolean)
	{
		m_bCollide = _Boolean;
	}


	sf::Vector2f Collider::getWorldCenter()
	{
		return m_Entity->getPosition();
	}


	Entity* Collider::getEntity()
	{
		return m_Entity;
	}
}