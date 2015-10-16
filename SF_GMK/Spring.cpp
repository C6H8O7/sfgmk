namespace sfgmk
{
	Spring::Spring(PointMass* _End1, PointMass* _End2, float _Stiffness, float _Damping)
		: m_End1(_End1), m_End2(_End2), m_fStiffness(_Stiffness), m_fDamping(_Damping), m_bBehind(false)
	{
		//When we create a spring, we set the natural length of the spring to be just slightly less than the distance between the two end points. 
		//This keeps the grid taut even when at rest, and improves the appearance somewhat.
		m_fTargetLength = Calc_Distance(m_End1->m_Position, m_End2->m_Position) * 0.95f; 
	}

	void Spring::update()
	{
		sf::Vector3f Distance = m_End1->m_Position - m_End2->m_Position;
		float fLength = Calc_Norm(Distance);

		if( fLength > m_fTargetLength )
		{
			Distance = (Distance / fLength) * (fLength - m_fTargetLength);
			sf::Vector3f dv = m_End2->m_Velocity - m_End1->m_Velocity;
			sf::Vector3f force = m_fStiffness * Distance - dv * m_fDamping;

			m_End1->applyForce(-force);
			m_End2->applyForce(force);
		}
	}
}