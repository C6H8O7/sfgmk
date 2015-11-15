namespace sfgmk
{
	namespace engine
	{
		EntityManager::EntityManager() : m_uiEntityAccount(0U), m_uiEntityGlobalAccount(0U), m_llSortTimer(sf::Int64(0.0)), m_bDrawId(false)
		{
			sf::Vector2u WindowSize = GRAPHIC_MANAGER->getRenderWindow()->getSize();
			m_Render.create(WindowSize.x, WindowSize.y);

			m_Rect.setSize(sf::Vector2f(32.0f, 12.0f));
			m_Rect.setOutlineThickness(1.0f);
			m_Rect.setOutlineColor(sf::Color::Black);
			m_Rect.setFillColor(sf::Color(255, 255, 255, 200));

			m_Font = DATA_MANAGER->getFont("sfgmk_ConsoleFont1");
			m_Text.setFont(m_Font);
			m_Text.setCharacterSize(12);
			m_Text.setColor(sf::Color::Black);
		}

		EntityManager::~EntityManager()
		{
			freeEntityVector();
		}


		void EntityManager::update()
		{	
			for( int i(m_EntityVector.size() - 1); i >= 0; i-- )
			{
				m_EntityVector[i]->update(TIME_DELTA);
			
				//Suppression entités en fin de vie
				if( !m_EntityVector[i]->getIsAlive() )
					removeEntity(i);
			}

			m_Render.clear(EMPTY_COLOR);
			m_Render.setView(*CAMERA);
		}


		void EntityManager::draw()
		{
			if( m_bDrawId )
			{
				sf::Vector2f Position;
				unsigned int uiId;

				for( Entity*& entity : m_EntityVector )
				{
					uiId = entity->getSingleId();
					Position = entity->getPosition() + sf::Vector2f(-m_Rect.getSize().x, entity->getSprite()->getSize().y * 0.25f);

					m_Rect.setPosition(Position);
					m_Render.draw(m_Rect);

					m_Text.setString(std::to_string(uiId));
					m_Text.setPosition(Position + sf::Vector2f(1.0f, 1.0f));
					m_Render.draw(m_Text);
				}

				m_Render.display();
				m_Sprite.setTexture(m_Render.getTexture());

				m_Sprite.setPosition(CAMERA->getRelativOrigin());

				float scale = 1.0f / CAMERA->getZoomFactor();
				m_Sprite.setScale(scale, scale);

				GRAPHIC_MANAGER->getRenderTexture()->draw(m_Sprite);
			}
		}


		void EntityManager::setDrawId()
		{
			m_bDrawId = !m_bDrawId;
		}


		Entity* EntityManager::getEntity(const unsigned int& _Id)
		{
			for( Entity*& entity : m_EntityVector )
			{
				if( entity->getSingleId() == _Id )
					return entity;
			}

			perror("Single ID of Entity not found");
			return NULL;
		}

		std::vector<Entity*>& EntityManager::getEntityVector()
		{
			return m_EntityVector;
		}

		const unsigned int& EntityManager::getEntityNumber()
		{
			return m_uiEntityAccount;
		}

		void EntityManager::addEntity(Entity* _Entity)
		{
			_Entity->setSingleId(m_uiEntityGlobalAccount);
			m_uiEntityGlobalAccount++;

			m_EntityVector.push_back(_Entity);
			m_uiEntityAccount++;
		}

		void EntityManager::removeEntity(const unsigned int& _Index)
		{
			DeletePtrCntrElement(m_EntityVector, _Index);
			m_uiEntityAccount--;
		}


		void EntityManager::freeEntityVector()
		{
			ClearPtrCntr(m_EntityVector);
		}


		void EntityManager::sortEntityVector()
		{
			m_SortTimer.restart();
			std::sort(m_EntityVector.begin(), m_EntityVector.end(), sortEntityByZ);
			m_llSortTimer = m_SortTimer.getElapsedTime().asMicroseconds();
		}


		inline bool sortEntityByZ(Entity* _Entity1, Entity* _Entity2)
		{
			return(_Entity1->getZ() < _Entity2->getZ());
		}
	}
}