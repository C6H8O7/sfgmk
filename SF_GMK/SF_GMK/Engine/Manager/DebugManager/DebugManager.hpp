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

				ConsoleDev m_Console;

				Selector m_Selector;

			public:
				void update(const float& _TimeDelta);

				inline ConsoleDev& getConsole() { return m_Console; };
		};
	}
}


#endif