namespace sfgmk
{
	Bloom::Bloom(const sf::Vector2u _Size, float _SizeDownScale, float _EffectLevel, float _SaturateLevel, sf::Vector2f _BlurOffset)
		: m_Size(_Size), m_fSizeDownScale(_SizeDownScale), m_fEffectLevel(_EffectLevel), m_fSaturateLevel(_SaturateLevel), m_BlurOffset(_BlurOffset)
	{
		
		m_BrightnessShader.loadFromFile(SFGMK_DATA_PATH + "shader/saturate.frag", sf::Shader::Fragment);
		m_BlurShader.loadFromFile(SFGMK_DATA_PATH + "shader/gaussianBlur.frag", sf::Shader::Fragment);
		m_AddShader.loadFromFile(SFGMK_DATA_PATH + "shader/add.frag", sf::Shader::Fragment);
	
		m_TempTexture1.create(m_Size.x, m_Size.y);
		m_TempTexture1.setSmooth(true);
		m_TempTexture2.create((unsigned int)(m_Size.x * m_fSizeDownScale), (unsigned int)(m_Size.y * m_fSizeDownScale));
	}

	Bloom::~Bloom()
	{
	}


	void Bloom::applyEffect(sf::RenderTexture& _Source)
	{
		//Saturate
		m_BrightnessShader.setParameter("_Texture", _Source.getTexture());
		m_BrightnessShader.setParameter("_SaturationFloor", m_fSaturateLevel);

		PostShader::applyPostShader(m_BrightnessShader, m_TempTexture1);
		m_TempTexture1.display();

		//Blur H
		m_BlurShader.setParameter("_Texture", m_TempTexture1.getTexture());
		m_BlurShader.setParameter("_Offset", sf::Vector2f(m_BlurOffset.x, 0.0f));

		PostShader::applyPostShader(m_BlurShader, m_TempTexture2);
		m_TempTexture2.display();

		//Blur V
		m_BlurShader.setParameter("_Texture", m_TempTexture2.getTexture());
		m_BlurShader.setParameter("_Offset", sf::Vector2f(0.0f, m_BlurOffset.y));

		PostShader::applyPostShader(m_BlurShader, m_TempTexture2);
		m_TempTexture2.display();

		//Add
		m_AddShader.setParameter("_TextureSource", _Source.getTexture());
		m_AddShader.setParameter("_TextureEffect", m_TempTexture2.getTexture());
		m_AddShader.setParameter("_EffectLevel", m_fEffectLevel);

		PostShader::applyPostShader(m_AddShader, _Source);
		_Source.display();
	}
}