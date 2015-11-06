/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		02/11/2015
	@brief		FSM de l'AI (ATTENTION, implémentation actuelle = 32 states MAXIMUM)

--------------------------------------------------------------------------------------------------*/

#ifndef AI_FSM_HPP
#define AI_FSM_HPP


namespace sfgmk
{
	#define OnEnter if( _Progress == engine::eEnter )
	#define OnUpdate else if( _Progress == engine::eUpdate )
	#define OnExit else if( _Progress == engine::eExit )

	namespace engine
	{
		enum eAIState_Progress
		{
			eEnter = 0,
			eUpdate,
			eExit,
			eAIState_Progress_NUMBER
		};

		struct stAIStateFunctions
		{
			eAIState_Progress StateProgress;
			FoncterTemplate* CurrentFunc;
		};

		class SFGMK_API AIStateMachine
		{
			friend class Entity;

			public:
				AIStateMachine(Entity* _Parent, int _InitState = 0);
				~AIStateMachine();

			private:
				int m_iState;
				int m_iNextState;
				DynamicArray<stAIStateFunctions*> m_StatesFunctionsArray;

				Entity* m_EntityParent;

				void process(const float& _TimeDelta);

			public:
				void progress();
				void changeState();
				void changeState(const int& _NextState);
				void setNextState(const int& _NextState);

				bool addState(int _StateId, FoncterTemplate* _NewFunction);
		};
	}
}


#endif