/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		25/11/2014
	@brief		Template de singleton, les différents managers en seront dérivés

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_SINGLETON_TEMPLATE_HPP
#define SFGMK_SINGLETON_TEMPLATE_HPP


namespace sfgmk
{
	template <typename T>
	class SingletonTemplate
	{
		protected:
			static T* m_Instance;

			SingletonTemplate()
			{
			}

			virtual ~SingletonTemplate()
			{
			}

		public:
			static T* getSingleton()
			{
				if( m_Instance == NULL )
					m_Instance = new T;

				return m_Instance;
			}

			static void releaseSingleton()
			{
				if( m_Instance != NULL )
				{
					delete m_Instance;
					m_Instance = NULL;
				}
			}
		};

	template <typename T>
	T *SingletonTemplate<T>::m_Instance = NULL;
}


#endif