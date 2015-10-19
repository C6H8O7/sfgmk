#ifndef GRAPH_CURB_HPP
#define GRAPH_CURB_HPP


namespace sfgmk
{
	namespace math
	{
		class GraphCurb
		{
			private:
				sfgmk::DynamicArray<sf::Vector2f> m_Values;
				std::string m_Name;
				sf::Color m_Color;

			public:
				GraphCurb(const std::string& _Name, const sf::Color& _Color = sf::Color::Blue);
				~GraphCurb();

				void addValue(const sf::Vector2f& _NewValue);
				sfgmk::DynamicArray<sf::Vector2f>& getValues();

				const std::string& getName();
				const sf::Color& getColor();
		};
	}
}



#endif