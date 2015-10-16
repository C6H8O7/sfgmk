/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		02/07/2015
	@brief		Classe ajoutant des fonctionnalités aux std::thread

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_THREAD_TEMPLATE_HPP
#define SFGMK_THREAD_TEMPLATE_HPP


namespace sfgmk
{
	template<typename T, typename... Args>
	class ThreadTemplate
	{
		private:
			std::thread m_Thread;
			std::function<T(Args... _Args)> m_Function;
			bool m_bLaunched;
		
		public:
			ThreadTemplate(std::function<T(Args... _Args)> _Function, Args... _Args, bool _LaunchThreadNow = false) : m_Function(_Function), m_bLaunched(false)
			{
				if( _LaunchThreadNow )
					Launch(_Args);
			}

			~ThreadTemplate()
			{
				Wait();
			}


			bool Launch(Args... _Args)
			{
				if( !m_bLaunched )
				{
					m_bLaunched = true;
					m_Thread = std::thread(m_Function, _Args);
					return true;
				}

				return false;
			}

			void Wait()
			{
				if( m_bLaunched )
					m_Thread.join();
			}
	};
}


#endif