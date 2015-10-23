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
			INPUT_MANAGER->update();

			//Boucle events
			sf::Event event;
			while( GRAPHIC_MANAGER->getRenderWindow()->pollEvent(event) )
			{
				if( event.type == sf::Event::Closed )
					GRAPHIC_MANAGER->getRenderWindow()->close();

				INPUT_MANAGER->handleEvent(event);
			}
			GRAPHIC_MANAGER->set();

			//Console
			sfgmk::engine::ConsoleDev::getSingleton()->update(m_fTimeDelta);
			
			if( INPUT_MANAGER->getKeyboard().getKeyState(sf::Keyboard::Quote) == sfgmk::engine::KeyStates::KEY_PRESSED )
				sfgmk::engine::ConsoleDev::getSingleton()->setActive();

			//Son
			SOUND_MANAGER->update();
		}

		void Core::loop()
		{
			//Update état(s) courant(s)
			STATE_MACHINE_MANAGER->update();

			//Update des entités
			ENTITY_MANAGER->update();

			//Physique
			PHYSIC_MANAGER->update();
			//std::function<void()> PhysicUpdate(std::bind(&PhysicManager::update, sfgmk::PhysicManager::getSingleton()));
			// = measureFunctionExecutionTime(PhysicUpdate);

			//Trie les entités du vector en fonction de leur Z
			ENTITY_MANAGER->sortEntityVector();

			//void(EntityManager::*ptr)(void) = &EntityManager::sortEntityVector;
			//(EntityManager::getSingleton()->*ptr)();


			//m_ExecutionTimes.dEntitySort = measureFunctionExecutionTime(ptr, EntityManager::getSingleton());

			/*int (A::*ptr)(int) = &A::fonction;
			int (A::*ptr)(int) = &A::fonction;  //On déclare un pointeur sur la fonction membre
			A instance;  //On crée une instance de la classe A
			int resultat = (instance.*ptr)(2);*/

			GRAPHIC_MANAGER->compute();
			GRAPHIC_MANAGER->draw();

			//Draw état(s) courant(s)
			STATE_MACHINE_MANAGER->draw();
		}

		void Core::postLoop()
		{
			GRAPHIC_MANAGER->display();
		}
	}
}