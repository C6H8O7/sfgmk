namespace sfgmk
{
	GraphCurb::GraphCurb(const std::string& _Name, const sf::Color& _Color) : m_Name(_Name), m_Color(_Color)
	{
	}

	GraphCurb::~GraphCurb()
	{
		m_Values.clear();
	}


	void GraphCurb::addValue(const sf::Vector2f& _NewValue)
	{
		m_Values.pushBack(_NewValue);
	}

	sfgmk::DynamicArray<sf::Vector2f>& GraphCurb::getValues()
	{
		return m_Values;
	}


	const std::string& GraphCurb::getName()
	{
		return m_Name;
	}

	const sf::Color& GraphCurb::getColor()
	{
		return m_Color;
	}
}