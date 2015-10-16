namespace sfgmk
{
	Sprite::Sprite()
	{
		m_isAnim = 0;
		m_Animation = 0;
	}

	Sprite::Sprite(std::string _resName, bool _isAnim)
	{
		sfgmk::DataManager *dataman = sfgmk::DataManager::getSingleton();

		m_isAnim = _isAnim;
		m_Animation = 0;

		if( m_isAnim )
			setAnimation(sfgmk::DataManager::getSingleton()->getAnimation(_resName));
		else
			sf::Sprite::setTexture(dataman->getTexture(_resName));
	}

	Sprite::~Sprite()
	{
		if( m_Animation )
			delete m_Animation;
	}

	void Sprite::setAnimation(sfgmk::Animation& _animation)
	{
		if( m_isAnim && m_Animation )
		{
			delete m_Animation;
			m_Animation = 0;
		}

		m_Animation = new sfgmk::Animation(_animation);

		m_isAnim = true;

		m_Animation->update(*this, 0.0f);
	}

	void Sprite::setRelativeOrigin(float _rox, float _roy)
	{
		sf::FloatRect rec = getLocalBounds();

		return setOrigin(rec.width * _rox, rec.height * _roy);
	}

	sf::Vector2f Sprite::getCenter()
	{
		return sf::Vector2f(getTextureRect().width * 0.5f, getTextureRect().height * 0.5f);
	}

	void Sprite::FinalizeSprite(float _timeDelta)
	{
		if( m_isAnim && m_Animation )
			m_Animation->update(*this, _timeDelta);
	}

	Sprite& sfgmk::Sprite::finalize(float _timeDelta)
	{
		FinalizeSprite(_timeDelta);

		return *this;
	}

	void Sprite::SetLoop(bool _loop)
	{
		if( m_Animation )
			m_Animation->SetLoop(_loop);
	}

	bool Sprite::GetLoop()
	{
		if( m_Animation )
			return m_Animation->GetLoop();

		return false;
	}
}