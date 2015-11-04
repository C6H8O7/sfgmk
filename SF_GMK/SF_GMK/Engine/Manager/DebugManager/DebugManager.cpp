namespace sfgmk
{
	namespace engine
	{
		DebugManager::DebugManager() : m_Selector(NULL)
		{
			//Commandes console
			m_Console.registerCommand("/freecam", new sfgmk::FoncterTemplateInstance<Camera, void>(GraphicManager::getSingleton()->getCurrentCamera(), &Camera::setFreeMove), "Camera libre activee\n", "Camera libre desactivee\n");
			m_Console.registerCommand("/physic", new sfgmk::FoncterTemplateInstance<PhysicManager, void>(PHYSIC_MANAGER, &PhysicManager::setDraw), "Affichage physique activee\n", "Affichage physique desactivee\n");
			m_Console.registerCommand("/life", new sfgmk::FoncterTemplateInstance<EntityWithPv, void>(&EntityWithPv::setLifeBarDraw), "Affichage vie activee\n", "Affichage vie desactivee\n");
			m_Console.registerCommand("/selector", new sfgmk::FoncterTemplateInstance<DebugManager, void>(this, &DebugManager::SetSelector), "Selector active\n", "Selector desactive\n");
		}

		DebugManager::~DebugManager()
		{
		}


		void DebugManager::update(const float& _TimeDelta)
		{
			//Console
			if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::Quote) == sfgmk::engine::KeyStates::KEY_PRESSED )
				m_Console.setActive();

			m_Console.update(_TimeDelta);
		}


		void DebugManager::SetSelector()
		{
			if( m_Selector )
			{
				m_Selector->setIsAlive(false);
				m_Selector = NULL;
			}
			else
			{
				m_Selector = new Selector(true);
				ADD_ENTITY(m_Selector);
			}
		}

		void DebugManager::DeleteSelector()
		{
			m_Selector = NULL;
		}
	}
}