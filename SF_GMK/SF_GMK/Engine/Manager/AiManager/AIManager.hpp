/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		02/11/2015
	@brief		Fichier d'entête de l'AI Manager

--------------------------------------------------------------------------------------------------*/

#ifndef AI_MANAGER_HPP
#define AI_MANAGER_HPP


namespace sfgmk
{
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
			FoncterTemplate* CurrentFunc[eAIState_Progress_NUMBER];
		};

		class AIStateMachine
		{
			public:
				AIStateMachine(Entity* _Parent, int _InitState = 0);
				~AIStateMachine();

			private:
				int m_iState;
				int* m_iNextState;
				DynamicArray<stAIStateFunctions> m_StatesFunctionsArray;

				Entity* m_EntityParent;

			public:
				void process(const float& _TimeDelta);

				bool addState(int _StateId, FoncterTemplate* _NewFunction);
		};
	}
}


#endif