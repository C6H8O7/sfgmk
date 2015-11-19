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

		void AIManager::draw(sf::RenderTexture* _render)
		{
			if( m_bDrawAiState )
			{
				std::vector<Entity*>* Entities = &ENTITY_MANAGER->getEntityVector();
				AIStateMachine* TempAi = NULL;
				sf::Vector2f Position;
				float fSizeRatio(0.0f);
				float fDecal(0.0f);

				for( std::vector<Entity*>::iterator it = (*Entities).begin(); it != (*Entities).end(); ++it )
				{
					TempAi = (*it)->getAiFsm();

					if( TempAi )
					{ 
						fSizeRatio = (*it)->getSprite()->getSize().x / m_Rect.getSize().x;
						fDecal = ABS((*it)->getSprite()->getSize().x - m_Rect.getSize().x) * 0.5f;
						if( fSizeRatio < 1.0f )
							fDecal = -fDecal;

						Position = (*it)->getPosition() + sf::Vector2f(fDecal, (*it)->getSprite()->getSize().y);

						m_Rect.setOrigin((*it)->getOrigin());
						m_Rect.setPosition(Position);
						_render->draw(m_Rect);

						m_Text.setOrigin((*it)->getOrigin());
						m_Text.setString(TempAi->getCurrentStateName() + " " + TempAi->getCurrentProgressString());
						m_Text.setPosition(Position + sf::Vector2f(1.0f, 1.0f));
						_render->draw(m_Text);
					}
				}
			}
		}


		void AIManager::setDrawAiState()
		{
			m_bDrawAiState = !m_bDrawAiState;
		}
	}
}