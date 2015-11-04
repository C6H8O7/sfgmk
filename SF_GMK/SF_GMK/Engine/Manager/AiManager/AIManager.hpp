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
		struct stAIStateFunctions
		{
			int iAdvance;
			FoncterTemplate* CurrentFunc[3];
		};

		class AIStateMachine
		{
			public:
				AIStateMachine();
				~AIStateMachine();

			private:
				int m_iState;
				int* m_iNextState;
				DynamicArray<stAIStateFunctions> m_StatesFunctionsArray;

			public:
				void update();
		};


		class AIManager
		{
			public:
				AIManager();
				~AIManager();
		};
	}
}


#endif