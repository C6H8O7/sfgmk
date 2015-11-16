namespace sfgmk
{
	int getNumberOfFileInDir(const char* _DirPath)
	{
		DIR* LevelRepertory = NULL;
		struct dirent* ReadFile = NULL;
		unsigned int uiNumberOfFile(0U);

		LevelRepertory = opendir(_DirPath);

		//Si erreur ouverture
		if( LevelRepertory == NULL )
		{
			if(SFGML_ENABLE_DIR_DEBUG)
				perror("Dossier introuvable");
			return 0;
		}

		//Si dossier trouvé
		else
		{
			while( (ReadFile = readdir(LevelRepertory)) != NULL ) //On récupére tous les fichiers contenus
				++uiNumberOfFile;

			closedir(LevelRepertory);
			return( uiNumberOfFile - 2U );
		}
	}

	int getNumberOfFileInDir(const std::string& _DirPath)
	{
		return getNumberOfFileInDir(_DirPath.c_str());
	}


	bool goToChar(FILE* _File, const int& _Char)
	{
		int iBuffer(0);

		while( (iBuffer = fgetc(_File)) != EOF )
		{
			if( iBuffer == _Char )
				return true;
		}

		return false;
	}


	stSTRING_ARRAY* cutStringInPart(const std::string& _String, const unsigned int& _MaxSize)
	{
		stSTRING_ARRAY* Strings = new stSTRING_ARRAY;

		//Nombre de parts
		Strings->uiStringNumber = _String.size() / _MaxSize;
		if( (_String.length() % _MaxSize) != 0 )
			Strings->uiStringNumber++;

		//Création des parts
		Strings->sStrings = new std::string[Strings->uiStringNumber];
		for( int i(0); i < Strings->uiStringNumber; i++ )
			Strings->sStrings[i] = std::string(_String.substr(i * _MaxSize, _MaxSize));

		return Strings;
	}
}