namespace sfgmk
{
	FxText::FxText(std::string _String, sf::Font _Font, unsigned int _Size, sf::Color _Color) : m_Render(NULL), m_TextRender(NULL), m_Color(_Color)
	{
		sf::Text Text(_String, _Font, _Size);
		Text.setColor(m_Color);
		m_Size = sf::Vector2u(unsigned int(Text.getLocalBounds().width), unsigned int(Text.getLocalBounds().height));

		m_TextRender = new sf::RenderTexture();
		m_TextRender->create(m_Size.x, m_Size.y);
		m_TextRender->clear(EMPTY_COLOR);
		m_TextRender->draw(Text);
		m_TextRender->display();

		sf::Image TextImage(m_TextRender->getTexture().copyToImage());
		TextImage.createMaskFromColor(EMPTY_COLOR);

		for( unsigned int i(0); i < m_Size.x; i++ )
		{
			for( unsigned int j(0); j < m_Size.y; j++ )
			{
				if( TextImage.getPixel(i, j).a != EMPTY_COLOR.a )
					m_PixelArray.pushBack(sf::Vector2f((float)i, (float)j));
			}
		}
	
		m_Render = new sf::RenderTexture();
		m_Render->create(m_Size.x, m_Size.y);
		m_Render->clear();
	}

	FxText::~FxText()
	{
		m_PixelArray.clear();

		SAFE_DELETE(m_Render);
		SAFE_DELETE(m_TextRender);
	}
}