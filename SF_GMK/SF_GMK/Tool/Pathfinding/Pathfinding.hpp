/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		27/11/2015
	@brief		Classe premettant de calculer un chemin en utilisant différents algos

--------------------------------------------------------------------------------------------------*/

#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP


namespace sfgmk
{
	typedef std::vector<sf::Vector2i> PathfindingPathCntr;

	enum ePATHFINDING_ALGOS
	{
		eZpath = 0,
		eDijkstra,
		eAStar,
		eJps,
		ePATHFINDING_ALGOS_NUMBER
	};

	class SFGMK_API Pathfinding
	{
		public:
			Pathfinding();
			~Pathfinding();

		private:
			enum eNEXT_CASES
			{
				eTop = 0,
				eRight,
				eBot,
				eLeft,
				eNEXT_CASES_NUMBER_4 = 4,

				eTopRight = 4,
				eBotRight,
				eBotLeft,
				eTopLeft,
				eNEXT_CASES_NUMBER_8
			};
			struct stPATHFINDING_NODE
			{
				sf::Vector2i GridCoords;
				stPATHFINDING_NODE* ParentPtr;
				sf::Vector2i DirectionFromParent;

				float fCostSoFar;
				float fHeuristic;
				float fEstimatedTotalCost;

				stPATHFINDING_NODE(const sf::Vector2i& _GridCoord, stPATHFINDING_NODE* _Parent = NULL, const sf::Vector2i& DirectionFromParent = sf::Vector2i(0, 0), float _CostSoFar = 0.0f, float _Heuristic = 0.0f, float _EstimatedTotalCost = 0.0f)
					:  GridCoords(_GridCoord), ParentPtr(_Parent), DirectionFromParent(DirectionFromParent), fCostSoFar(_CostSoFar), fHeuristic(_Heuristic), fEstimatedTotalCost(_EstimatedTotalCost) {}
			};

			sf::Vector2i m_PrecomputedNextCases[eNEXT_CASES_NUMBER_8];
			sf::Vector2i m_ExpandedNodes[eNEXT_CASES_NUMBER_8];

			FoncterTemplateArray m_Algorithms;
			std::string m_sAlgosNames[ePATHFINDING_ALGOS_NUMBER];

			PathfindingMap* m_Map;
			stPATHFINDING_SIMPLIFIED_NODE** m_SimplifiedMap;
			sf::Vector2i m_Begin, m_End, m_Size;
			PathfindingPathCntr* m_Path;
			bool m_bStartFound;
			unsigned int m_uiStep;
			unsigned int m_uiCasesTested;
			sf::Clock m_Clock;
			sf::Int64 m_ElapsedTime;

			//Z-Path
			std::queue<sf::Vector2i> m_ZPathList;
			sf::Vector2i m_CasesToTest[2];

			//A*
			std::list<stPATHFINDING_NODE*> m_JpsOpenList;
			std::list<stPATHFINDING_NODE*> m_JpsCloseList;

		public:
			void computePathfinding(PathfindingPathCntr* _Path, const ePATHFINDING_ALGOS& _Algo, PathfindingMap* _Map, const sf::Vector2i& _Begin, const sf::Vector2i& _End);

		private:
			void allocSimplifiedMap();
			void desallocSimplifiedMap();

			inline bool Pathfinding::checkDiagonalWall(const sf::Vector2i& _CaseOne, const sf::Vector2i& _CaseTwo)
			{
				m_CasesToTest[0] = sf::Vector2i(_CaseOne.x, _CaseTwo.y);
				m_CasesToTest[1] = sf::Vector2i(_CaseTwo.x, _CaseOne.y);

				if( m_SimplifiedMap[m_CasesToTest[0].x][m_CasesToTest[0].y].bIswall && m_SimplifiedMap[m_CasesToTest[1].x][m_CasesToTest[1].y].bIswall )
					return true;
				else
					return false;
			}

			//Z-Path
			inline int Pathfinding::computeNextCases4(const sf::Vector2i& _CurrentCase, sf::Vector2i _Array[eNEXT_CASES_NUMBER_4])
			{
				int iIndex(0);

				for( int i(0); i < eNEXT_CASES_NUMBER_4; i++ )
				{
					_Array[iIndex] = _CurrentCase + m_PrecomputedNextCases[i];

					//Si la case est dans la map
					if( _Array[iIndex].x >= 0 && _Array[iIndex].x < m_Size.x && _Array[iIndex].y >= 0 && _Array[iIndex].y < m_Size.y )
					{
						//Si la case n'est pas un mur et n'a pas été testée
						if( !(m_SimplifiedMap[_Array[iIndex].x][_Array[iIndex].y].bIswall) && !(m_SimplifiedMap[_Array[iIndex].x][_Array[iIndex].y].bTested) )
							iIndex++;
					}
				}

				return iIndex;
			}
			inline int Pathfinding::computeNextCases8(const sf::Vector2i& _CurrentCase, sf::Vector2i _Array[eNEXT_CASES_NUMBER_8])
			{
				int iIndex(0);

				for( int i(0); i < eNEXT_CASES_NUMBER_8; i++ )
				{
					_Array[iIndex] = _CurrentCase + m_PrecomputedNextCases[i];

					//Si la case est dans la map
					if( _Array[iIndex].x >= 0 && _Array[iIndex].x < m_Size.x && _Array[iIndex].y >= 0 && _Array[iIndex].y < m_Size.y )
						iIndex++;
				}

				return iIndex;
			}
			void zPath();

			//Dijkstra
			void sortDijkstra();
			void dijkstra();

			//A*
			inline bool isInCases(const sf::Vector2i& _Position)
			{
				return (_Position.x >= 0 && _Position.x < m_Size.x && _Position.y >= 0 && _Position.y < m_Size.y);
			}
			bool Pathfinding::isWall(const sf::Vector2i& _Position)
			{
				return m_SimplifiedMap[_Position.x][_Position.y].bIswall;
			}
			inline void astar_compute_next_cases(sf::Vector2i _current, sf::Vector2i _cases[8], int* _validCases);
			inline int astar_search_in_list(const sf::Vector2i& _node, std::vector<stPATHFINDING_NODE*>& _list);
			inline void astar_remove_from_list(stPATHFINDING_NODE* _node, std::vector<stPATHFINDING_NODE*>& _list, bool _delete);
			inline stPATHFINDING_NODE* astar_find_smallest(std::vector<stPATHFINDING_NODE*>& _list);
			inline float astar_heuristic(const sf::Vector2i& _node); 
			void aStar();

			//Jps
			inline void jps_forced_neighbours(sf::Vector2i& _current, sf::Vector2i& _dir, sf::Vector2i _forced[8], int* _forcedCount);
			inline sf::Vector2i* jps_jump(sf::Vector2i& _current, sf::Vector2i& _start, sf::Vector2i& _end, sf::Vector2i& _dir);
			inline void jps_identify_successors(sf::Vector2i& _current, sf::Vector2i& _start, sf::Vector2i& _end, sf::Vector2i* _successors, int* _validSuccessors);
			void jps();
	};
}


#endif