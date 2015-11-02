namespace sfgmk
{
	namespace engine
	{
		DebugManager::DebugManager()
		{
			ADD_ENTITY(&m_Selector);

			//Commandes console
			m_Console.registerCommand("/freecam", new sfgmk::FoncterMethodTemplate<Camera, void>(GraphicManager::getSingleton()->getCurrentCamera(), &Camera::setFreeMove), "Camera libre activee\n", "Camera libre desactivee\n");
			m_Console.registerCommand("/physic", new sfgmk::FoncterMethodTemplate<PhysicManager, void>(PHYSIC_MANAGER, &PhysicManager::setDraw), "Affichage physique activee\n", "Affichage physique desactivee\n");
			m_Console.registerCommand("/life", new sfgmk::FoncterFunctionTemplate<void>(&EntityWithPv::setLifeBarDraw), "Affichage vie activee\n", "Affichage vie desactivee\n");
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
	}
}