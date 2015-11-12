namespace sfgmk
{
	namespace engine
	{
		Core::Core() : m_fTimeDelta(0.0f)
		{
		#ifdef SFGMK_DEBUG
			memset(m_ExecutionTimes, 0, sizeof(sMANAGER_EXECUTION_TIMES[eMANAGERS_NUMBER]));
		#endif
		}

		Core::~Core()
		{
		}


		bool Core::initManagers()
		{
			//AIManager
			sfgmk::FoncterTemplateInstance<AIManager, void, const float&>* AIManagerUpdatePtr = new sfgmk::FoncterTemplateInstance<AIManager, void, const float&>(AI_MANAGER, &AIManager::update);
			m_MainFunctions.m_FunctionsArray.pushBack(AIManagerUpdatePtr);
		
			//EntityManager
			sfgmk::FoncterTemplateInstance<EntityManager, void>* EntityManagerUpdatePtr = new sfgmk::FoncterTemplateInstance<EntityManager, void>(ENTITY_MANAGER, &EntityManager::update);
			m_MainFunctions.m_FunctionsArray.pushBack(EntityManagerUpdatePtr);

			//GraphicManager
			sfgmk::FoncterTemplateInstance<GraphicManager, void>* GraphicManagerSetPtr = new sfgmk::FoncterTemplateInstance<GraphicManager, void>(GRAPHIC_MANAGER, &GraphicManager::set);
			m_MainFunctions.m_FunctionsArray.pushBack(GraphicManagerSetPtr);
			sfgmk::FoncterTemplateInstance<GraphicManager, void>* GraphicManagerUpdatePtr = new sfgmk::FoncterTemplateInstance<GraphicManager, void>(GRAPHIC_MANAGER, &GraphicManager::update);
			m_MainFunctions.m_FunctionsArray.pushBack(GraphicManagerUpdatePtr);
			sfgmk::FoncterTemplateInstance<GraphicManager, void>* GraphicManagerDrawPtr = new sfgmk::FoncterTemplateInstance<GraphicManager, void>(GRAPHIC_MANAGER, &GraphicManager::draw);
			m_MainFunctions.m_FunctionsArray.pushBack(GraphicManagerDrawPtr);
			sfgmk::FoncterTemplateInstance<GraphicManager, void>* GraphicManagerDisplayPtr = new sfgmk::FoncterTemplateInstance<GraphicManager, void>(GRAPHIC_MANAGER, &GraphicManager::display);
			m_MainFunctions.m_FunctionsArray.pushBack(GraphicManagerDisplayPtr);

			//InputManager
			sfgmk::FoncterTemplateInstance<InputManager, void>* InputManagerUpdatePtr = new sfgmk::FoncterTemplateInstance<InputManager, void>(INPUT_MANAGER, &InputManager::update);
			m_MainFunctions.m_FunctionsArray.pushBack(InputManagerUpdatePtr);

			//PhysicManager
			sfgmk::FoncterTemplateInstance<PhysicManager, void>* PhysicManagerUpdatePtr = new sfgmk::FoncterTemplateInstance<PhysicManager, void>(PHYSIC_MANAGER, &PhysicManager::update);
			m_MainFunctions.m_FunctionsArray.pushBack(PhysicManagerUpdatePtr);

			//SoundManager
			sfgmk::FoncterTemplateInstance<SoundManager, void>* SoundManagerUpdatePtr = new sfgmk::FoncterTemplateInstance<SoundManager, void>(SOUND_MANAGER, &SoundManager::update);
			m_MainFunctions.m_FunctionsArray.pushBack(SoundManagerUpdatePtr);

			//StateMachineManager
			sfgmk::FoncterTemplateInstance<StateMachineManager, void>* StateMachineManagerUpdatePtr = new sfgmk::FoncterTemplateInstance<StateMachineManager, void>(STATE_MACHINE_MANAGER, &StateMachineManager::update);
			m_MainFunctions.m_FunctionsArray.pushBack(StateMachineManagerUpdatePtr);
			sfgmk::FoncterTemplateInstance<StateMachineManager, void>* StateMachineManagerDrawPtr = new sfgmk::FoncterTemplateInstance<StateMachineManager, void>(STATE_MACHINE_MANAGER, &StateMachineManager::draw);
			m_MainFunctions.m_FunctionsArray.pushBack(StateMachineManagerDrawPtr);

			//MsgManager
			sfgmk::FoncterTemplateInstance<MsgManager, void>* MsgManagerUpdatePtr = new sfgmk::FoncterTemplateInstance<MsgManager, void>(MESSAGE_MANAGER, &MsgManager::Update);
			m_MainFunctions.m_FunctionsArray.pushBack(MsgManagerUpdatePtr);

			return true;
		}

		bool Core::releaseManagers()
		{
			m_MainFunctions.m_FunctionsArray.clear();

			AI_MANAGER->releaseSingleton();
			DATA_MANAGER->releaseSingleton();
			ENTITY_MANAGER->releaseSingleton();
			GRAPHIC_MANAGER->releaseSingleton();
			INPUT_MANAGER->releaseSingleton();
			PHYSIC_MANAGER->releaseSingleton();
			SOUND_MANAGER->releaseSingleton();
			STATE_MACHINE_MANAGER->releaseSingleton();

			return true;
		}

	
		void Core::update()
		{
			//Update du time delta
			m_fTimeDelta = (m_ClockTimeDelta.restart()).asSeconds();
		}

		void Core::preLoop()
		{
			this->update();

			DEBUG_MANAGER->update(m_fTimeDelta);

			INPUT_MANAGER_UPDATE
			GRAPHIC_MANAGER_SET
			SOUND_MANAGER_UPDATE
			MSG_MANAGER_UPDATE
		}

		void Core::loop()
		{
			STATE_MACHINE_MANAGER_UPDATE
			AI_MANAGER_UPDATE
			ENTITY_MANAGER_UPDATE
			PHYSIC_MANAGER_UPDATE
			GRAPHIC_MANAGER_UPDATE

			GRAPHIC_MANAGER_DRAW
			STATE_MACHINE_MANAGER_DRAW
		}

		void Core::postLoop()
		{
			GRAPHIC_MANAGER_DISPLAY
		}
	}
}