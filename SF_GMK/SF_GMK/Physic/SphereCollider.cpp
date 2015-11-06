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
			const sf::Vector2f& scale = m_Entity->getScale();
			return m_fRadius * MAX(scale.x, scale.y);
		}

		float SphereCollider::getSquaredWorldRadius()
		{
			float r = getWorldRadius();
			return r * r;
		}

		void SphereCollider::setRadius(const float& _Radius)
		{
			m_fRadius = _Radius;
		}
}