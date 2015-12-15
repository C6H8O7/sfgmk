namespace sfgmk
{
	Pathfinding::Pathfinding() : m_Map(false), m_Begin(sf::Vector2i(0, 0)), m_End(sf::Vector2i(0, 0)), m_Size(sf::Vector2i(0, 0)), m_Path(NULL), m_bStartFound(false), m_uiStep(0U), m_uiCasesTested(0U), m_ElapsedTime(0), m_SimplifiedMap(NULL)
	{
		//Precompute
		m_PrecomputedNextCases[eTop] = sf::Vector2i(0, -1);
		m_PrecomputedNextCases[eRight] = sf::Vector2i(1, 0);
		m_PrecomputedNextCases[eBot] = sf::Vector2i(0, 1);
		m_PrecomputedNextCases[eLeft] = sf::Vector2i(-1, 0);
		m_PrecomputedNextCases[eTopRight] = sf::Vector2i(1, -1);
		m_PrecomputedNextCases[eBotRight] = sf::Vector2i(1, 1);
		m_PrecomputedNextCases[eBotLeft] = sf::Vector2i(-1, 1);
		m_PrecomputedNextCases[eTopLeft] = sf::Vector2i(-1, -1);

		memset(m_ExpandedNodes, 0, sizeof(sf::Vector2i) * eNEXT_CASES_NUMBER_8);

		//Algorithms foncters
		m_Algorithms.m_FunctionsArray.pushBack(new sfgmk::FoncterTemplateInstance<Pathfinding, void>(this, &Pathfinding::zPath));
		m_Algorithms.m_FunctionsArray.pushBack(new sfgmk::FoncterTemplateInstance<Pathfinding, void>(this, &Pathfinding::dijkstra));
		m_Algorithms.m_FunctionsArray.pushBack(new sfgmk::FoncterTemplateInstance<Pathfinding, void>(this, &Pathfinding::aStar));
		m_Algorithms.m_FunctionsArray.pushBack(new sfgmk::FoncterTemplateInstance<Pathfinding, void>(this, &Pathfinding::jps));
		m_Algorithms.m_FunctionsArray.pushBack(new sfgmk::FoncterTemplateInstance<Pathfinding, void>(this, &Pathfinding::aStarKcc));
		m_Algorithms.m_FunctionsArray.pushBack(new sfgmk::FoncterTemplateInstance<Pathfinding, void>(this, &Pathfinding::jpsKcc));

		//Strings algos names
		m_sAlgosNames[eZpath] = "Z-Path";
		m_sAlgosNames[eDijkstra] = "Dijkstra";
		m_sAlgosNames[eAStar] = "A*";
		m_sAlgosNames[eJps] = "Jps";
		m_sAlgosNames[eAStarKcc] = "A*Kcc";
		m_sAlgosNames[eJpsKcc] = "JpsKcc";
	}

	Pathfinding::~Pathfinding()
	{
		m_Algorithms.m_FunctionsArray.deleteAndClear();
	}


	void Pathfinding::computePathfinding(PathfindingPathCntr* _Path, const ePATHFINDING_ALGOS& _Algo, PathfindingMap* _Map, const sf::Vector2i& _Begin, const sf::Vector2i& _End)
	{
		//Reset variables
		m_bStartFound = false;
		m_uiStep = 0U;
		m_uiCasesTested = 0U;

		//Init
		m_Map = _Map;
		m_Begin = _Begin;
		m_End = _End;
		m_Size = sf::Vector2i(_Map->getMapWidth(), _Map->getMapHeight());
		m_Path = _Path;

		//Désalloue map simplifiée
		desallocSimplifiedMap();

		//Alloue map simplifiée
		allocSimplifiedMap();

		//Lance le pathfinding
		m_Clock.restart();
		m_Algorithms[_Algo];
		m_ElapsedTime = m_Clock.getElapsedTime().asMicroseconds();
		std::cout << m_sAlgosNames[_Algo] << " computed in " << m_ElapsedTime << " microseconds (" << m_ElapsedTime * 0.001f << " ms), with a total of " << m_uiStep << " steps and " << m_uiCasesTested << " cases tested.\nPath length: " << _Path->size() << std::endl << std::endl;
	}


	stPATHFINDING_SIMPLIFIED_NODE** Pathfinding::getSimplifiedMap()
	{
		return m_SimplifiedMap;
	}

	void Pathfinding::resetMap()
	{
		desallocSimplifiedMap();
		m_Map = NULL;
	}


	void Pathfinding::allocSimplifiedMap()
	{
		m_SimplifiedMap = (stPATHFINDING_SIMPLIFIED_NODE**)malloc(sizeof(stPATHFINDING_SIMPLIFIED_NODE*) * m_Size.x);
		if( m_SimplifiedMap == NULL )
			std::cout << "Error" << std::endl;
		else
		{
			for( int i(0); i < m_Size.x; i++ )
			{
				m_SimplifiedMap[i] = (stPATHFINDING_SIMPLIFIED_NODE*)malloc(sizeof(stPATHFINDING_SIMPLIFIED_NODE) * m_Size.y);
				if( m_SimplifiedMap[i] == NULL )
					std::cout << "Error" << std::endl;
			}
		}
		for( int i(0); i < m_Size.x; i++ )
			memcpy(m_SimplifiedMap[i], m_Map->getSimplifiedMap()[i], sizeof(stPATHFINDING_SIMPLIFIED_NODE) * m_Size.y);
	}

	void Pathfinding::desallocSimplifiedMap()
	{
		if( m_SimplifiedMap != NULL )
		{
			for( int i(0); i < m_Size.x; i++ )
			{
				free(m_SimplifiedMap[i]);
				m_SimplifiedMap[i] = NULL;
			}
			free(m_SimplifiedMap);
			m_SimplifiedMap = NULL;
		}
	}


	void Pathfinding::zPath()
	{
		unsigned int uiListCurrentSize(0U);
		stPATHFINDING_SIMPLIFIED_NODE* TempCase = NULL;
		sf::Vector2i* TempVector = NULL;
		int iExpandedNodesNumber(0);

		//On démarre de la fin
		m_SimplifiedMap[m_End.x][m_End.y].bTested = true;
		m_ZPathList.push(m_End);

		while( !m_bStartFound && (uiListCurrentSize = m_ZPathList.size()) )
		{
			m_uiStep++;

			for( unsigned int i(0U); i < uiListCurrentSize; i++ )
			{
				TempVector = &m_ZPathList.front();

				//Cases adjacentes
				iExpandedNodesNumber = computeNextCases4(*TempVector, m_ExpandedNodes);

				for( int j(0); j < iExpandedNodesNumber; j++ )
				{
					m_uiCasesTested++;
					TempCase = &m_SimplifiedMap[m_ExpandedNodes[j].x][m_ExpandedNodes[j].y];

					TempCase->bTested = true;
					TempCase->uiStep = m_uiStep;
					m_ZPathList.push(m_ExpandedNodes[j]);

					//Si on trouve le point de départ
					if( m_ExpandedNodes[j] == m_Begin )
					{
						m_bStartFound = true;
						j = eNEXT_CASES_NUMBER_4;
						i = uiListCurrentSize;
					}
				}
				//Enléve le node testé
				m_ZPathList.pop();
			}
		}

		//Libére les nodes restants
		while( m_ZPathList.size() )
			m_ZPathList.pop();

		//Calcule le chemin
		if( m_bStartFound )
		{
			TempVector = &m_Begin;
			bool bEndFound(false);
			size_t VectorSize(0);

			m_Path->push_back(*TempVector);

			while( !bEndFound )
			{
				iExpandedNodesNumber = computeNextCases8(*TempVector, m_ExpandedNodes);

				for( int i(0); i < iExpandedNodesNumber; i++ )
				{
					//Si la case a été calculée
					if( m_SimplifiedMap[m_ExpandedNodes[i].x][m_ExpandedNodes[i].y].bTested )
					{
						VectorSize = m_Path->size() - 1;

						//Si on ne traverse pas un mur en diagonale
						if( !checkDiagonalWall((*m_Path)[VectorSize], m_ExpandedNodes[i]) )
						{
							if( m_SimplifiedMap[m_ExpandedNodes[i].x][m_ExpandedNodes[i].y].uiStep < m_SimplifiedMap[TempVector->x][TempVector->y].uiStep )
								*TempVector = m_ExpandedNodes[i];
						}
					}
				}

				m_Path->push_back(*TempVector);

				//Si on a atteint la fin
				if( *TempVector == m_End )
					bEndFound = true;
			}
		}
	}


	void Pathfinding::sortDijkstra()
	{
		/*if( m_DijkstraList.size() > 1 )
		{
			bool bChange(true);
			sf::Vector2i Temp;

			while( bChange )
			{
				bChange = false;

				for( std::list<sf::Vector2i>::iterator it = m_DijkstraList.begin(), itTwo = ++m_DijkstraList.begin(); it != m_DijkstraList.end(), itTwo != m_DijkstraList.end(); ++it, ++itTwo )
				{
					if( m_CaseArray[(*it).x][(*it).y].fDistanceFromBegin > m_CaseArray[(*itTwo).x][(*itTwo).y].fDistanceFromBegin )
					{
						Temp = (*it);
						(*it) = (*itTwo);
						(*itTwo) = Temp;
						bChange = true;
					}
				}
			}
		}*/
	}

	void Pathfinding::dijkstra()
	{
		/*std::cout << "Start Dijkstra algo." << std::endl;

		bool bStartFound(false);
		unsigned int uiStep(0U);
		unsigned int uiListCurrentSize(0U);
		sf::Vector2i NextCases[eNEXT_CASES_NUMBER_8];
		stCASE* TempCase = NULL;
		sf::Vector2i* TempVector;
		m_CaseArray[m_Begin.x][m_Begin.y].bTested = true;
		float fDistance(0.0f);

		//On démarre du début
		m_DijkstraList.push_back(m_Begin);

		if( !(m_Begin == m_End) )
		{
			while( !bStartFound && (uiListCurrentSize = m_DijkstraList.size()) )
			{
				uiStep++;

				TempVector = &m_DijkstraList.front();
				fDistance = m_CaseArray[(*TempVector).x][(*TempVector).y].fDistanceFromBegin;

				//Cases adjacentes
				computeNextCases8(*TempVector, NextCases);

				for( int j(0); j < eNEXT_CASES_NUMBER_8; j++ )
				{
					//Si on est pas en dehors du tableau et qu'on ne traverse pas un mur
					if( isInCases(NextCases[j]) && !checkDiagonalWall(*TempVector, NextCases[j]) )
					{
						TempCase = &m_CaseArray[NextCases[j].x][NextCases[j].y];

						//Si on a pas encore été testé, et qu'on est pas un mur
						if( !(TempCase->bIswall) && !(TempCase->bTested) )
						{
							TempCase->bTested = true;
							TempCase->iStep = uiStep;
							TempCase->FillColor = FILL_COLOR;

							TempCase->Parent = *TempVector;
							TempCase->fDistanceFromBegin = fDistance + 1.0f;

							m_DijkstraList.push_back(NextCases[j]);
						}

						//Si on trouve le point de départ
						if( NextCases[j] == m_End )
						{
							bStartFound = true;
							j = eNEXT_CASES_NUMBER_8;
						}
					}
				}

				m_DijkstraList.pop_front();

				sortDijkstra();
			}
		}

		//Libére la liste de nodes
		clearListDijkstra();

		//Stocke le chemin calculé
		if( bStartFound )
			dijkstraComputeFoundPath(uiStep);

		std::cout << "Dijkstra algo achieved ( " + std::to_string(uiStep) + " steps) in ";*/

		/*sf::Vector2i TempCaseIndexs = m_End;
		bool bEndFound(false);

		while( m_CaseArray[TempCaseIndexs.x][TempCaseIndexs.y].Parent != sf::Vector2i(-1, -1) )
		{
			m_Path.push_back(TempCaseIndexs);
			TempCaseIndexs = m_CaseArray[TempCaseIndexs.x][TempCaseIndexs.y].Parent;
		}

		m_Path.push_back(m_Begin);*/
	}


	void Pathfinding::astar_compute_next_cases(sf::Vector2i _current, sf::Vector2i _cases[8], int* _validCases)
	{
		int c[] = { 0, -1, 1, 0, 0, 1, -1, 0, 1, -1, 1, 1, -1, 1, -1, -1 };

		for( int i = 0; i < 8; i++ )
		{
			sf::Vector2i expanded_node(_current.x + c[i * 2], _current.y + c[i * 2 + 1]);

			if( isInCases(expanded_node) && !isWall(expanded_node) )
			{
				if( i < 4 || !checkDiagonalWall(_current, expanded_node) )
				{
					_cases[(*_validCases)++] = expanded_node;
				}
			}
		}
	}

	int Pathfinding::astar_search_in_list(const sf::Vector2i& _node, std::vector<stPATHFINDING_NODE*>& _list)
	{
		for( unsigned int i(0); i < _list.size(); i++ )
			if( _node == _list[i]->GridCoords )
				return (int)i;

		return -1;
	}

	void Pathfinding::astar_remove_from_list(stPATHFINDING_NODE* _node, std::vector<stPATHFINDING_NODE*>& _list, bool _delete)
	{
		for( unsigned int i(0); i < _list.size(); i++ )
		{
			if( _node == _list[i] )
			{
				_list[i] = _list[_list.size() - 1];
				_list.pop_back();

				if( _delete )
					delete _node;
			}
		}
	}

	Pathfinding::stPATHFINDING_NODE* Pathfinding::astar_find_smallest(std::vector<stPATHFINDING_NODE*>& _list)
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

	float Pathfinding::astar_heuristic(const sf::Vector2i& _node)
	{
		float dx = (float)(m_Begin.x - _node.x);
		float dy = (float)(m_Begin.y - _node.y);

		return dx * dx + dy * dy;
	}

	void Pathfinding::aStar()
	{
		float cost = 1.0f;

		bool found = false;
		std::vector<stPATHFINDING_NODE*> open_list;
		std::vector<stPATHFINDING_NODE*> closed_list;

		int valid = 0;
		sf::Vector2i expanded_nodes[8];

		stPATHFINDING_NODE* smallest = 0;
		stPATHFINDING_SIMPLIFIED_NODE* currCase = 0;
		stPATHFINDING_NODE* newNode = 0;

		// Algorithm
		float f = astar_heuristic(m_End);
		open_list.push_back(new stPATHFINDING_NODE(m_End, NULL, sf::Vector2i(0, 0), f, f));

		while( open_list.size() > 0 )
		{
			m_uiStep++;

			smallest = astar_find_smallest(open_list);

			if( smallest->GridCoords == m_Begin )
			{
				found = true;
				break;
			}

			valid = 0;
			astar_compute_next_cases(smallest->GridCoords, expanded_nodes, &valid);

			for( int i = 0; i < valid; i++ )
			{
				currCase = &m_SimplifiedMap[expanded_nodes[i].x][expanded_nodes[i].y];

				if( !currCase->bTested )
				{
					m_uiCasesTested++;

					if( astar_search_in_list(expanded_nodes[i], closed_list) < 0 && astar_search_in_list(expanded_nodes[i], open_list) < 0 )
					{
						//Coût diagonales
						i > 4 ? cost = 1.4f : cost = 1.0f;

						newNode = new stPATHFINDING_NODE(expanded_nodes[i], smallest);
						newNode->fCostSoFar = newNode->ParentPtr->fCostSoFar + cost;
						newNode->fHeuristic = astar_heuristic(newNode->GridCoords);
						newNode->fEstimatedTotalCost = newNode->fCostSoFar + newNode->fHeuristic;

						currCase->bTested = true;
						currCase->uiStep = (int)newNode->fEstimatedTotalCost;

						open_list.push_back(newNode);
					}
				}
			}

			astar_remove_from_list(smallest, open_list, false);

			closed_list.push_back(smallest);
		}

		// Final path
		if( found )
		{
			stPATHFINDING_NODE* curr = astar_find_smallest(open_list);

			while( curr->ParentPtr )
			{
				m_Path->push_back(curr->GridCoords);
				curr = curr->ParentPtr;
			}

			m_Path->push_back(curr->GridCoords);
		}
	}


	void Pathfinding::jps_forced_neighbours(sf::Vector2i& _current, sf::Vector2i& _dir, sf::Vector2i _forced[8], int* _forcedCount)
	{
		// TODO récupérer tous les voisins forcés
	}

	sf::Vector2i* Pathfinding::jps_jump(sf::Vector2i& _current, sf::Vector2i& _start, sf::Vector2i& _end, sf::Vector2i& _dir)
	{
		sf::Vector2i next(_current + _dir);

		// On vérifie qu'on est pas tombés sur l'arrivée
		if (next == _end) return new sf::Vector2i(_end);

		// Vérification voisin forcé
		int forced_count = 0;
		sf::Vector2i forced[8];
		jps_forced_neighbours(_current, _dir, forced, &forced_count);

		// Si il y a un voisin forcé, on ajoute le point de saut
		if (forced_count)
			return new sf::Vector2i(next);

		// Cas en diagonal
		if (_dir.x && _dir.y)
		{
			if(jps_jump(next, _start, _end, sf::Vector2i(_dir.x, 0))
			|| jps_jump(next, _start, _end, sf::Vector2i(0, _dir.y)))
			{
				return new sf::Vector2i(next);
			}
		}

		return jps_jump(next, _start, _end, _dir);
	}

	void Pathfinding::jps_identify_successors(sf::Vector2i& _current, sf::Vector2i& _start, sf::Vector2i& _end, sf::Vector2i* _successors, int* _validSuccessors)
	{
		int valid_expanded_nodes = 0;
		sf::Vector2i expanded_nodes[8];

		// On récupère tous les voisins valables
		astar_compute_next_cases(_current, expanded_nodes, &valid_expanded_nodes);

		sf::Vector2i direction;
		
		for (int i = 0; i < valid_expanded_nodes; i++)
		{
			sf::Vector2i& expanded_node = expanded_nodes[i];

			direction = expanded_node - _current;

			// On tente de trouver un nouveau point de saut
			sf::Vector2i* jumpPoint = jps_jump(_current, _start, _end, direction);

			if (jumpPoint)
				_successors[(*_validSuccessors)++] = *jumpPoint;
		}
	}

	void Pathfinding::jps()
	{
		// TODO
		// http://users.cecs.anu.edu.au/~dharabor/data/papers/harabor-grastien-aaai11.pdf
	}


	void Pathfinding::switchElementFromList(stPATHFINDING_NODE* _Element)
	{
		m_JpsCloseList.push_back(_Element);
		m_JpsOpenList.remove(_Element);
	}

	Pathfinding::stPATHFINDING_NODE* Pathfinding::getSmallest()
	{
		stPATHFINDING_NODE* SmallestNode = m_JpsOpenList.front();

		for( stPATHFINDING_NODE*& Node : m_JpsOpenList )
		{
			if( Node->fEstimatedTotalCost < SmallestNode->fEstimatedTotalCost )
				SmallestNode = Node;
		}
		return SmallestNode;
	}

	Pathfinding::stPATHFINDING_NODE* Pathfinding::checkInLists(const sf::Vector2i& _Coords)
	{
		std::list<stPATHFINDING_NODE*>::iterator OpenListIt = m_JpsOpenList.begin();
		std::list<stPATHFINDING_NODE*>::iterator CloseListIt = m_JpsCloseList.begin();
		size_t iSizeOpen = m_JpsOpenList.size();
		size_t iSizeClose = m_JpsCloseList.size();
		size_t i(0);

		while( OpenListIt != m_JpsOpenList.end() || CloseListIt != m_JpsCloseList.end() )
		{
			if( i < m_JpsOpenList.size() )
			{
				if( (*OpenListIt)->GridCoords == _Coords )
					return (*OpenListIt);

				++OpenListIt;
			}

			if( i < m_JpsCloseList.size() )
			{
				if( (*CloseListIt)->GridCoords == _Coords )
					return (*CloseListIt);

				++CloseListIt;
			}

			i++;
		}

		return NULL;
	}

	void Pathfinding::aStarKcc()
	{
		stPATHFINDING_NODE* TempNode = NULL;
		sf::Vector2i* TempVector = NULL;
		stPATHFINDING_SIMPLIFIED_NODE* TempCase = NULL;
		int iExpandedNodeNumber(0);

		//On démarre du début
		m_SimplifiedMap[m_Begin.x][m_Begin.y].bTested = true;
		m_JpsOpenList.emplace_back(new stPATHFINDING_NODE(m_Begin, NULL, sf::Vector2i(0, 0), 0.0f, 0.0f, (float)ABS(m_End.x - m_Begin.x) + (float)ABS(m_End.y - m_Begin.y)));
		TempNode = m_JpsOpenList.front();

		while( !m_bStartFound && m_JpsOpenList.size() )
		{
			m_uiStep++;
			switchElementFromList(TempNode);
			TempVector = &m_JpsCloseList.back()->GridCoords;

			//Cases adjacentes
			iExpandedNodeNumber = computeNextCases8Astar(*TempVector, m_ExpandedNodes);

			for( int j(0); j < iExpandedNodeNumber; j++ )
			{
				m_uiCasesTested++;

				TempCase = &m_SimplifiedMap[m_ExpandedNodes[j].x][m_ExpandedNodes[j].y];

				TempCase->bTested = true;
				TempCase->uiStep = m_uiStep;

				//Si on est pas déjà dans une des deux listes dans la close list
				if( !checkInLists(m_ExpandedNodes[j]) )
				{
					float fCostSofar = j < eNEXT_CASES_NUMBER_4 ? 1.0f : 1.4f;

					m_JpsOpenList.emplace_back(new stPATHFINDING_NODE(m_ExpandedNodes[j], m_JpsCloseList.back(), sf::Vector2i(0, 0), m_JpsCloseList.back()->fCostSoFar + fCostSofar, 0.0f, (float)ABS(m_End.x - m_ExpandedNodes[j].x) + (float)ABS(m_End.y - m_ExpandedNodes[j].y)));

					//Si on trouve l'arrivée
					if( m_ExpandedNodes[j] == m_End )
					{
						m_bStartFound = true;
						j = eNEXT_CASES_NUMBER_8;
					}
				}
			}
			//Recherche l'élément avec la plus faible heurisitique
			TempNode = getSmallest();
		}

		// Final path
		if( m_bStartFound )
		{
			stPATHFINDING_NODE* End = m_JpsOpenList.back();

			while( End->ParentPtr )
			{
				m_Path->push_back(End->GridCoords);
				End = End->ParentPtr;
			}

			m_Path->push_back(End->GridCoords);
		}

		//Clear conteneurs
		for( auto it = m_JpsOpenList.begin(); it != m_JpsOpenList.end(); ++it )
			delete (*it);
		m_JpsOpenList.clear();
		for( auto it = m_JpsCloseList.begin(); it != m_JpsCloseList.end(); ++it )
			delete (*it);
		m_JpsCloseList.clear();
	}


	void Pathfinding::jpsKcc()
	{
		stPATHFINDING_NODE* TempNode = NULL;
		sf::Vector2i* TempVector = NULL;
		stPATHFINDING_SIMPLIFIED_NODE* TempCase = NULL;
		int iExpandedNodeNumber(0);

		//On démarre du début
		m_SimplifiedMap[m_Begin.x][m_Begin.y].bTested = true;
		m_JpsOpenList.emplace_back(new stPATHFINDING_NODE(m_Begin, NULL, sf::Vector2i(0, 0), 0.0f, 0.0f, (float)ABS(m_End.x - m_Begin.x) + (float)ABS(m_End.y - m_Begin.y)));
		TempNode = m_JpsOpenList.front();

		while( !m_bStartFound && m_JpsOpenList.size() )
		{
			m_uiStep++;
			switchElementFromList(TempNode);
			TempVector = &m_JpsCloseList.back()->GridCoords;

			//Cases adjacentes
			iExpandedNodeNumber = computeNextCases8Astar(*TempVector, m_ExpandedNodes);

			for( int j(0); j < iExpandedNodeNumber; j++ )
			{
				m_uiCasesTested++;

				TempCase = &m_SimplifiedMap[m_ExpandedNodes[j].x][m_ExpandedNodes[j].y];

				TempCase->bTested = true;
				TempCase->uiStep = m_uiStep;

				//Si on est pas déjà dans une des deux listes dans la close list
				if( !checkInLists(m_ExpandedNodes[j]) )
				{
					m_JpsOpenList.emplace_back(new stPATHFINDING_NODE(m_ExpandedNodes[j], m_JpsCloseList.back(), sf::Vector2i(0, 0), 0.0f, 0.0f, (float)ABS(m_End.x - m_ExpandedNodes[j].x) + (float)ABS(m_End.y - m_ExpandedNodes[j].y)));

					//Si on trouve l'arrivée
					if( m_ExpandedNodes[j] == m_End )
					{
						m_bStartFound = true;
						j = eNEXT_CASES_NUMBER_8;
					}
				}
			}
			//Recherche l'élément avec la plus faible heurisitique
			TempNode = getSmallest();
		}

		// Final path
		if( m_bStartFound )
		{
			stPATHFINDING_NODE* End = m_JpsOpenList.back();

			while( End->ParentPtr )
			{
				m_Path->push_back(End->GridCoords);
				End = End->ParentPtr;
			}

			m_Path->push_back(End->GridCoords);
		}

		//Clear conteneurs
		for( auto it = m_JpsOpenList.begin(); it != m_JpsOpenList.end(); ++it )
			delete (*it);
		m_JpsOpenList.clear();
		for( auto it = m_JpsCloseList.begin(); it != m_JpsCloseList.end(); ++it )
			delete (*it);
		m_JpsCloseList.clear();
	}
}