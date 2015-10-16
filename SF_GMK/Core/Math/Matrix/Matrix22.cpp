namespace sfgmk
{
	Matrix22::Matrix22()
	{
		m_fArray[0] = 0.0f;
		m_fArray[1] = 0.0f;
		m_fArray[2] = 0.0f;
		m_fArray[3] = 0.0f;
	}

	Matrix22::Matrix22(float _Value1, float _Value2, float _Value3, float _Value4)
	{
		m_fArray[0] = _Value1;
		m_fArray[1] = _Value2;
		m_fArray[2] = _Value3;
		m_fArray[3] = _Value4;
	}

	Matrix22::Matrix22(float _Value[4])
	{
		m_fArray[0] = _Value[0];
		m_fArray[1] = _Value[1];
		m_fArray[2] = _Value[2];
		m_fArray[3] = _Value[3];
	}

	Matrix22::~Matrix22()
	{
	}


	sf::Vector2f Matrix22::operator*(sf::Vector2f& _Vector2)
	{
		float fX = _Vector2.x * m_fArray[0] + _Vector2.y * m_fArray[1];
		float fY = _Vector2.x * m_fArray[2] + _Vector2.y * m_fArray[3];

		return sf::Vector2f(fX, fY);
	}
}