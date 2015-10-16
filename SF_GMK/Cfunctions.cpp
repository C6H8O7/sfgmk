namespace sfgmk
{
	const unsigned int mystrlen(const char* _Chain)
	{
		int i = 0;

		while( _Chain[i] != '\0' )
			i++;

		return i;
	}

	const unsigned int mystrnumlen(const char* _Chain)
	{
		int i = 0;

		while( _Chain[i] > 47 && _Chain[i] < 58 )
			i++;

		return i;
	}

	unsigned long int myatoi(const char* _Chain)
	{
		int i, iResultNumber = 0;
		
		//R�cup�re la longueur de la cha�ne de caract�res
		const int iChainLength = mystrnumlen(_Chain);

		//R�cup�re les nombres dans la cha�ne de caract�res, et construit la valeur num�rique
		for( i = 0; i < iChainLength; i++ )
		{
			if( _Chain[i] > 47 && _Chain[i] < 58 )	//Table ASCII, 0 = 48, 9 = 57
				iResultNumber += (int)(_Chain[i] - 48) * (Pow(10, iChainLength - (i + 1)));
			else
				i = iChainLength;
		}
	
		return iResultNumber;
	}

	const int mystrcmp(const char* _ChainOne, const char* _ChainTwo)
	{
		int i, iStrLengthOne = mystrlen(_ChainOne), iStrLengthTwo = mystrlen(_ChainTwo);

		if( iStrLengthOne != iStrLengthTwo )
			return 2;

		for( i = 0; i < iStrLengthOne; i++ )
		{
			if( _ChainOne[i] < _ChainTwo[i] )
				return -1;
			else if( _ChainOne[i] > _ChainTwo[i] )
				return 1;
		}

		return 0;
	}

	char* mystrcpy(const char* _Source, char* _Dest, const size_t _CharacterToCopy, const unsigned int _DestSize)
	{
		unsigned int i;

		//Copie le nombre de caract�re sp�cifi�, dans la limite de la taille du buffer
		for( i = 0; i < _CharacterToCopy && i < _DestSize; i++ )
			_Dest[i] = _Source[i];

		//Place un caract�re de fin de cha�ne en bout de cha�ne
		i < _DestSize ? _Dest[i] = '\0' : _Dest[_DestSize - 1] = '\0';

		return _Dest;
	}

	char* mystrcat(const char* _Source, char* _Dest)
	{
		char* szEndOfString = mystrchr(_Dest, '\0');
		int i = 0;

		while( _Source[i] != '\0' )
		{
			szEndOfString[i] = _Source[i];
			i++;
		}
	
		szEndOfString[i] = '\0';

		return _Dest;
	}

	char* mystrchr(const char* _Chain, int _Character)
	{
		char cCharacter = (char)_Character;
		int i, iStrLen = mystrlen(_Chain) + 1; //+1 permet de chercher le caract�re de fin de cha�ne

		for( i = 0; i < iStrLen; i++ )
		{
			if( _Chain[i] == cCharacter )
				return (char*)&_Chain[i];
		}

		return NULL;
	}

	char* mystrtok(char* _String, const char* _Tokens)
	{
		int i = 0, j = 0, iStrLength = mystrlen(_String), iTokLength = mystrlen(_Tokens);
		bool bBoolean = false;

		if( _String != NULL )
		{
			//Identifie le d�but de cha�ne
				//Parcours la cha�ne de caract�res
				for( i = 0; i < iStrLength; i++ )
				{
					bBoolean = false;

					//Parcoure les tokens
					for( j = 0; j < iTokLength; j++ )
					{
						if( _String[i] == _Tokens[j] )
							bBoolean = true;
					}

					//Si pas de token correspondant, on a le d�but de la cha�ne
					if( !bBoolean )
					{
						_String = &_String[i];
						i = iStrLength;
					}
				}

			//Identifie la fin de la cha�ne et y place un caract�re de fin de cha�ne
				i = 0;
				//Parcours la cha�ne de caract�res
				while( _String[i] != '\0' )
				{
					//Parcoure les tokens
					for( j = 0; j < iTokLength; j++ )
					{
						if( _String[i] == _Tokens[j] )
						{
							_String[i] = '\0';
							return _String;
						}
					}

					i++;
				}
		}

		return _String;
	}
}