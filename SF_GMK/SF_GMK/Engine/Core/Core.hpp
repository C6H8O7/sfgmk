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
		struct sSfgmkExecutionTimes
		{
			signed long long dEntityUpdate;
			signed long long dEntitySort;
			signed long long dParallaxeComputation;
			signed long long dPhysic;
			signed long long dParallaxeDisplay;
			signed long long dStateUpdate;
			signed long long dStateDraw;
		};

		class SFGMK_API Core : public SingletonTemplate<Core>
		{
			friend class SingletonTemplate<Core>;

			private:
				Core();
				~Core();

				sf::Clock m_ClockTimeDelta;
				float m_fTimeDelta;

				sSfgmkExecutionTimes m_ExecutionTimes;
				
			public:
				void update();
				void preLoop();
				void loop();
				void postLoop();

				inline const float& getTimeDelta() { return m_fTimeDelta; }

				const sSfgmkExecutionTimes& getExecutionTimes() { return m_ExecutionTimes; }
		};
	}
}


#endif