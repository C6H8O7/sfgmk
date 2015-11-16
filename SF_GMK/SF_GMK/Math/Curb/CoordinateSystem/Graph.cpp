namespace sfgmk
{
	namespace math
	{
		Graph::Graph(sf::Vector2u _Units, sf::Vector2f _UnitValue) : m_UnitNumber(_Units), m_UnitValue(_UnitValue)
		{
			m_ScreenSize = (sf::Vector2f)GRAPHIC_MANAGER->getRenderTexture()->getSize();

			m_Repere[0] = sf::Vector2f(0.0f, m_ScreenSize.y * 0.5f);
			m_Repere[1] = sf::Vector2f(m_ScreenSize.x, m_ScreenSize.y * 0.5f);
			m_Repere[2] = sf::Vector2f(m_ScreenSize.x * 0.5f, 0.0f);
			m_Repere[3] = sf::Vector2f(m_ScreenSize.x * 0.5f, m_ScreenSize.y);

			m_Font.loadFromFile("data/sfgmk/console/fontSquare.ttf");
			m_Text.setFont(m_Font);
			m_Text.setCharacterSize(12);
			m_Text.setColor(sf::Color::White);
		}

		Graph::~Graph()
		{
			m_Curbs.deleteAndClear();
		}


		void Graph::draw()
		{
			sf::RenderTexture* RenderTexture = GRAPHIC_MANAGER->getRenderTexture();

			sf::Vertex Line[2];
			char cBuffer[16];
			std::string sString;
			float fUnit(0.0f);

			//Repère
			Line[0] = m_Repere[0];
			Line[1] = m_Repere[1];
			RenderTexture->draw(Line, 2, sf::Lines);

			Line[0] = m_Repere[2];
			Line[1] = m_Repere[3];
			RenderTexture->draw(Line, 2, sf::Lines);

			//Unités
			m_Text.setCharacterSize(12);
			m_Text.setColor(sf::Color::White);

			//X
			for( unsigned int i(0); i < m_UnitNumber.x; i++ )
			{
				Line[0] = sf::Vector2f(i * (m_ScreenSize.x / m_UnitNumber.x), m_ScreenSize.y * 0.5f + 5.0f);
				Line[1] = sf::Vector2f(i * (m_ScreenSize.x / m_UnitNumber.x), m_ScreenSize.y * 0.5f - 5.0f);
				RenderTexture->draw(Line, 2, sf::Lines);

				i < m_UnitNumber.x * 0.5f ? fUnit = -((m_UnitNumber.x * 0.5f) - i) * m_UnitValue.x : fUnit = i * m_UnitValue.x - (m_UnitNumber.x * 0.5f * m_UnitValue.x);

				sprintf(cBuffer, "%f", fUnit);
				sString = cBuffer;
				sString = sString.substr(0, sString.find('.') + 3);
				m_Text.setString(sString);

				m_Text.setPosition(sf::Vector2f(i * (m_ScreenSize.x / m_UnitNumber.x) - 15.0f, m_ScreenSize.y * 0.5f + 5.0f));
				RenderTexture->draw(m_Text);

			}
			//Y
			for( unsigned int i(0); i < m_UnitNumber.y; i++ )
			{
				Line[0] = sf::Vector2f(m_ScreenSize.x * 0.5f + 5.0f, i * (m_ScreenSize.y / m_UnitNumber.y));
				Line[1] = sf::Vector2f(m_ScreenSize.x * 0.5f - 5.0f, i * (m_ScreenSize.y / m_UnitNumber.y));
				RenderTexture->draw(Line, 2, sf::Lines);

				i < m_UnitNumber.y * 0.5f ? fUnit = ((m_UnitNumber.y * 0.5f) - i) * m_UnitValue.y : fUnit = -(i * m_UnitValue.y - (m_UnitNumber.y * 0.5f * m_UnitValue.y));

				sprintf(cBuffer, "%f", fUnit);
				sString = cBuffer;
				sString = sString.substr(0, sString.find('.') + 3);
				m_Text.setString(sString);

				m_Text.setPosition(sf::Vector2f(i * (m_ScreenSize.x / m_UnitNumber.x) - 15.0f, m_ScreenSize.y * 0.5f + 5.0f));
				m_Text.setPosition(sf::Vector2f(m_ScreenSize.x * 0.5f - 35.0f, i * (m_ScreenSize.y / m_UnitNumber.y) - 7.0f));
				RenderTexture->draw(m_Text);
			}

			//Courbes
			for( unsigned int i(0); i < m_Curbs.getElementNumber(); i++ )
			{
				sf::Color CurbColor = m_Curbs[i]->getColor();

				//Tracé
				for( unsigned int j(0); j < m_Curbs[i]->getValues().getElementNumber() - 1; j++ )
				{
					sf::Vector2f Point1 = sf::Vector2f(m_Curbs[i]->getValues()[j].x / m_UnitValue.x * (m_ScreenSize.x / m_UnitNumber.x), -m_Curbs[i]->getValues()[j].y / m_UnitValue.y * (m_ScreenSize.y / m_UnitNumber.y)) + m_ScreenSize * 0.5f;
					sf::Vector2f Point2 = sf::Vector2f(m_Curbs[i]->getValues()[j + 1].x / m_UnitValue.x * (m_ScreenSize.x / m_UnitNumber.x), -m_Curbs[i]->getValues()[j + 1].y / m_UnitValue.y * (m_ScreenSize.y / m_UnitNumber.y)) + m_ScreenSize * 0.5f;

					Line[0] = Point1;
					Line[1] = Point2;

					Line[0].color = CurbColor;
					Line[1].color = CurbColor;

					RenderTexture->draw(Line, 2, sf::Lines);
				}

				//Nom de la courbe
				m_Text.setColor(CurbColor);
				m_Text.setString(m_Curbs[i]->getName());
				m_Text.setCharacterSize(16);
				m_Text.setPosition(sf::Vector2f(15.0f, i * 15.0f));
				RenderTexture->draw(m_Text);
			}
		}


		void Graph::addCurb(GraphCurb* _NewCurb)
		{
			m_Curbs.pushBack(_NewCurb);
		}
	}
}