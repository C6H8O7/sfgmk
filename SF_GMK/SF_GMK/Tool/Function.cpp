namespace sfgmk
{
	int getNumberOfFileInDir(const char* _DirPath)
	{
		DIR* LevelRepertory = NULL;
		struct dirent* ReadFile = NULL;
		int iNumberOfFile(0);

		LevelRepertory = opendir(_DirPath);

		//Si erreur ouverture
		if( LevelRepertory == NULL )
		{
			perror("Dossier introuvable");
			return 0;
		}

		//Si dossier trouvé
		else
		{
			while( (ReadFile = readdir(LevelRepertory)) != NULL ) //On récupére tous les fichiers contenus
				++iNumberOfFile;

			closedir(LevelRepertory);
			return (iNumberOfFile - 2);
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
}