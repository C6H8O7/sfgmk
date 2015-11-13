namespace sfgmk
{
	FloatingDamage::FloatingDamage(int _Value, int _InitialPv, Entity* _Target) : m_fAngle(0), m_fAlpha(255)
	{
		//Texte
		m_Text.setString(std::to_string(_Value));
		m_Text.setFont(DATA_MANAGER->getFont("sfgmk_FontFloatingDamage"));

		//Selon le % de PV perdu, on change l'affichage des dégats
		float fRatio = (float)_InitialPv / (float)_Value;
		if( fRatio >= 0.5f )
		{
			m_Text.setColor(sf::Color::Red);
			m_Text.setCharacterSize(120);
		}
		else if( fRatio >= 0.25f )
		{
			m_Text.setColor(sf::Color(255, 140, 0, 255));
			m_Text.setCharacterSize(60);
		}
		else
		{
			m_Text.setColor(sf::Color::White);
			m_Text.setCharacterSize(30);
		}

		//Position initiale
		setPosition(_Target->getPosition3D() + sf::Vector3f(0.0f, 0.0f, -1.0f)); //Permet au texte dêtre devant l'entité
		m_Text.setPosition(getPosition());
	}

	FloatingDamage::~FloatingDamage()
	{
	}


	void FloatingDamage::update(const float& _TimeDelta)
	{
		//Update position
		if( m_fAngle < PI )
		{
			m_fAngle += _TimeDelta * 10.0f;
			m_Text.setPosition(sf::Vector2f(getPosition().x + m_fAngle * 15.0f, getPosition().y - sin(m_fAngle) * 20.0f)); //X et Y augmentent avec l'angle ==> trajectoire en cloche
		}

		//Update alpha + scale
		m_fAlpha -= TIME_DELTA * 200.0f;
		m_Text.setColor(sf::Color(m_Text.getColor().r, m_Text.getColor().g, m_Text.getColor().b, (sf::Uint8)m_fAlpha));
		m_Text.setScale(m_fAlpha / (float)255, m_fAlpha / (float)255);

		//Destruction
		if( m_fAlpha <= 0.0f )
			setIsAlive(false);
	}

	void FloatingDamage::draw(sf::RenderTexture* _Render)
	{
		_Render->draw(m_Text);
	}
}