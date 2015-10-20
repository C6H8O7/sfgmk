#ifndef GRAPH_HPP
#define GRAPH_HPP


namespace sfgmk
{
	namespace math
	{
		class SFGMK_API Graph
		{
			private:
				sfgmk::DynamicArray<GraphCurb*> m_Curbs;

				sf::Vector2f m_Repere[4];
				sf::Vector2u m_UnitNumber;
				sf::Vector2f m_ScreenSize;
				sf::Vector2f m_UnitValue;

				sf::Font m_Font;
				sf::Text m_Text;

			public:
				Graph(sf::Vector2u _Units = sf::Vector2u(10U, 10U), sf::Vector2f _UnitValue = sf::Vector2f(1.0f, 1.0f));
				~Graph();

				void draw();

				void addCurb(GraphCurb* _NewCurb);
		};
	}
}



#endif