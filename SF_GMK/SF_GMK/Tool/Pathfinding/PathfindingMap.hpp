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
		eWALL = 0,
		eGROUND = 1
	};

	class SFGMK_API PathfindingMap
	{
		public:
			PathfindingMap();
			~PathfindingMap();

		private:
			sf::Uint8* m_uiMap;
			sf::Vector2i m_Size;
			unsigned int m_uiCaseNumber;

			void freeMap();

		public:
			inline int getIndex(const sf::Vector2i& _Position)
			{
				return _Position.y * m_Size.x + _Position.x;
			}
			inline int getSafeIndex(const sf::Vector2i& _Position)
			{
				if( !isInMap(_Position) ) return eOUT_OF_MAP;
				return _Position.y * m_Size.x + _Position.x;
			}
			inline bool isInMap(const sf::Vector2i& _Position)
			{
				if( _Position.x < 0 || _Position.x >= m_Size.x ) return false;
				if( _Position.y < 0 || _Position.y >= m_Size.y ) return false;
				return true;
			}
			inline int getTerrainType(const sf::Vector2i& _Position)
			{
				int iIndex = getIndex(_Position);
				if( iIndex == eOUT_OF_MAP )
					return iIndex;
				return m_uiMap[iIndex];
			}
			inline bool isWall(const int& _Index)
			{
				return (m_uiMap[_Index] == eWALL);
			}
			inline bool checkDiagonalWall(const sf::Vector2i& _CaseOne, const sf::Vector2i& _CaseTwo)
			{
				sf::Vector2i m_CasesToTest[2] = { sf::Vector2i(_CaseOne.x, _CaseTwo.y), sf::Vector2i(_CaseTwo.x, _CaseOne.y) };
			
				if( isWall(getIndex(m_CasesToTest[0])) && isWall(getIndex(m_CasesToTest[1])) )
					return true;
				else
					return false;
			}

			sf::Uint8* getMap();
			const sf::Vector2i& getSize();
			const unsigned int& getCaseNumber();

			bool loadMapFromFile(const char* _FileName, sf::Vector2i& _Begin, sf::Vector2i& _End);

			void setTerrainType(const int& _X, const int& _Y, const ePATHFINDING_TERRAIN_TYPE& _Type);
	};
}


#endif