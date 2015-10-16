/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		17/06/2015
	@brief		Tableau stockant des fonctions, de manière générique

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_FUNCTION_ARRAY_HPP
#define SFGMK_FUNCTION_ARRAY_HPP


namespace sfgmk
{
	template<typename T, typename... Args>
	class FunctionArray
	{
		private:
			T(*m_Functions[2])(void); //Tableau de pointeurs des fonctions principales

		public:
			FunctionArray()
			{
			}

			~FunctionArray()
			{
			}
	};
}


#endif