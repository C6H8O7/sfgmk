namespace sfgmk
{
		PointMass::PointMass()
			: m_Acceleration(0.0f, 0.0f, 0.0f), m_fDamping(POINT_MASS_DEFAULT_DAMPING), m_Position(0.0f, 0.0f, 0.0f), m_Velocity(0.0f, 0.0f, 0.0f), m_fInverseMass(0.0f)
		{
		}

		PointMass::PointMass(const sf::Vector3f& Position, float _InvMass)
			: m_Acceleration(0.0f, 0.0f, 0.0f), m_fDamping(0.98f), m_Position(Position), m_Velocity(0, 0, 0), m_fInverseMass(_InvMass)
		{
		}


		void PointMass::applyForce(const sf::Vector3f& _Force)
		{
			m_Acceleration += _Force * m_fInverseMass * TIME_DELTA * 10.0f;
		}

		void PointMass::increaseDamping(float _Factor)
		{
			m_fDamping *= _Factor;
		}


		void PointMass::update()
		{
			m_Velocity += m_Acceleration;
			m_Position += m_Velocity;
			m_Acceleration = sf::Vector3f(0.0f, 0.0f, 0.0f);

			if( math::Calc_NormSquared(m_Velocity) < EPSILON ) //Originally "0.001f * 0.001f"
				m_Velocity = sf::Vector3f(0.0f, 0.0f, 0.0f);

			m_Velocity *= m_fDamping;
			m_fDamping = POINT_MASS_DEFAULT_DAMPING;
		}
}