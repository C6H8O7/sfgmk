namespace sfgmk
{
	namespace engine
	{
		AIManager::AIManager() : m_bDrawAiState(false)
		{
			sf::Vector2u WindowSize = GRAPHIC_MANAGER->getRenderWindow()->getSize();
			m_Render.create(WindowSize.x, WindowSize.y);

			m_Rect.setSize(sf::Vector2f(64.0f, 12.0f));
			m_Rect.setOutlineThickness(1.0f);
			m_Rect.setOutlineColor(sf::Color::Black);
			m_Rect.setFillColor(sf::Color(255, 255, 255, 225));

			m_Font = DATA_MANAGER->getFont("sfgmk_ConsoleFont1");
			m_Text.setFont(m_Font);
			m_Text.setCharacterSize(12);
			m_Text.setColor(sf::Color::Black);
		}

		AIManager::~AIManager()
		{
		}


		void AIManager::update(const float& _TimeDelta)
		{
			m_Render.clear(EMPTY_COLOR);
			m_Render.setView(*CAMERA);
		}

		void AIManager::draw()
		{
			if( m_bDrawAiState )
			{
				std::vector<Entity*>* Entities = &ENTITY_MANAGER->getEntityVector();
				AIStateMachine* TempAi = NULL;
				sf::Vector2f Position;

				for( std::vector<Entity*>::iterator it = (*Entities).begin(); it != (*Entities).end(); ++it )
				{
					TempAi = (*it)->getAiFsm();

					if( TempAi )
					{
						Position = (*it)->getPosition() + sf::Vector2f(0.0f, (*it)->getSprite()->getSize().y);

						m_Rect.setPosition(Position);
						m_Render.draw(m_Rect);

						m_Text.setString(TempAi->getCurrentStateName() + " " + TempAi->getCurrentProgressString());
						m_Text.setPosition(Position + sf::Vector2f(1.0f, 1.0f));
						m_Render.draw(m_Text);
					}
				}

				m_Render.display();
				m_Sprite.setTexture(m_Render.getTexture());

				m_Sprite.setPosition(CAMERA->getRelativOrigin());

				float scale = 1.0f / CAMERA->getZoomFactor();
				m_Sprite.setScale(scale, scale);

				GRAPHIC_MANAGER->getRenderTexture()->draw(m_Sprite);
			}
		}


		void AIManager::setDrawAiState()
		{
			m_bDrawAiState = !m_bDrawAiState;
		}
	}
}