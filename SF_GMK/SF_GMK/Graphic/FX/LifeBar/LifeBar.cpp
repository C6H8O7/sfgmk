namespace sfgmk
{
	LifeBar::LifeBar(const sf::IntRect& _EntityRect, const bool& _DrawText) : m_bDrawPv(_DrawText), m_sPvString(""), m_fPvRatio(0.0f), m_Scale(sf::Vector2f(1.0f, 1.0f)), m_GaugeScale(sf::Vector2f(1.0f, 1.0f))
	{
		m_BarRect.setSize(sf::Vector2f((float)_EntityRect.width, 10.0f));
		m_BarRect.setOutlineThickness(2.0f);
		m_BarRect.setOutlineColor(sf::Color::Black);
		m_BarRect.setFillColor(sf::Color::White);

		m_GaugeRect.setOutlineThickness(0);
		m_GaugeRect.setSize(m_BarRect.getSize());

		m_PvFont = DATA_MANAGER->getFont("sfgmk_ConsoleFont1");
		m_PvText.setFont(m_PvFont);
		m_PvText.setCharacterSize(10);
		m_PvText.setColor(sf::Color::Black);
	}

	LifeBar::~LifeBar()
	{
	}


	void LifeBar::update(const int& _CurrentLife, const int& _InitialLife, const sf::Vector2f& _Origin, const sf::Vector2f& _Scale, const float& _Rotation, const sf::Vector2f& _Position)
	{
		//Transform
		setOrigin(_Origin);
		m_Scale.x = _Scale.x;
		setScale(m_Scale);
		setRotation(_Rotation);
		setPosition(_Position + sf::Vector2f(0.0f, -m_BarRect.getSize().y));

		m_fPvRatio = (float)_CurrentLife / (float)_InitialLife;
		m_GaugeScale.x = m_fPvRatio;

		m_PvText.setString(std::to_string(_CurrentLife) + "/" + std::to_string(_InitialLife));

		//Gauge color
		if( m_fPvRatio > 0.5f )
			m_GaugeRect.setFillColor(sf::Color::Green);
		else
		{
			if( m_fPvRatio > 0.25f )
				m_GaugeRect.setFillColor(sf::Color(255, 140, 0, 255));
			else
				m_GaugeRect.setFillColor(sf::Color::Red);
		}
	}

	void LifeBar::draw()
	{
		GRAPHIC_MANAGER->getRenderTexture()->draw(m_BarRect, getTransform());
		setScale(m_GaugeScale);
		GRAPHIC_MANAGER->getRenderTexture()->draw(m_GaugeRect, getTransform());

		if( m_bDrawPv )
		{
			setScale(1.0f, 1.0f);
			setPosition(getPosition() + sf::Vector2f(m_BarRect.getSize().x * m_Scale.x * 0.5f - m_PvText.getGlobalBounds().width * 0.5f, 0.0f));
			GRAPHIC_MANAGER->getRenderTexture()->draw(m_PvText, getTransform());
		}
	}


	bool LifeBar::setDrawPv()
	{
		m_bDrawPv = !m_bDrawPv;
		return m_bDrawPv;
	}

	void LifeBar::setDrawPv(const bool& _Boolean)
	{
		m_bDrawPv = _Boolean;
	}
}