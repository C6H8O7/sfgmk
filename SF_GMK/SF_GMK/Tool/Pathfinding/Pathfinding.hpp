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

			enum eEXPANDED_CASES
			{
				eTop = 0,
				eRight,
				eBot,
				eLeft,
				eEXPANDED_CASES_NUMBER_4 = 4,

				eTopRight = 4,
				eBotRight,
				eBotLeft,
				eTopLeft,
				eEXPANDED_CASES_NUMBER_8
			};

			enum eLIST_POSITION
			{
				eNone = 0,
				eOpenList,
				eCloseList
			};

			struct stEXPANDED_NODES
			{
				sf::Vector2i Coords;
				int iIndex;
			};

			struct stPATHFINDING_NODE
			{
				sf::Vector2i GridCoords;
				stPATHFINDING_NODE* ParentPtr;
				sf::Vector2i DirectionFromParent;

				float fCostSoFar;
				float fHeuristic;
				float fEstimatedTotalCost;

				stPATHFINDING_NODE(const sf::Vector2i& _GridCoord, stPATHFINDING_NODE* _Parent = NULL, const sf::Vector2i& DirectionFromParent = sf::Vector2i(0, 0), const float& _CostSoFar = 0.0f, const float& _Heuristic = 0.0f, const float& _EstimatedTotalCost = 0.0f)
					:  GridCoords(_GridCoord), ParentPtr(_Parent), DirectionFromParent(DirectionFromParent), fCostSoFar(_CostSoFar), fHeuristic(_Heuristic), fEstimatedTotalCost(_EstimatedTotalCost) {}
			};

			struct stPATHFINDING_GRID_NODE
			{
				bool bTested;
				unsigned int uiStep;
				eLIST_POSITION List;
			};

		private:
			sf::Vector2i m_PrecomputedNextCases[eEXPANDED_CASES_NUMBER_8];
			stEXPANDED_NODES m_ExpandedNodes[eEXPANDED_CASES_NUMBER_8];

			FoncterTemplateArray m_Algorithms;
			std::string m_sAlgosNames[ePATHFINDING_ALGOS_NUMBER];

			PathfindingMap* m_Map;
			stPATHFINDING_GRID_NODE* m_NodeMap;
			sf::Vector2i m_Begin, m_End, m_Size;
			PathfindingPathCntr* m_Path;
			bool m_bGoalFound;
			unsigned int m_uiStep;
			sf::Clock m_Clock;
			sf::Int64 m_ElapsedTime;

			//Z-Path
			std::queue<sf::Vector2i> m_ZPathList;

			//A*
			std::list<stPATHFINDING_NODE*> m_OpenList;
			std::list<stPATHFINDING_NODE*> m_CloseList;

		public:
			void computePathfinding(PathfindingPathCntr* _Path, const ePATHFINDING_ALGOS& _Algo, PathfindingMap* _Map, const sf::Vector2i& _Begin, const sf::Vector2i& _End, const bool& _SmoothPath = false, const float& _CaseSize = 0.0f);
			void smoothPath(PathfindingPathCntr* _Path, const float& _CaseSize);

			stPATHFINDING_GRID_NODE* getGridNodeMap() { return m_NodeMap; }

		private:
			inline void allocNodeMap()
			{
				m_NodeMap = (stPATHFINDING_GRID_NODE*)calloc(m_Map->getCaseNumber(), sizeof(stPATHFINDING_GRID_NODE));
			}
			inline void resetNodeMap()
			{
				memset(m_NodeMap, 0, sizeof(stPATHFINDING_GRID_NODE) * m_Map->getCaseNumber());
			}
			inline void releaseNodeMap()
			{
				if( m_NodeMap != NULL )
				{
					free(m_NodeMap);
					m_NodeMap = NULL;
				}
			}

			inline int computeExpandedCases4(const sf::Vector2i& _CurrentCase)
			{
				int iArrayIndex(0);

				for( int i(0); i < eEXPANDED_CASES_NUMBER_4; i++ )
				{
					m_ExpandedNodes[iArrayIndex].Coords = _CurrentCase + m_PrecomputedNextCases[i];

					//Si la case est dans la map
					if( (m_ExpandedNodes[iArrayIndex].iIndex = m_Map->getSafeIndex(m_ExpandedNodes[iArrayIndex].Coords)) != eOUT_OF_MAP )
					{
						//Si la case n'est pas un mur
						if( !(m_Map->isWall(m_ExpandedNodes[iArrayIndex].iIndex)) )
							iArrayIndex++;
					}
				}

				return iArrayIndex;
			}
			inline int computeExpandedCases8(const sf::Vector2i& _CurrentCase)
			{
				int iArrayIndex(0);

				for( int i(0); i < eEXPANDED_CASES_NUMBER_8; i++ )
				{
					m_ExpandedNodes[iArrayIndex].Coords = _CurrentCase + m_PrecomputedNextCases[i];

					//Si la case est dans la map
					if( (m_ExpandedNodes[iArrayIndex].iIndex = m_Map->getSafeIndex(m_ExpandedNodes[iArrayIndex].Coords)) != eOUT_OF_MAP )
					{
						//Si la case n'est pas un mur
						if( !(m_Map->isWall(m_ExpandedNodes[iArrayIndex].iIndex)) )
						{
							//Si on ne traverse pas un mur en diagonale
							if( i < eEXPANDED_CASES_NUMBER_4 || !m_Map->checkDiagonalWall(_CurrentCase, m_ExpandedNodes[iArrayIndex].Coords) )
								iArrayIndex++;
						}
					}
				}

				return iArrayIndex;
			}

			inline stPATHFINDING_NODE* findSmallestNode(std::list<stPATHFINDING_NODE*>& _list)
			{
				float value = -1.0f;
				stPATHFINDING_NODE* node = 0;

				for( stPATHFINDING_NODE*& _node : _list )
				{
					if( _node->fEstimatedTotalCost < value || value < 0.0f )
					{
						node = _node;
						value = _node->fEstimatedTotalCost;
					}
				}
				return node;
			}
			inline stPATHFINDING_NODE* searchInList(const sf::Vector2i& _node, std::list<stPATHFINDING_NODE*>& _list)
			{
				for( stPATHFINDING_NODE*& Temp : _list )
					if( _node == Temp->GridCoords )
						return Temp;

				return NULL;
			}

			inline void forcedNeighbours(sf::Vector2i& _current, sf::Vector2i& _dir, sf::Vector2i _forced[8], int* _forcedCount)
			{
				sf::Vector2i testWall;

				// Diagonal
				if( _dir.x && _dir.y )
				{
					testWall = sf::Vector2i(_current.x - _dir.x, _current.y);
					if( m_Map->getSafeIndex(testWall) != eOUT_OF_MAP && m_Map->isWall(m_Map->getSafeIndex(testWall)) )
					{
						sf::Vector2i forced(_current.x - _dir.x, _current.y + _dir.y);

						if( m_Map->getSafeIndex(forced) != eOUT_OF_MAP && !m_Map->isWall(m_Map->getSafeIndex(forced)) )
							_forced[(*_forcedCount)++] = forced;
					}
					testWall = sf::Vector2i(_current.x, _current.y - _dir.y);
					if( m_Map->getSafeIndex(testWall) != eOUT_OF_MAP && m_Map->isWall(m_Map->getSafeIndex(testWall)) )
					{
						sf::Vector2i forced(_current.x + _dir.x, _current.y - _dir.y);

						if( m_Map->getSafeIndex(forced) != eOUT_OF_MAP && !m_Map->isWall(m_Map->getSafeIndex(forced)) )
							_forced[(*_forcedCount)++] = forced;
					}
				}
				// Horizontal
				else if( _dir.x )
				{
					testWall = sf::Vector2i(_current.x, _current.y - 1);
					if( m_Map->getSafeIndex(testWall) != eOUT_OF_MAP && m_Map->isWall(m_Map->getSafeIndex(testWall)) )
					{
						sf::Vector2i forced(_current.x + _dir.x, _current.y - 1);

						if( m_Map->getSafeIndex(forced) != eOUT_OF_MAP && !m_Map->isWall(m_Map->getSafeIndex(forced)) )
							_forced[(*_forcedCount)++] = forced;
					}
					testWall = sf::Vector2i(_current.x, _current.y + 1);
					if( m_Map->getSafeIndex(testWall) != eOUT_OF_MAP && m_Map->isWall(m_Map->getSafeIndex(testWall)) )
					{
						sf::Vector2i forced(_current.x + _dir.x, _current.y + 1);

						if( m_Map->getSafeIndex(forced) != eOUT_OF_MAP && !m_Map->isWall(m_Map->getSafeIndex(forced)) )
							_forced[(*_forcedCount)++] = forced;
					}
				}
				// Vertical
				else if( _dir.y )
				{
					testWall = sf::Vector2i(_current.x - 1, _current.y);
					if( m_Map->getSafeIndex(testWall) != eOUT_OF_MAP && m_Map->isWall(m_Map->getSafeIndex(testWall)) )
					{
						sf::Vector2i forced(_current.x - 1, _current.y + _dir.y);

						if( m_Map->getSafeIndex(forced) != eOUT_OF_MAP && !m_Map->isWall(m_Map->getSafeIndex(forced)) )
							_forced[(*_forcedCount)++] = forced;
					}
					testWall = sf::Vector2i(_current.x + 1, _current.y);
					if( m_Map->getSafeIndex(testWall) != eOUT_OF_MAP && m_Map->isWall(m_Map->getSafeIndex(testWall)) )
					{
						sf::Vector2i forced(_current.x + 1, _current.y + _dir.y);

						if( m_Map->getSafeIndex(forced) != eOUT_OF_MAP && !m_Map->isWall(m_Map->getSafeIndex(forced)) )
							_forced[(*_forcedCount)++] = forced;
					}
				}
			}
			inline void neighbours(sf::Vector2i& _current, sf::Vector2i& _dir, sf::Vector2i _neighbours[8], int* _neighboursCount)
			{
				if( _dir.x && _dir.y )
				{
					sf::Vector2i n[3];
					n[0] = sf::Vector2i(_current.x, _current.y + _dir.y);
					n[1] = sf::Vector2i(_current.x + _dir.x, _current.y);
					n[2] = sf::Vector2i(_current.x + _dir.x, _current.y + _dir.y);

					for( int i = 0; i < 3; i++ )
						if( m_Map->getSafeIndex(n[i]) != eOUT_OF_MAP && !m_Map->isWall(m_Map->getSafeIndex(n[i])) )
							_neighbours[(*_neighboursCount)++] = n[i];
				}
				else
				{
					sf::Vector2i n(_current.x + _dir.x, _current.y + _dir.y);

					if( m_Map->getSafeIndex(n) != eOUT_OF_MAP && !m_Map->isWall(m_Map->getSafeIndex(n)) )
						_neighbours[(*_neighboursCount)++] = n;
				}
			}
			inline void allNeighbours(sf::Vector2i& _current, sf::Vector2i& _dir, sf::Vector2i _allNeighbours[8], int* _allNeighboursCount)
			{
				int countNormal = 0, countForced = 0;
				sf::Vector2i normal[8], forced[8];

				neighbours(_current, _dir, normal, &countNormal);
				forcedNeighbours(_current, _dir, forced, &countForced);

				for( int i = 0; i < countNormal; i++ )
					_allNeighbours[(*_allNeighboursCount)++] = normal[i];

				for( int i = 0; i < countForced; i++ )
					_allNeighbours[(*_allNeighboursCount)++] = forced[i];
			}
			inline sf::Vector2i* jump(sf::Vector2i& _current, sf::Vector2i& _start, sf::Vector2i& _end, sf::Vector2i& _dir)
			{
				stEXPANDED_NODES next = { _current + _dir, m_Map->getSafeIndex(_current + _dir) };
		
				if( next.iIndex == eOUT_OF_MAP || m_Map->isWall(next.iIndex) )
					return 0;

				// On vérifie qu'on est pas tombés sur l'arrivée
				if( next.Coords == _end )
					return new sf::Vector2i(_end);

				// Vérification voisin forcé
				int forced_count = 0;
				sf::Vector2i forced[8];
				forcedNeighbours(_current, _dir, forced, &forced_count);

				// Si il y a un voisin forcé, on ajoute le point de saut
				if( forced_count )
					return new sf::Vector2i(next.Coords);

				// Cas en diagonal
				if( _dir.x && _dir.y )
				{
					if( jump(next.Coords, _start, _end, sf::Vector2i(_dir.x, 0))
					   || jump(next.Coords, _start, _end, sf::Vector2i(0, _dir.y)) )
					{
						return new sf::Vector2i(next.Coords);
					}
				}

				return jump(next.Coords, _start, _end, _dir);
			}
			inline void identifySuccessors(sf::Vector2i& _current, sf::Vector2i& _start, sf::Vector2i& _end, int* _validSuccessors)
			{
				int iValid_expanded_nodes = 0;

				// On récupère tous les voisins valables
				float dx = (float)(_end.x - _start.x);
				float dy = (float)(_end.y - _start.y);

				sf::Vector2i dir((int)(dx / ABS(dx)), (int)(dy / ABS(dy)));
				//jps_all_neighbours(_current, dir, expanded_nodes, &valid_expanded_nodes);
				iValid_expanded_nodes = computeExpandedCases8(_current);

				sf::Vector2i direction;

				for( int i = 0; i < iValid_expanded_nodes; i++ )
				{
					sf::Vector2i& expanded_node = m_ExpandedNodes[i].Coords;

					direction = expanded_node - _current;

					// On tente de trouver un nouveau point de saut
					sf::Vector2i* jumpPoint = jump(_current, _start, _end, direction);

					if( jumpPoint )
					{
						m_ExpandedNodes[(*_validSuccessors)].iIndex = m_Map->getIndex(*jumpPoint);
						m_ExpandedNodes[(*_validSuccessors)++].Coords = *jumpPoint;
					}
				}
			}

			void zPath();
			void dijkstra();
			void aStar();
			void jps();
	};
}


#endif