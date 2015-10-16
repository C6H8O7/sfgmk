/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		18/06/2015
	@brief		Permet de stocker une fonction, quel que soit son type / ses arguments

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_GENERIC_FUNCTION_HPP
#define SFGMK_GENERIC_FUNCTION_HPP


namespace sfgmk
{
	template<typename T, typename... Args>
	class GenericFunction
	{
		private:
			T(*m_Function[1])(Args...); 

		public:
			GenericFunction()
			{
			}

			~GenericFunction()
			{
			}


			T(**getFunction(void))(Args...)
			{
				return m_Function;
			}
	};
}


#endif