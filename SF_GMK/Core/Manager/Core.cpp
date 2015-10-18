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

				sfgmk::InputManager::getSingleton()->handleEvent(event);
			}
			GraphicManager::getSingleton()->set();

			//Console
			sfgmk::ConsoleDev::getSingleton()->update(m_fTimeDelta);

			if( sfgmk::InputManager::getSingleton()->getKeyboard().getKeyState(sf::Keyboard::Quote) == sfgmk::KeyStates::KEY_PRESSED )
				sfgmk::ConsoleDev::getSingleton()->setActive();

			//Son
			SoundManager::getSingleton()->update();
		}

		void Core::loop()
		{
			//Update �tat(s) courant(s)
			StateMachineManager::getSingleton()->update();

			//Si state loading en cours, c'est une autre instance de parallaxe qui est utilis�e
			//Parallaxe* CurrentParallaxe = &m_Parallaxe;
			//if( CURRENT_STATE->getID() == eLoadingState )
			//	CurrentParallaxe = ((sfgmk::StateLoading*)(CURRENT_STATE))->Get_Parallaxe();

			//Update des entit�s
			EntityManager::getSingleton()->update();

			//Physique
			//std::function<void()> PhysicUpdate(std::bind(&PhysicManager::update, sfgmk::PhysicManager::getSingleton()));
			//m_EntityTimers.dPhysic = measureFunctionExecutionTime(PhysicUpdate);

			//Trie les entit�s du vector en fonction de leur Z
			EntityManager::getSingleton()->sortEntityVector();

			//Draw �tat(s) courant(s)
			StateMachineManager::getSingleton()->draw();

			//GraphicManager::getSingleton()->compute();
			//GraphicManager::getSingleton()->draw();
		}

		void Core::postLoop()
		{
			GraphicManager::getSingleton()->display();
		}
	}
}