/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		02/07/2015
	@brief		Classe ajoutant des fonctionnalités aux std::thread
				Samples:	ThreadTemplate<> NewThread(new FoncterTemplateInstance<CLASS_NULL, void>(&Function));
							ThreadTemplate<> NewThread(new FoncterTemplateInstance<Classe, void>(Instance, &Classe::Function));

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_THREAD_TEMPLATE_HPP
#define SFGMK_THREAD_TEMPLATE_HPP


namespace sfgmk
{
	template<typename... Args>
	class ThreadTemplate
	{
		private:
			std::thread* m_Thread;
			FoncterTemplate* m_Function;
			bool m_bLaunched;
			bool m_bWaited;

			inline void Run(Args... _Args)
			{
				m_Function->Execute(_Args...);
			}

		public:
			ThreadTemplate(FoncterTemplate* _Foncter = NULL) : m_Thread(NULL), m_Function(_Foncter), m_bLaunched(false), m_bWaited(false) {}
			~ThreadTemplate() { Wait(); SAFE_DELETE(m_Function); }

			bool SetFunc(FoncterTemplate* _Foncter)
			{
				if( m_bLaunched )
					return false;

				m_Function = _Foncter;
				return true;
			}

			bool Launch(Args... _Args)
			{
				if( !m_bLaunched && m_Function )
				{
					m_bLaunched = true;
					m_Thread = new std::thread(&ThreadTemplate::Run, this, _Args...);
					return true;
				}

				return false;
			}

			void Wait()
			{
				if( m_bLaunched && !m_bWaited )
				{
					m_Thread->join();
					m_bWaited = true;
				}
			}

			void Reset()
			{
				if( !m_bLaunched || m_bWaited )
				{
					SAFE_DELETE(m_Thread);
					m_bLaunched = false;;
					m_bWaited = false;
				}
			}

			BOOL Kill() //A utiliser à vos risques et périls
			{
				BOOL bResult(false);

				if( m_bLaunched && !m_bWaited )
					bResult = TerminateThread((HANDLE)m_Thread->native_handle(), 0);

				return bResult;
			}

			inline BOOL SetPriority(const int& _Priority)
			{
				if( m_Thread )
					return SetThreadPriority((HANDLE)m_Thread->native_handle(), _Priority);
				return false;
			}
	};
}


#endif