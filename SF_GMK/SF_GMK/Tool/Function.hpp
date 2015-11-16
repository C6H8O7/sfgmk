/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		11/01/2015
	@brief		Diverses fonctions utilis�es dans sfgmk

--------------------------------------------------------------------------------------------------*/

#ifndef FUNCTION_HPP
#define FUNCTION_HPP


namespace sfgmk
{
	#define SAFE_DELETE(a) if( a != NULL ) { delete a; a = NULL; }

	struct stSTRING_ARRAY
	{
		std::string* sStrings;
		unsigned int uiStringNumber;
	};

	//Supprime les �l�ments d'un conteneur, et le clear
	template<typename T> 
	void ClearPtrCntr(T& _Cntr)
	{
		for( auto it = _Cntr.begin(); it != _Cntr.end(); ++it )
			delete *it;

		_Cntr.clear();
	}

	//Supprime un �l�ment d'un vector apr�s l'avoir pass� en queue
	template<typename T>
	void DeletePtrCntrElement(std::vector<T*>& _Cntr, const unsigned int& _ElementIndex)
	{
		unsigned int VectorEnd = _Cntr.size() - 1;

		T* Temp = _Cntr[_ElementIndex];
		_Cntr[_ElementIndex] = _Cntr[VectorEnd];
		_Cntr[VectorEnd] = Temp;
		delete Temp;

		_Cntr.pop_back();
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
					T fTemp = _Array[i + 1];
					_Array[i + 1] = _Array[i];
					_Array[i] = fTemp;
				}
			}
		}
	}

	//Execute une fonction, et renvoie la dur�e d'ex�cution de celle-ci
	template<typename T, typename... Args>
	signed long long measureFunctionExecutionTime(std::function<T(Args... _Args)>& _Function, Args... _Args)
	{
		sf::Clock Timer;

		_Function(_Args...);

		return (_Timer.getElapsedTime().asMicroseconds());
	}

	template<typename T, typename T2, typename... Args>
	signed long long measureFunctionExecutionTime(T2(T::*ptr)(Args... _Args), T* _Instance)
	{
		sf::Clock Timer;

		(_Instance->*ptr)(_Args...);

		return (Timer.getElapsedTime().asMicroseconds());
	}

	template<typename... Args>
	signed long long measureFoncterExecutionTime(FoncterTemplate* _Fonc, Args... _Args)
	{
		sf::Clock Timer;

		_Fonc->Execute(_Args...);

		return (Timer.getElapsedTime().asMicroseconds());
	}

	//Met tous les �l�ments d'un tableau � la valeur donn�e
	template<typename T>
	void initArrayValue(T* _Array, const size_t& _ArraySize, const T& _Value)
	{
		for( size_t i(0); i < _ArraySize; i++ )
			_Array[i] = _Value;
	}

	//Renvoi le nombre de fichiers dans le dossier sp�cifi�
	int SFGMK_API getNumberOfFileInDir(const char* _DirPath);
	int SFGMK_API getNumberOfFileInDir(const std::string& _DirPath);

	//D�place le curseur de lecture d'un fichier jusqu'au caract�re pass� en argument
	bool SFGMK_API goToChar(FILE* _File, const int& _Char);

	//D�coupe une string en multiples strings selon la taille max sp�cifi�e
	stSTRING_ARRAY* cutStringInPart(const std::string& _String, const unsigned int& _MaxSize);
}


#endif