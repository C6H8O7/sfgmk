namespace sfgmk
{
	BlackHole::BlackHole() : m_Grid(NULL), m_fSprayAngle(0.0f)
	{
	}

	BlackHole::~BlackHole()
	{
	}


	void BlackHole::update(const float& _TimeDelta)
	{
		m_fSprayAngle += 120.0f * _TimeDelta;
		move(sf::Vector2f(25.0f, 25.0f) * _TimeDelta);

		math::Matrix22 rot(cos(m_fSprayAngle), -sin(m_fSprayAngle), sin(m_fSprayAngle), cos(m_fSprayAngle));
		sf::Vector2f finalV = rot * sf::Vector2f(50.0f, 50.0f);

		m_Grid->applyImplosiveForce((float)sinf(m_fSprayAngle / 2.0f) * 10.0f + 20.0f, getPosition3D() + sf::Vector3f(finalV.x, finalV.y, 0.0f), 200.0f);
	}


	void BlackHole::setGrid(Grid* _Grid)
	{
		m_Grid = _Grid;
	}
}