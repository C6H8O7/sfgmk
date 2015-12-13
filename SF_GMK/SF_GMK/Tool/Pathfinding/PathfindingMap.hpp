/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		12/12/2015
	@brief		Map sous forme de grille pour pathfinding

--------------------------------------------------------------------------------------------------*/

#ifndef PATHFINDING_MAP_HPP
#define PATHFINDING_MAP_HPP


namespace sfgmk
{
	enum ePATHFINDING_TERRAIN_TYPE
	{
		eOUT_OF_MAP = -1,
		eWALL,
		eGROUND
	};

	
	struct stPATHFINDING_SIMPLIFIED_NODE
	{
		bool bTested;
		bool bIswall;
		unsigned int uiStep;
	};

	class SFGMK_API PathfindingMap
	{
		public:
			PathfindingMap();
			~PathfindingMap();

		private:
			int* m_Map;
			int m_Width, m_Height;
			stPATHFINDING_SIMPLIFIED_NODE** m_SimplifiedMap;

			void freeMaps();

		public:
			inline int getMapWidth() const { return m_Width; }
			inline int getMapHeight() const { return m_Height; }
			inline int getIndex(const int& _X, const int& _Y)
			{
				if( _X < 0 || _X >= m_Width ) return -1;
				if( _Y < 0 || _Y >= m_Height ) return -1;
				return _Y * m_Width + _X;
			}

			bool loadMapFromFile(const char* _FileName);

			int* getMap();
			stPATHFINDING_SIMPLIFIED_NODE** getSimplifiedMap();

			int getTerrainType(const int& _X, const int& _Y);
			void setTerrainType(const int& _X, const int& _Y, const ePATHFINDING_TERRAIN_TYPE& _Type);
	};
}


#endif