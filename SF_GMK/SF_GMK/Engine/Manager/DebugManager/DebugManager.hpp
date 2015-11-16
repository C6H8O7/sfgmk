/*--------------------------------------------------------------------------------------------------

@author		GMK
@date		29/10/2015
@brief		Console et autres outils debug

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_DEBUG_MANAGER_HPP
#define SFGMK_DEBUG_MANAGER_HPP

namespace sfgmk
{
	namespace engine
	{
		class SFGMK_API DebugManager : public SingletonTemplate<DebugManager>
		{
			friend class SingletonTemplate<DebugManager>;

			private:
				DebugManager();
				~DebugManager();

				std::thread* m_ConsoleThread;
				ConsoleDev m_Console;

				Selector* m_Selector;

			public:
				void update(const float& _TimeDelta);

				inline ConsoleDev& getConsole() { return m_Console; };
				inline Selector* getSelector() { return m_Selector; };

				void SetSelector();
				void DeleteSelector();

				void ShowWarden();
		};
	}
}


#endif