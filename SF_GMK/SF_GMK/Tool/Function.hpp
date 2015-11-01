/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		11/01/2015
	@brief		Diverses fonctions utilisées dans sfgmk

--------------------------------------------------------------------------------------------------*/

#ifndef FUNCTION_HPP
#define FUNCTION_HPP


namespace sfgmk
{
	#define SAFE_DELETE(a) if( a != NULL ) { delete a; a = NULL; }

	//Opérations sur les bits
	#define SET_BIT(flag, bit) ((flag) |= (1 << (bit))) 
	#define CLEAR_BIT(flag, bit) ((flag) &= ~(1 << (bit))) 
	#define GET_BIT(flag, bit) ((flag) & (1 << (bit)))

	//Supprime les éléments d'un conteneur, et le clear
	template<typename T> 
	void ClearPtrCntr(T& _Cntr)
	{
		for( auto it = _Cntr.begin(); it != _Cntr.end(); ++it )
			delete *it;

		_Cntr.clear();
	}

	//Supprime un élément d'un vector après l'avoir passé en queue
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

	//Tri à bulle d'un tableau
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

	//Execute une fonction, et renvoie la durée d'exécution de celle-ci
	template<typename T, typename... Args>
	signed long long measureFunctionExecutionTime(signed long long& _Out, std::function<T(Args... _Args)>& _Function, Args... _Args)
	{
		sf::Clock Timer;

		_Function(_Args...);

		return (_Out = Timer.getElapsedTime().asMicroseconds());
	}

	template<typename T, typename T2, typename... Args>
	signed long long measureFunctionExecutionTime(signed long long& _Out, T2(T::*ptr)(Args... _Args), T* _Instance)
	{
		sf::Clock Timer;

		(_Instance->*ptr)(_Args...);

		return (_Out = Timer.getElapsedTime().asMicroseconds());
	}

	template<typename... Args>
	signed long long measureFoncterExecutionTime(signed long long& _Out, FoncterTemplate* _Fonc, Args... _Args)
	{
		sf::Clock Timer;

		_Fonc->Execute(_Args...);

		return (_Out = Timer.getElapsedTime().asMicroseconds());
	}

	//Met tous les éléments d'un tableau à la valeur donnée
	template<typename T>
	void initArrayValue(T* _Array, const size_t& _ArraySize, const T& _Value)
	{
		for( size_t i(0); i < _ArraySize; i++ )
			_Array[i] = _Value;
	}

	//Renvoi le nombre de fichiers dans le dossier spécifié
	int SFGMK_API getNumberOfFileInDir(const char* _DirPath);
	int SFGMK_API getNumberOfFileInDir(const std::string& _DirPath);

	//Déplace le curseur de lecture d'un fichier jusqu'au caractère passé en argument
	bool SFGMK_API goToChar(FILE* _File, const int& _Char);
}


#endif