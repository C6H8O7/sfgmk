namespace sfgmk
{
	Pathfinding::Pathfinding() : m_CaseArray(NULL), m_Begin(sf::Vector2i(0, 0)), m_End(sf::Vector2i(0, 0)), m_GridSize(0, 0), m_bStartFound(false), m_uiStep(0U)
	{
		m_sAlgosNames[eZpath] = "Z-Path";
		m_sAlgosNames[eDijkstra] = "Dijkstra";
		m_sAlgosNames[eAStar] = "A*";

		memset(m_NextCases, 0, sizeof(sf::Vector2i) * eNEXT_CASES_NUMBER_8);

		//Algorithm foncters
		sfgmk::FoncterTemplateInstance<Pathfinding, void>* zPathPtr = new sfgmk::FoncterTemplateInstance<Pathfinding, void>(this, &Pathfinding::zPath);
		m_Algorithms.m_FunctionsArray.pushBack(zPathPtr);

		sfgmk::FoncterTemplateInstance<Pathfinding, void>* dijkstraPtr = new sfgmk::FoncterTemplateInstance<Pathfinding, void>(this, &Pathfinding::dijkstra);
		m_Algorithms.m_FunctionsArray.pushBack(dijkstraPtr);

		sfgmk::FoncterTemplateInstance<Pathfinding, void>* astarPtr = new sfgmk::FoncterTemplateInstance<Pathfinding, void>(this, &Pathfinding::aStar);
		m_Algorithms.m_FunctionsArray.pushBack(astarPtr);
	}

	Pathfinding::~Pathfinding()
	{
		m_Algorithms.m_FunctionsArray.deleteAndClear();

		std::queue<sf::Vector2i> EmptyList;
		std::swap(m_ZPathList, EmptyList);
	}


	void Pathfinding::computeNextCases4(const sf::Vector2i& _CurrentCase, sf::Vector2i _Array[eNEXT_CASES_NUMBER_4])
	{
		_Array[eTop] = _CurrentCase + sf::Vector2i(0, -1);
		_Array[eRight] = _CurrentCase + sf::Vector2i(1, 0);
		_Array[eBot] = _CurrentCase + sf::Vector2i(0, 1);
		_Array[eLeft] = _CurrentCase + sf::Vector2i(-1, 0);
	}

	void Pathfinding::computeNextCases8(const sf::Vector2i& _CurrentCase, sf::Vector2i _Array[eNEXT_CASES_NUMBER_8])
	{
		_Array[eTop] = _CurrentCase + sf::Vector2i(0, -1);
		_Array[eRight] = _CurrentCase + sf::Vector2i(1, 0);
		_Array[eBot] = _CurrentCase + sf::Vector2i(0, 1);
		_Array[eLeft] = _CurrentCase + sf::Vector2i(-1, 0);
		
		_Array[eTopRight] = _CurrentCase + sf::Vector2i(1, -1);
		_Array[eBotRight] = _CurrentCase + sf::Vector2i(1, 1);
		_Array[eBotLeft] = _CurrentCase + sf::Vector2i(-1, 1);
		_Array[eTopLeft] = _CurrentCase + sf::Vector2i(-1, -1);
	}

	bool Pathfinding::isInCases(const sf::Vector2i& _Position)
	{
		return (_Position.x >= 0 && _Position.x < m_GridSize.x && _Position.y >= 0 && _Position.y < m_GridSize.y);
	}

	bool Pathfinding::checkDiagonalWall(const sf::Vector2i& _CaseOne, const sf::Vector2i& _CaseTwo)
	{
		m_CasesToTest[0] = sf::Vector2i(_CaseOne.x, _CaseTwo.y);
		m_CasesToTest[1] = sf::Vector2i(_CaseTwo.x, _CaseOne.y);

		if( m_CaseArray[m_CasesToTest[0].x][m_CasesToTest[0].y].bIswall && m_CaseArray[m_CasesToTest[1].x][m_CasesToTest[1].y].bIswall )
			return true;
		else
			return false;
	}


	void Pathfinding::computePathfinding(std::vector<sf::Vector2i>* _Path, const ePATHFINDING_ALGOS& _Algo, stPATHFINDING_CASE** _Grid, const sf::Vector2i& _GridSize, const sf::Vector2i& _Begin, const sf::Vector2i& _End)
	{
		//Vector de sortie
		m_Path = _Path;

		//Stocke les infos grille
		m_GridSize = _GridSize;
		m_CaseArray = _Grid;

		//Reset variables
		m_bStartFound = false;
		m_uiStep = 0U;

		//Points d'intérêt
		m_Begin = _Begin;
		m_End = _End;

		//Lance le pathfinding
		m_Clock.restart();
		m_Algorithms[_Algo];
		m_ElapsedTime = m_Clock.getElapsedTime().asMicroseconds();
		std::cout << m_sAlgosNames[_Algo] << " computed in " << m_ElapsedTime * 0.001f << " ms." << std::endl;
	}


	void Pathfinding::zPath()
	{
		unsigned int uiListCurrentSize(0U);
		stPATHFINDING_CASE* TempCase = NULL;
		sf::Vector2i* TempVector = NULL;

		//On démarre de la fin
		m_CaseArray[m_End.x][m_End.y].bTested = true;
		m_ZPathList.push(m_End);
	
		while( !m_bStartFound && (uiListCurrentSize = m_ZPathList.size()) )
		{
			m_uiStep++;

			for( unsigned int i(0U); i < uiListCurrentSize; i++ )
			{
				TempVector = &m_ZPathList.front();

				//Cases adjacentes
				computeNextCases4(*TempVector, m_NextCases);

				for( int j(0); j < eNEXT_CASES_NUMBER_4; j++ )
				{
					//Si on est pas en dehors du tableau
					if( isInCases(m_NextCases[j]) )
					{
						TempCase = &m_CaseArray[m_NextCases[j].x][m_NextCases[j].y];

						//Si on a pas encore été testé, et qu'on est pas un mur
						if( !(TempCase->bIswall) && !(TempCase->bTested) )
						{
							TempCase->bTested = true;
							TempCase->uiStep = m_uiStep;
							m_ZPathList.push(m_NextCases[j]);

							//Si on trouve le point de départ
							if( m_NextCases[j] == m_Begin )
							{
								m_bStartFound = true;
								j = eNEXT_CASES_NUMBER_4;
								i = uiListCurrentSize;
							}
						}
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
				computeNextCases8(*TempVector, m_NextCases);

				for( int j(0); j < eNEXT_CASES_NUMBER_8; j++ )
				{
					//Si on est pas en dehors du tableau
					if( isInCases(m_NextCases[j]) )
					{
						//Si la case a été calculée
						if( m_CaseArray[m_NextCases[j].x][m_NextCases[j].y].bTested )
						{
							VectorSize = m_Path->size() - 1;

							//Si on ne traverse pas un mur en diagonale
							if( !checkDiagonalWall((*m_Path)[VectorSize], m_NextCases[j]) )
							{
								if( m_CaseArray[m_NextCases[j].x][m_NextCases[j].y].uiStep < m_CaseArray[TempVector->x][TempVector->y].uiStep )
									*TempVector = m_NextCases[j];
							}
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

	void Pathfinding::dijkstra()
	{
	}

	void Pathfinding::aStar()
	{
	}




	/*void StateGamePathfinding::dijkstra()
	{
		std::cout << "Start Dijkstra algo." << std::endl;

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

		std::cout << "Dijkstra algo achieved ( " + std::to_string(uiStep) + " steps) in ";
	}

	void StateGamePathfinding::dijkstraComputeFoundPath(unsigned int _Step)
	{
		sf::Vector2i TempCaseIndexs = m_End;
		bool bEndFound(false);

		while( m_CaseArray[TempCaseIndexs.x][TempCaseIndexs.y].Parent != sf::Vector2i(-1, -1) )
		{
			m_Path.push_back(TempCaseIndexs);
			TempCaseIndexs = m_CaseArray[TempCaseIndexs.x][TempCaseIndexs.y].Parent;
		}

		m_Path.push_back(m_Begin);
	}

	void StateGamePathfinding::sortDijkstra()
	{
		if( m_DijkstraList.size() > 1 )
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
		}
	}


	int StateGamePathfinding::astar_search_in_list(sf::Vector2i _node, std::vector<NodeAStar*>& _list)
	{
		for( unsigned int i(0); i < _list.size(); i++ )
			if( _node == _list[i]->GridCoords )
				return (int)i;

		return -1;
	}

	void StateGamePathfinding::astar_remove_from_list(NodeAStar* _node, std::vector<NodeAStar*>& _list, bool _delete)
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

	StateGamePathfinding::NodeAStar* StateGamePathfinding::astar_find_smallest(std::vector<NodeAStar*>& _list)
	{
		float value = -1.0f;
		NodeAStar* node = 0;

		for( NodeAStar*& _node : _list )
		{
			if( _node->fEstimatedTotalCost < value || value < 0.0f )
			{
				node = _node;
				value = _node->fEstimatedTotalCost;
			}
		}

		return node;
	}

	float StateGamePathfinding::astar_heuristic(sf::Vector2i& _node)
	{
		return math::Calc_DistanceSquared(_node.x, _node.y, m_End.x, m_End.y);
	}

	void StateGamePathfinding::astar()
	{
		std::cout << "Start AStar algo." << std::endl;

		float cost = 1.0f;

		bool found = false;
		std::vector<NodeAStar*> open_list;
		std::vector<NodeAStar*> closed_list;

		// Algorithm

		open_list.push_back(new NodeAStar(m_Begin, 0, 0, astar_heuristic(m_Begin), astar_heuristic(m_Begin)));

		while( open_list.size() > 0 )
		{
			NodeAStar* smallest = astar_find_smallest(open_list);

			if( smallest->GridCoords == m_End )
			{
				found = true;
				break;
			}

			sf::Vector2i expanded_nodes[8];
			computeNextCases8(smallest->GridCoords, expanded_nodes);

			for( int i = 0; i < 8; i++ )
			{
				if( isInCases(expanded_nodes[i]) )
				{
					stCASE* node = &m_CaseArray[expanded_nodes[i].x][expanded_nodes[i].y];

					if( !node->bIswall && astar_search_in_list(expanded_nodes[i], closed_list) < 0 && astar_search_in_list(expanded_nodes[i], open_list) < 0 )
					{
						cost = 1.0f;

						if( i >= 4 )
							cost = 1.4f;

						NodeAStar* newNode = new NodeAStar(expanded_nodes[i], smallest);
						newNode->fCostSoFar = newNode->ParentPtr->fCostSoFar + cost;
						newNode->fHeuristic = astar_heuristic(newNode->GridCoords);
						newNode->fEstimatedTotalCost = newNode->fCostSoFar + newNode->fHeuristic;

						node->bTested = true;
						node->iStep = (int)newNode->fEstimatedTotalCost;

						open_list.push_back(newNode);
					}
				}
			}

			astar_remove_from_list(smallest, open_list, false);

			closed_list.push_back(smallest);
		}

		std::cout << (found ? "FOUND!" : "Not Found :( !") << std::endl;

		// Final path

		if( found )
		{
			NodeAStar* curr = astar_find_smallest(open_list);

			while( curr->ParentPtr )
			{
				m_Path.push_back(curr->GridCoords);
				curr = curr->ParentPtr;
			}

			m_Path.push_back(curr->GridCoords);
		}

		std::cout << "AStar algo achieved ( ";
	}*/
}