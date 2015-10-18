/*--------------------------------------------------------------------------------------------------

@author		GMK
@date		18/10/2015
@brief		Classe manager qui gère la machine a état du jeu

--------------------------------------------------------------------------------------------------*/

#ifndef STATEMACHINEMANAGER_H
#define STATEMACHINEMANAGER_H

namespace sfgmk
{
	namespace engine
	{
		class StateMachineManager : public SingletonTemplate< StateMachineManager >
		{
			friend class SingletonTemplate< StateMachineManager >;

			public:

				// Manager part ////////////////////////////////////////////

				void update();
				void draw();

				StateMachine* getStateMachine();

				// StateBank part ///////////////////////////////////////////

				std::map<int, std::pair<void*, std::string>> m_States;

				template <class T>
				void RegisterState(const int& _id, const std::string& _StateDataPath)
				{
					StateMachine::StateCreationConv conv;
					conv._statefunc = &GetStateCreationFunc<T>;

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

				///////////////////////////////////////////////////////////

			protected:

				StateMachineManager();
				~StateMachineManager();

				StateMachine* m_StateMachine;
		};
	}
}

#endif