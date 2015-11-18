namespace sfgmk
{
	Sprite::Sprite()
	{
		m_isAnim = 0;
		m_Animation = 0;
	}

	Sprite::Sprite(std::string _resName, bool _isAnim)
	{
		sfgmk::engine::DataManager *dataman = DATA_MANAGER;

		m_isAnim = _isAnim;
		m_Animation = 0;

		if( m_isAnim )
			setAnimation(dataman->getAnimation(_resName));
		else
			sf::Sprite::setTexture(dataman->getTexture(_resName));
	}

	Sprite::~Sprite()
	{
		if( m_Animation )
			delete m_Animation;
	}

	void Sprite::setAnimation(Animation& _animation)
	{
		if( m_isAnim && m_Animation )
		{
			delete m_Animation;
			m_Animation = 0;
		}

		m_Animation = new Animation(_animation);

		m_isAnim = true;

		m_Animation->update(*this, 0.0f);
	}

	void Sprite::setRelativOrigin(float _rox, float _roy)
	{
		const sf::IntRect& rec = getTextureRect();

		return setOrigin(rec.width * _rox, rec.height * _roy);
	}

	sf::Vector2f Sprite::getCenter()
	{
		return sf::Vector2f(getTextureRect().width * 0.5f, getTextureRect().height * 0.5f);
	}

	sf::Vector2f Sprite::getSize()
	{
		return sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height);
	}

	void Sprite::FinalizeSprite(float _timeDelta)
	{
		if( m_isAnim && m_Animation )
			m_Animation->update(*this, _timeDelta);
	}

	Sprite& Sprite::finalize(float _timeDelta)
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

	void Sprite::Pause()
	{
		if (m_Animation)
			m_Animation->Pause();
	}

	void Sprite::Stop()
	{
		if (m_Animation)
			m_Animation->Stop();
	}

	void Sprite::Play()
	{
		if (m_Animation)
			m_Animation->Play();
	}
}