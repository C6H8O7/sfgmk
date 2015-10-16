namespace sfgmk
{
	Curb::Curb(float _Duration) : m_fTotalTime(_Duration), m_fCurrentTime(0.0f), m_fTimeRatio(0.0f)
	{
	}

	Curb::~Curb()
	{
		delete[] m_Points;
	}


	sf::Vector3f* Curb::getPoints()
	{
		return m_Points;
	}
	sf::Vector3f Curb::getCurrentPosition()
	{
		return m_CurrentPos;
	}
	float Curb::getTotalTime()
	{
		return m_fTotalTime;
	}
	float Curb::getCurrentTime()
	{
		return m_fCurrentTime;
	}
	float Curb::getTimeRatio()
	{
		return m_fTimeRatio;
	}
}