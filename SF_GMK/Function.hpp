/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		11/01/2015
	@brief		Diverses fonctions utilis�es dans sfgmk

--------------------------------------------------------------------------------------------------*/

#ifndef FUNCTION_HPP
#define FUNCTION_HPP


namespace sfgmk
{
	//Op�rations sur les bits
	#define SET_BIT(flag, bit) ((flag) |= (1 << (bit))) 
	#define CLEAR_BIT(flag, bit) ((flag) &= ~(1 << (bit))) 
	#define GET_BIT(flag, bit) ((flag) & (1 << (bit)))

	/*
	enum {
	OPTION1 = 1 << 0,
	OPTION2 = 1 << 1
	};

	void foo(int);

	int main(void)
	{
	foo(OPTION1 | OPTION2);
	return 0;
	}

	void foo(int options)
	{
	if (options & OPTION1)
	printf("OPTION1 trouve\n");
	if (options & OPTION2)
	printf("OPTION2 trouve\n");
	}



	foo(OPTION1 | OPTION2)

	options & OPTION1

	*/

	#define SAFE_DELETE(a) if( a != NULL ) {delete a; a= NULL;}


	//Supprime les �l�ments d'un conteneur, et le clear
	template <class T> void ClearPtrCntr(T& cntr)
	{
		for( typename T::iterator it = cntr.begin(); it != cntr.end(); ++it )
			delete *it;

		cntr.clear();
	}

	//Tri � bulle d'un tableau
	template <class T> void sortBubble(T* _Array, int _Size)
	{
		bool bChange(true);

		while( bChange )
		{
			bChange = false;

			for( int i(0); i < _Size - 1; i++ )
			{
				if( _Array[i] > _Array[i + 1] )
				{
					bChange = true;
					float fTemp = _Array[i + 1];
					_Array[i + 1] = _Array[i];
					_Array[i] = fTemp;
				}
			}
		}
	}

	//Execute une fonction, et renvoie la dur�e d'ex�cution de celle-ci
	template<typename T, typename... Args>
	double measureFunctionExecutionTime(std::function<T(Args... _Args)> _Function, Args... _Args)
	{
		sf::Time ElapsedTime;
		sf::Clock Timer;

		_Function(_Args...);

		ElapsedTime = Timer.getElapsedTime();
		return ElapsedTime.asMicroseconds() * 0.001f;
	}

	//Met tous les �l�ments d'un tableau � la valeur donn�e
	template<typename T>
	void initArrayValue(T* _Array, const size_t& _ArraySize, const T& _Value)
	{
		for( size_t i(0); i < _ArraySize; i++ )
			_Array[i] = _Value;
	}

	//Renvoi le nombre de fichiers dans le dossier sp�cifi�
	int getNumberOfFileInDir(const char* _DirPath);
	int getNumberOfFileInDir(const std::string& _DirPath);

	//D�place le curseur de lecture d'un fichier jusqu'au caract�re pass� en argument
	bool goToChar(FILE* _File, const int& _Char);
}


#endif