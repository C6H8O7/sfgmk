namespace sfgmk
{
	PathfindingMap::PathfindingMap() : m_uiMap(NULL), m_Size(0, 0), m_uiCaseNumber(0U)
	{
	}

	PathfindingMap::~PathfindingMap()
	{
		freeMap();
	}


	void PathfindingMap::freeMap()
	{
		if( m_uiMap != NULL )
		{
			free(m_uiMap);
			m_uiMap = NULL;
		}
	}


	sf::Uint8* PathfindingMap::getMap()
	{
		return m_uiMap;
	}

	const sf::Vector2i& PathfindingMap::getSize()
	{
		return m_Size;
	}

	const unsigned int& PathfindingMap::getCaseNumber()
	{
		return m_uiCaseNumber;
	}


	bool PathfindingMap::loadMapFromFile(const char* _FileName, sf::Vector2i& _Begin, sf::Vector2i& _End)
	{
		freeMap();

		FILE* FileToLoad = NULL;
		fopen_s(&FileToLoad, _FileName, "r");
		if( !FileToLoad )
		{
			std::cout << "File " << _FileName << " not found." << std::endl;
			return false;
		}
		
		//Chargement:
		std::cout << "Load file " << _FileName << std::endl;

		//Taille de la map
		fscanf_s(FileToLoad, "%d %d", &m_Size.x, &m_Size.y);
		m_uiMap = (sf::Uint8*)malloc(sizeof(sf::Uint8) * (m_Size.x * m_Size.y));

		if( m_uiMap == NULL )
		{
			std::cout << "Error" << std::endl;
			return false;
		}
		
		//Begin et end
		char cBuffer = '\0';

		while( (cBuffer = fgetc(FileToLoad)) != '\n' );
		fscanf_s(FileToLoad, "%d", &_Begin.x);
		cBuffer = fgetc(FileToLoad);
		fscanf_s(FileToLoad, "%d", &_Begin.y);

		while( (cBuffer = fgetc(FileToLoad)) != '\n' );
		fscanf_s(FileToLoad, "%d", &_End.x);
		cBuffer = fgetc(FileToLoad);
		fscanf_s(FileToLoad, "%d", &_End.y);

		while( (cBuffer = fgetc(FileToLoad)) != '\n' );

		//Valeurs des cases
		unsigned int uiCaseNumber = m_uiCaseNumber = m_Size.x * m_Size.y;
		int iIteration(0);
		int iX(0), iY(0);

		while( (cBuffer = fgetc(FileToLoad)) != EOF && uiCaseNumber > 0U )
		{
			if( !(cBuffer == '\n') )
			{
				uiCaseNumber--;

				m_uiMap[iIteration] = cBuffer + ASCII_NUMBER_GAP;
				
				iIteration++;
				iX++;
			}
			else
			{
				iY++;
				iX = 0;
			}
		}
		
		fclose(FileToLoad);
		return true;
	}


	void PathfindingMap::setTerrainType(const int& _X, const int& _Y, const ePATHFINDING_TERRAIN_TYPE& _Type)
	{
		int iIndex = getIndex(sf::Vector2i(_X, _Y));
		if( !(iIndex == eOUT_OF_MAP) )
			m_uiMap[iIndex] = _Type;
	}
}