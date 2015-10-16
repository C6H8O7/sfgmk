/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		16/12/2014
	@brief		Classe qui gère la machine a état du jeu

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_STATEMACHINE_HPP
#define SFGMK_STATEMACHINE_HPP


namespace sfgmk
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

	class StateBank : public SingletonTemplate<StateBank>
	{
		friend class SingletonTemplate<StateBank>;

		public:
			std::map<int, std::pair<void*, std::string>> m_States;
	
			template <class T>
			void RegisterState(const int& _id, const std::string& _StateDataPath)
			{
				StateMachine::StateCreationConv conv;
				conv._statefunc = &sfgmk::GetStateCreationFunc<T>;

				m_States[_id] = std::pair<void*, std::string>(conv._voidptr, _StateDataPath);
			}

			template <class T>
			friend State *GetStateCreationFunc()
			{
				return new T();
			}	

			const std::string& getStateRessourcePath(const int& _StateId)
			{
				return m_States[_StateId].second;
			}
	};
};


#endif