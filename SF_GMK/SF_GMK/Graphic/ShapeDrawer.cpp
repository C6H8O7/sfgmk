namespace sfgmk
{
	ShapeDrawer::ShapeDrawer()
	{
	}

	ShapeDrawer::~ShapeDrawer()
	{
	}


	void ShapeDrawer::setRenderTexture(sf::RenderTexture* _Render)
	{
		m_GraphicManagerRenderTexturePtr = _Render;
	}


	void ShapeDrawer::drawLine(const sf::Vertex Points[2])
	{
		m_GraphicManagerRenderTexturePtr->draw(Points, 2, sf::Lines);

	}

	void ShapeDrawer::drawLine(const sf::Vertex Points[2], sf::RenderTexture* _Render)
	{
		_Render->draw(Points, 2, sf::Lines);
	}

	void ShapeDrawer::drawCircle(const sf::Vector2f& _Position, const float& _Radius, const unsigned short& _PointCount, 
								 const sf::Color& _FilLColor, const float& _OutlineThickness, const sf::Color& _OutlineColor, const sf::Texture* _texture, const sf::IntRect& _TextureRect)
	{
		m_Circle.setRadius(_Radius);
		m_Circle.setPointCount(_PointCount);
		m_Circle.setFillColor(_FilLColor);
		m_Circle.setOutlineThickness(_OutlineThickness);
		m_Circle.setOutlineColor(_OutlineColor);
		m_Circle.setTexture(_texture);
		m_Circle.setTextureRect(_TextureRect);
	
		m_Circle.setPosition(_Position);
		m_GraphicManagerRenderTexturePtr->draw(m_Circle);
	}

	void ShapeDrawer::drawCircle(const sf::Vector2f& _Position, const float& _Radius, const unsigned short& _PointCount, sf::RenderTexture* _Render,
					const sf::Color& _FilLColor, const float& _OutlineThickness, const sf::Color& _OutlineColor, const sf::Texture* _texture, const sf::IntRect& _TextureRect)
	{
		m_Circle.setRadius(_Radius);
		m_Circle.setPointCount(_PointCount);
		m_Circle.setFillColor(_FilLColor);
		m_Circle.setOutlineThickness(_OutlineThickness);
		m_Circle.setOutlineColor(_OutlineColor);
		m_Circle.setTexture(_texture);
		m_Circle.setTextureRect(_TextureRect);

		m_Circle.setPosition(_Position);
		_Render->draw(m_Circle);
	}

	void ShapeDrawer::drawRectangle(const sf::Vector2f& _Position, const sf::Vector2f& _Size, 
									const sf::Color& _FilLColor, const float& _OutlineThickness, const sf::Color& _OutlineColor, const sf::Texture* _texture, const sf::IntRect& _TextureRect)
	{
		m_Rectangle.setSize(_Size);
		m_Rectangle.setFillColor(_FilLColor);
		m_Rectangle.setOutlineThickness(_OutlineThickness);
		m_Rectangle.setOutlineColor(_OutlineColor);
		m_Rectangle.setTexture(_texture);
		m_Rectangle.setTextureRect(_TextureRect);
		
		m_Rectangle.setPosition(_Position);
		m_GraphicManagerRenderTexturePtr->draw(m_Rectangle);
	}

	void ShapeDrawer::drawRectangle(const sf::Vector2f& _Position, const sf::Vector2f& _Size, sf::RenderTexture* _Render,
					   const sf::Color& _FilLColor, const float& _OutlineThickness, const sf::Color& _OutlineColor, const sf::Texture* _texture, const sf::IntRect& _TextureRect)
	{
		m_Rectangle.setSize(_Size);
		m_Rectangle.setFillColor(_FilLColor);
		m_Rectangle.setOutlineThickness(_OutlineThickness);
		m_Rectangle.setOutlineColor(_OutlineColor);
		m_Rectangle.setTexture(_texture);
		m_Rectangle.setTextureRect(_TextureRect);

		m_Rectangle.setPosition(_Position);
		_Render->draw(m_Rectangle);
	}
}