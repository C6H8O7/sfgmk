namespace sfgmk
{
	Animation::Animation()
	{
		m_fTimer = 0.0f;
		m_iCurrentFrame = 0;
		m_isLoop = true;
	}

	Animation::Animation(Animation& _copy)
	{
		m_isLoop = _copy.m_isLoop;
		m_fTimer = _copy.m_fTimer;
		m_iCurrentFrame = _copy.m_iCurrentFrame;

		m_Frames = _copy.m_Frames;
	}

	Animation::~Animation()
	{

	}

	void Animation::update(Sprite& _sprite, float _timeDelta)
	{
		int currentFrame = m_iCurrentFrame;

		m_fTimer += _timeDelta;

		while( m_fTimer >= m_Frames[m_iCurrentFrame]->duration )
		{
			m_fTimer -= m_Frames[m_iCurrentFrame++]->duration;

			if( m_iCurrentFrame >= (int)m_Frames.size() )
			{
				if( m_isLoop )
					m_iCurrentFrame = 0;
				else
					m_iCurrentFrame = (int)m_Frames.size() - 1;
			}
		}

		if( m_iCurrentFrame != currentFrame || m_fTimer == 0.0f )
			_sprite.sf::Sprite::setTexture(*m_Frames[m_iCurrentFrame]->texture, true);
	}

	void Animation::FreeTextures()
	{
		for( int i = (int)m_Frames.size() - 1; i >= 0; i-- )
		{
			AnimationFrame *frame = m_Frames[i];

			if( frame )
			{
				if( frame->texture )
				{
					delete frame->texture;
					frame->texture = 0;
				}
				delete m_Frames[i];
			}

			m_Frames[i] = m_Frames[m_Frames.size() - 1];
			m_Frames.pop_back();
		}
	}

	bool Animation::AddFrame(std::string _filePath, float _duration, sf::IntRect _rectangle)
	{
		sf::Texture *texture = new sf::Texture;

		bool result = texture->loadFromFile(_filePath, _rectangle);

		if( result )
		{
			AnimationFrame *frame = new AnimationFrame;

			frame->texture = texture;
			frame->duration = _duration;

			m_Frames.push_back(frame);
		}
		else
			delete texture;

		return result;
	}

	void Animation::SetLoop(bool _loop)
	{
		m_isLoop = _loop;
	}

	bool Animation::GetLoop()
	{
		return m_isLoop;
	}

	void Animation::LoadFromFile(std::string _file)
	{
		Parser parser(_file);

		int isSpriteSheet = parser.retrieveContent("isSpriteSheet").getInt();
		int nbFrames = parser.retrieveContent("nbFrames").getInt();

		if( parser.retrieveContent("isLoop").getInt() )
			m_isLoop = true;
		else
			m_isLoop = false;

		if( isSpriteSheet )
		{
			ParserContent spriteSheetConfig = parser.retrieveContent("spriteSheetConfig");

			int width = spriteSheetConfig.getInt();
			int height = spriteSheetConfig.getInt();
			int nbWidth = spriteSheetConfig.getInt();
			int nbHeight = spriteSheetConfig.getInt();

			int w = width / nbWidth;
			int h = height / nbHeight;

			ParserContent frames = parser.retrieveContent("frames");

			for( int i = 0; i<nbFrames; i++ )
			{
				ParserContent frame = frames.getLineAsContent();

				std::string filePath = frame.getString();
				float duration = frame.getFloat();

				int x = i%nbWidth;
				int y = i / nbWidth;

				AddFrame(filePath, duration, sf::IntRect(x * w, y * h, w, h));
			}
		}
		else
		{
			ParserContent frames = parser.retrieveContent("frames");

			for( int i = 0; i<nbFrames; i++ )
			{
				ParserContent frame = frames.getLineAsContent();

				std::string filePath = frame.getString();
				float duration = frame.getFloat();

				AddFrame(filePath, duration);
			}
		}
	}
}