namespace sfgmk
{
	namespace engine
	{
		Selector::Selector(bool _IsDebugManagerSelector) : m_bIsDebugManagerSelector(_IsDebugManagerSelector)
		{
			getSprite()->setTexture(DATA_MANAGER->getTexture("sfgmk_pix"));
			getSprite()->setColor(sf::Color(0, 255, 255, 64));
			setRelativOrigin(0.5f, 0.5f);

			m_fPositionZ = -1;

			m_State = SELECTOR_STATE::STANDBY;
		}

		Selector::~Selector()
		{
			if( m_bIsDebugManagerSelector )
				DEBUG_MANAGER->DeleteSelector();
		}

		void Selector::update(const float& _TimeDelta)
		{
			// Selector::update ==================

			sfgmk::engine::Mouse& mouse = INPUT_MANAGER->getMouse();

			sf::Vector2f mouse_pos = mouse.getWorldPosition();

			switch (mouse.getButtonState(sf::Mouse::Left))
			{
				case KeyStates::KEY_PRESSED:
					m_Start = mouse_pos;
					m_State = SELECTOR_STATE::SELECTION;
					break;

				case KeyStates::KEY_RELEASED:
					m_State = SELECTOR_STATE::SELECTED;
					break;

				case KeyStates::KEY_UP:
					m_State = SELECTOR_STATE::STANDBY;
					break;
			}

			switch (m_State)
			{
				case SELECTOR_STATE::SELECTION:
					m_End = mouse_pos;
					break;

				case SELECTOR_STATE::SELECTED:
					break;

				case SELECTOR_STATE::STANDBY:
					break;
			}

			sf::Vector2f scale = m_End - m_Start;
			scale.x = ABS(scale.x);
			scale.y = ABS(scale.y);

			sf::Vector2f pos = 0.5f * (m_End + m_Start);

			setPosition(pos);
			setScale(scale);

			if (m_Start != m_End)
				addObbCollider();
			else
				SAFE_DELETE(m_Collider);

			// Basic Entity::update ==============

			if (m_Collider)
				m_Collider->setCollide(false);

			finalize(_TimeDelta);

			// ===================================
		}
		
		void Selector::onPhysicEnter()
		{
			m_SelectedEntities.clear();
		}

		void Selector::onPhysicCollision(Entity* _entity)
		{
			m_SelectedEntities.pushBack(_entity);
		}

		void Selector::onPhysicExit()
		{
			printf("Selected entities = %d\n", m_SelectedEntities.getElementNumber());
		}
	}
}