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
	
	bool Pathfinding::isWall(const sf::Vector2i& _Position)
	{
		return m_CaseArray[_Position.x][_Position.y].bIswall;
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
		std::cout << m_sAlgosNames[_Algo] << " computed in " << m_ElapsedTime << " microseconds (" << m_ElapsedTime * 0.001f << " ms), with a total of " << m_uiStep << " steps." << std::endl;
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

	void Pathfinding::astar_compute_next_cases(sf::Vector2i _current, sf::Vector2i _cases[8], int* _validCases)
	{
		int c[] = { 0, -1, 1, 0, 0, 1, -1, 0, 1, -1, 1, 1, -1, 1, -1, -1 };

		for (int i = 0; i < 8; i++)
		{
			sf::Vector2i expanded_node(_current.x + c[i * 2], _current.y + c[i * 2 + 1]);

			if (isInCases(expanded_node) && !isWall(expanded_node))
			{
				if (i < 4 || !checkDiagonalWall(_current, expanded_node))
				{
					_cases[(*_validCases)++] = expanded_node;
				}
			}
		}
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
		stPATHFINDING_CASE* currCase = 0;
		stPATHFINDING_NODE* newNode = 0;

		// Algorithm
		open_list.push_back(new stPATHFINDING_NODE(m_End, 0, 0, astar_heuristic(m_End), astar_heuristic(m_End)));

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
				currCase = &m_CaseArray[expanded_nodes[i].x][expanded_nodes[i].y];

				if (!currCase->bTested)
				{
					if (astar_search_in_list(expanded_nodes[i], closed_list) < 0 && astar_search_in_list(expanded_nodes[i], open_list) < 0)
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
}