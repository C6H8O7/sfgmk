namespace sfgmk
{
	PathfindingMap::PathfindingMap() : m_Map(NULL), m_Width(0), m_Height(0), m_SimplifiedMap(NULL)
	{
	}

	PathfindingMap::~PathfindingMap()
	{
		freeMaps();
	}


	void PathfindingMap::freeMaps()
	{
		if( m_SimplifiedMap != NULL )
		{
			for( int i(0); i < m_Width; i++ )
			{
				free(m_SimplifiedMap[i]);
				m_SimplifiedMap[i] = NULL;
			}
			free(m_SimplifiedMap);
			m_SimplifiedMap = NULL;
		}

		if( m_Map != NULL )
			free(m_Map);
	}


	bool PathfindingMap::loadMapFromFile(const char* _FileName)
	{
		freeMaps();

		FILE* FileToLoad = NULL;
		fopen_s(&FileToLoad, _FileName, "r");
		if( !FileToLoad )
		{
			std::cout << "File " << _FileName << " not found." << std::endl;
			return false;
		}
		
		//Chargement
		std::cout << "Load file " << _FileName << std::endl;

		fscanf_s(FileToLoad, "%d %d", &m_Width, &m_Height);
		m_Map = (int*)malloc(sizeof(int) * (m_Width * m_Height));

		m_SimplifiedMap = (stPATHFINDING_SIMPLIFIED_NODE**)calloc(m_Width, sizeof(stPATHFINDING_SIMPLIFIED_NODE*));
		if( m_SimplifiedMap == NULL )
			std::cout << "Error" << std::endl;
		else
		{
			for( int i(0); i < m_Width; i++ )
			{
				m_SimplifiedMap[i] = (stPATHFINDING_SIMPLIFIED_NODE*)calloc(m_Height, sizeof(stPATHFINDING_SIMPLIFIED_NODE));
				if( m_SimplifiedMap[i] == NULL )
					std::cout << "Error" << std::endl;
			}
		}

		char cBuffer = '\0';
		int iIteration(0);
		int iX(0), iY(0);

		fgetc(FileToLoad); //'\n' première ligne
		while( (cBuffer = fgetc(FileToLoad)) != EOF )
		{
			if( !(cBuffer == '\n') )
			{
				if( cBuffer == 48 )
				{
					m_Map[iIteration] = eGROUND;
					m_SimplifiedMap[iX][iY].bIswall = false;
				}
				else if( cBuffer == 49 )
				{
					m_Map[iIteration] = eWALL;
					m_SimplifiedMap[iX][iY].bIswall = true;
				}

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


	int* PathfindingMap::getMap()
	{
		return m_Map;
	}

	stPATHFINDING_SIMPLIFIED_NODE** PathfindingMap::getSimplifiedMap()
	{
		return m_SimplifiedMap;
	}


	int PathfindingMap::getTerrainType(const int& _X, const int& _Y)
	{
		int iIndex = getIndex(_X, _Y);
		if( iIndex == -1 )
			return eOUT_OF_MAP;
		
		return m_Map[iIndex];
	}

	void PathfindingMap::setTerrainType(const int& _X, const int& _Y, const ePATHFINDING_TERRAIN_TYPE& _Type)
	{
		int iIndex = getIndex(_X, _Y);
		if( !(iIndex == -1) )
		{
			m_Map[iIndex] = _Type;
			if( _Type == eGROUND )
				m_SimplifiedMap[_X][_Y].bIswall = false;
			if( _Type == eWALL )
				m_SimplifiedMap[_X][_Y].bIswall = true;
		}
	}
}