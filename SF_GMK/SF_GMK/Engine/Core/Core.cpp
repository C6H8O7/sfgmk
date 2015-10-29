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

			//Debug
			DEBUG_MANAGER->update(m_fTimeDelta);

			//Son
			SOUND_MANAGER->update();
		}

		void Core::loop()
		{
			//Update état(s) courant(s)
			void(StateMachineManager::*ptr6)(void) = &StateMachineManager::update;
			m_ExecutionTimes.dStateUpdate = measureFunctionExecutionTime(ptr6, STATE_MACHINE_MANAGER);

			//Update des entités
			void(EntityManager::*ptr4)(void) = &EntityManager::update;
			m_ExecutionTimes.dEntityUpdate = measureFunctionExecutionTime(ptr4, ENTITY_MANAGER);

			//Physique
			void(PhysicManager::*ptr5)(void) = &PhysicManager::update;
			m_ExecutionTimes.dPhysic = measureFunctionExecutionTime(ptr5, PHYSIC_MANAGER);

			//Trie les entités du vector en fonction de leur Z
			void(EntityManager::*ptr)(void) = &EntityManager::sortEntityVector;
			m_ExecutionTimes.dEntitySort = measureFunctionExecutionTime(ptr, ENTITY_MANAGER);

			//Parallaxe
			void(GraphicManager::*ptr2)(void) = &GraphicManager::compute;
			m_ExecutionTimes.dParallaxeComputation = measureFunctionExecutionTime(ptr2, GRAPHIC_MANAGER);

			//Draw parallaxe
			void(GraphicManager::*ptr3)(void) = &GraphicManager::draw;
			m_ExecutionTimes.dParallaxeDisplay = measureFunctionExecutionTime(ptr3, GRAPHIC_MANAGER);

			//Draw état(s) courant(s)
			void(StateMachineManager::*ptr7)(void) = &StateMachineManager::draw;
			m_ExecutionTimes.dStateDraw = measureFunctionExecutionTime(ptr7, STATE_MACHINE_MANAGER);
		}

		void Core::postLoop()
		{
			void(GraphicManager::*ptr)(void) = &GraphicManager::display;
			m_ExecutionTimes.dParallaxeDisplay += measureFunctionExecutionTime(ptr, GRAPHIC_MANAGER);
		}
	}
}