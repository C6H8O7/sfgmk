/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		16/12/2014
	@brief		Classe qui gère la machine a état du jeu

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_STATEMACHINE_HPP
#define SFGMK_STATEMACHINE_HPP


namespace sfgmk
{
	namespace engine
	{
		typedef State* StatePtr;
		typedef StatePtr(*tStateCreationFunc)();

		class StateMachine
		{
			public:
				union StateCreationConv
				{
					void* _voidptr;
					tStateCreationFunc _statefunc;
				};

				StateMachine();
				~StateMachine();

				State *CreateState(int _id);

				void init(int _id);
				void update();
				bool changeState(int _id, int _StateToLoadId = 0);

				void draw();

				State* Get_CurrentState();

				const int& getStateCreatedId();
				const int& getStateToLoadId();

			private:
				State *m_CurrentState;

				int m_iStateCreated;
				int m_iStateToLoad;
		};
	}
}


#endif