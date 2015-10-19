namespace sfgmk
{
	namespace math
	{
		Bezier::Bezier(sf::Vector3f _Origin, sf::Vector3f Arrival, sf::Vector3f _ControlPoint1, sf::Vector3f _ControlPoint2, float _Time) : Curb(_Time)
		{
			m_Points = new sf::Vector3f[4];
			m_Points[0] = _Origin;
			m_Points[1] = _ControlPoint1;
			m_Points[2] = _ControlPoint2;
			m_Points[3] = Arrival;
		}

		Bezier::Bezier(sf::Vector3f _Points[4], float _Time) : Bezier(_Points[0], _Points[3], _Points[1], _Points[2], _Time)
		{
		}

		Bezier::~Bezier()
		{
		}


		void Bezier::update(float _TimeDelta)
		{
			m_fCurrentTime += _TimeDelta;
			m_fTimeRatio = m_fCurrentTime / m_fTotalTime;

			//Update coordonnees
			m_fTimeRatio < 1.0f ? computeCoords() : m_CurrentPos = m_Points[3];
		}


		void Bezier::computeCoords()
		{
			m_CurrentPos.x = ((1 - m_fTimeRatio) * (1 - m_fTimeRatio) * (1 - m_fTimeRatio)) * m_Points[0].x;
			m_CurrentPos.x += (3 * m_fTimeRatio * (1 - m_fTimeRatio) * (1 - m_fTimeRatio)) * m_Points[1].x;
			m_CurrentPos.x += (3 * m_fTimeRatio * m_fTimeRatio * (1 - m_fTimeRatio)) * m_Points[2].x;
			m_CurrentPos.x += CUBE(m_fTimeRatio) * m_Points[3].x;

			m_CurrentPos.y = ((1 - m_fTimeRatio) * (1 - m_fTimeRatio) * (1 - m_fTimeRatio)) * m_Points[0].y;
			m_CurrentPos.y += (3 * m_fTimeRatio * (1 - m_fTimeRatio) * (1 - m_fTimeRatio)) * m_Points[1].y;
			m_CurrentPos.y += (3 * m_fTimeRatio * m_fTimeRatio * (1 - m_fTimeRatio)) * m_Points[2].y;
			m_CurrentPos.y += CUBE(m_fTimeRatio) * m_Points[3].y;

			m_CurrentPos.z = ((1 - m_fTimeRatio) * (1 - m_fTimeRatio) * (1 - m_fTimeRatio)) * m_Points[0].z;
			m_CurrentPos.z += (3 * m_fTimeRatio * (1 - m_fTimeRatio) * (1 - m_fTimeRatio)) * m_Points[1].z;
			m_CurrentPos.z += (3 * m_fTimeRatio * m_fTimeRatio * (1 - m_fTimeRatio)) * m_Points[2].z;
			m_CurrentPos.z += CUBE(m_fTimeRatio) * m_Points[3].z;
		}
	}
}