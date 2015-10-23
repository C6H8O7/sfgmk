namespace sfgmk
{
	namespace engine
	{
		Core::Core()
		{
		}

		Core::~Core()
		{
		}

		void Core::update()
		{
			//Update du time delta
			m_fTimeDelta = (m_ClockTimeDelta.restart()).asSeconds();
		}

		void Core::preLoop()
		{
			this->update();

			//Update inputs
			InputManager::getSingleton()->update();

			//Boucle events
			sf::Event event;
			while( GraphicManager::getSingleton()->getRenderWindow()->pollEvent(event) )
			{
				if( event.type == sf::Event::Closed )
					GraphicManager::getSingleton()->getRenderWindow()->close();

				sfgmk::engine::InputManager::getSingleton()->handleEvent(event);
			}
			GraphicManager::getSingleton()->set();

			//Console
			sfgmk::engine::ConsoleDev::getSingleton()->update(m_fTimeDelta);

			if( sfgmk::engine::InputManager::getSingleton()->getKeyboard().getKeyState(sf::Keyboard::Quote) == sfgmk::engine::KeyStates::KEY_PRESSED )
				sfgmk::engine::ConsoleDev::getSingleton()->setActive();

			//Son
			SoundManager::getSingleton()->update();
		}

		void Core::loop()
		{
			//Update état(s) courant(s)
			StateMachineManager::getSingleton()->update();

			//Update des entités
			EntityManager::getSingleton()->update();

			//Physique
			PhysicManager::getSingleton()->update();
			//std::function<void()> PhysicUpdate(std::bind(&PhysicManager::update, sfgmk::PhysicManager::getSingleton()));
			//m_EntityTimers.dPhysic = measureFunctionExecutionTime(PhysicUpdate);

			//Trie les entités du vector en fonction de leur Z
			EntityManager::getSingleton()->sortEntityVector();

			GraphicManager::getSingleton()->compute();
			GraphicManager::getSingleton()->draw();

			//Draw état(s) courant(s)
			StateMachineManager::getSingleton()->draw();
		}

		void Core::postLoop()
		{
			GraphicManager::getSingleton()->display();
		}
	}
}