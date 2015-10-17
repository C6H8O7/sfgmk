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
		class Core : SingletonTemplate<Core>
		{
			friend class SingletonTemplate<Core>;

			private:
				Core();
				~Core();

				sf::Clock m_ClockTimeDelta;
				float m_fTimeDelta;
				
			public:
				void update();
				void preLoop();
				void loop();
				void postLoop();

				inline const float& getTimeDelta() { return m_fTimeDelta; }

				//inline static AIManager* getAIManager() { return AIManager::getSingleton(); }
				inline static DataManager* getDataManager() { return DataManager::getSingleton(); }
				//inline static DebugManager* getDebigManager() { return DebugManager::getSingleton(); }
				//inline static EntityManager* getEntityManager() { return EntityManager::getSingleton(); }
				//inline static GraphicManager* getGraphicManager() { return GraphicManager::getSingleton(); }
				inline static InputManager* getInputManager() { return InputManager::getSingleton(); }
				//inline static MessageManager* getMessageManager() { return MessageManager::getSingleton(); }
				//inline static PhysicManager* getPhysicManager() { return PhysicManager::getSingleton(); }
				inline static SoundManager* getSoundManager() { return SoundManager::getSingleton(); }
				//inline static StateMachineManager* get StateMachineManager() { return StateMachineManager::getSingleton(); }
		};
	}
}


#endif