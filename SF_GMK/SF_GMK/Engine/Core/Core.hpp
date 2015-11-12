/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		17/10/2015
	@brief		Coeur du moteur, accés aux managers

--------------------------------------------------------------------------------------------------*/

#ifndef CORE_HPP
#define CORE_HPP


namespace sfgmk
{
	namespace engine
	{
		struct sMANAGER_EXECUTION_TIMES
		{
			signed long long llUpdate;
			signed long long llDraw;
		};

		enum eMANAGERS
		{
			eAIManager = 0,
			eEntityManager,
			eGraphicManager,
			eInputManager,
			ePhysicManager,
			eSoundManager,
			eStateMachineManager,
			eDebugManager,
			eMsgManager,
			eMANAGERS_NUMBER
		};

	#ifdef SFGMK_DEBUG
		#define AI_MANAGER_UPDATE				m_ExecutionTimes[eAIManager].llUpdate = measureFoncterExecutionTime(m_MainFunctions.m_FunctionsArray[0], m_fTimeDelta);
		#define ENTITY_MANAGER_UPDATE			m_ExecutionTimes[eEntityManager].llUpdate = measureFoncterExecutionTime(m_MainFunctions.m_FunctionsArray[1]);
		#define GRAPHIC_MANAGER_SET				m_ExecutionTimes[eGraphicManager].llUpdate = measureFoncterExecutionTime(m_MainFunctions.m_FunctionsArray[2]);
		#define GRAPHIC_MANAGER_UPDATE			m_ExecutionTimes[eGraphicManager].llUpdate += measureFoncterExecutionTime(m_MainFunctions.m_FunctionsArray[3]);
		#define GRAPHIC_MANAGER_DRAW			m_ExecutionTimes[eGraphicManager].llDraw = measureFoncterExecutionTime(m_MainFunctions.m_FunctionsArray[4]);
		#define GRAPHIC_MANAGER_DISPLAY			m_ExecutionTimes[eGraphicManager].llDraw += measureFoncterExecutionTime(m_MainFunctions.m_FunctionsArray[5]);
		#define INPUT_MANAGER_UPDATE			m_ExecutionTimes[eInputManager].llUpdate = measureFoncterExecutionTime(m_MainFunctions.m_FunctionsArray[6]);
		#define PHYSIC_MANAGER_UPDATE			m_ExecutionTimes[ePhysicManager].llUpdate = measureFoncterExecutionTime(m_MainFunctions.m_FunctionsArray[7]);
		#define SOUND_MANAGER_UPDATE			m_ExecutionTimes[eSoundManager].llUpdate = measureFoncterExecutionTime(m_MainFunctions.m_FunctionsArray[8]);
		#define STATE_MACHINE_MANAGER_UPDATE	m_ExecutionTimes[eStateMachineManager].llUpdate = measureFoncterExecutionTime(m_MainFunctions.m_FunctionsArray[9]);
		#define STATE_MACHINE_MANAGER_DRAW		m_ExecutionTimes[eStateMachineManager].llDraw = measureFoncterExecutionTime(m_MainFunctions.m_FunctionsArray[10]);
		#define MSG_MANAGER_UPDATE				m_ExecutionTimes[eMsgManager].llUpdate = measureFoncterExecutionTime(m_MainFunctions.m_FunctionsArray[11]);
	#endif

	#ifndef SFGMK_DEBUG
		#define AI_MANAGER_UPDATE				m_MainFunctions.m_FunctionsArray[0]->Execute(m_fTimeDelta);
		#define ENTITY_MANAGER_UPDATE			m_MainFunctions.m_FunctionsArray[1]->Execute();
		#define GRAPHIC_MANAGER_SET				m_MainFunctions.m_FunctionsArray[2]->Execute();
		#define GRAPHIC_MANAGER_UPDATE			m_MainFunctions.m_FunctionsArray[3]->Execute();
		#define GRAPHIC_MANAGER_DRAW			m_MainFunctions.m_FunctionsArray[4]->Execute();
		#define GRAPHIC_MANAGER_DISPLAY			m_MainFunctions.m_FunctionsArray[5]->Execute();
		#define INPUT_MANAGER_UPDATE			m_MainFunctions.m_FunctionsArray[6]->Execute();
		#define PHYSIC_MANAGER_UPDATE			m_MainFunctions.m_FunctionsArray[7]->Execute();
		#define SOUND_MANAGER_UPDATE			m_MainFunctions.m_FunctionsArray[8]->Execute();
		#define STATE_MACHINE_MANAGER_UPDATE	m_MainFunctions.m_FunctionsArray[9]->Execute();
		#define STATE_MACHINE_MANAGER_DRAW		m_MainFunctions.m_FunctionsArray[10]->Execute();
		#define MSG_MANAGER_UPDATE				m_MainFunctions.m_FunctionsArray[11]->Execute();
	#endif

		class SFGMK_API Core : public SingletonTemplate<Core>
		{
			friend class SingletonTemplate<Core>;

			private:
				Core();
				~Core();

				sf::Clock m_ClockTimeDelta;
				float m_fTimeDelta;

				FoncterTemplateArray m_MainFunctions;
				sMANAGER_EXECUTION_TIMES m_ExecutionTimes[eMANAGERS_NUMBER];

			public:
				bool initManagers();
				bool releaseManagers();

				void update();

				void preLoop();
				void loop();
				void postLoop();

				inline const float& getTimeDelta() { return m_fTimeDelta; }
				inline sMANAGER_EXECUTION_TIMES* getManagersExecutionTimes() { return m_ExecutionTimes; }
		};
	}
}


#endif