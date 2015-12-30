namespace sfgmk
{
	Pathfinding::Pathfinding() : m_Map(NULL), m_NodeMap(NULL), m_Begin(sf::Vector2i(0, 0)), m_End(sf::Vector2i(0, 0)), m_Size(sf::Vector2i(0, 0)), m_Path(NULL), m_bGoalFound(false), m_uiStep(0U), m_ElapsedTime(0)
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

		//Algorithms foncters
		m_Algorithms.m_FunctionsArray.pushBack(new sfgmk::FoncterTemplateInstance<Pathfinding, void>(this, &Pathfinding::zPath));
		m_Algorithms.m_FunctionsArray.pushBack(new sfgmk::FoncterTemplateInstance<Pathfinding, void>(this, &Pathfinding::dijkstra));
		m_Algorithms.m_FunctionsArray.pushBack(new sfgmk::FoncterTemplateInstance<Pathfinding, void>(this, &Pathfinding::aStar));
		m_Algorithms.m_FunctionsArray.pushBack(new sfgmk::FoncterTemplateInstance<Pathfinding, void>(this, &Pathfinding::jps));

		//Strings algos names
		m_sAlgosNames[eZpath] = "Z-Path";
		m_sAlgosNames[eDijkstra] = "Dijkstra";
		m_sAlgosNames[eAStar] = "A*";
		m_sAlgosNames[eJps] = "Jps";
	}

	Pathfinding::~Pathfinding()
	{
		m_Algorithms.m_FunctionsArray.deleteAndClear();
		releaseNodeMap();
	}


	void Pathfinding::computePathfinding(PathfindingPathCntr* _Path, const ePATHFINDING_ALGOS& _Algo, PathfindingMap* _Map, const sf::Vector2i& _Begin, const sf::Vector2i& _End, const bool& _SmoothPath, const float& _CaseSize)
	{
		//Reset variables
		m_bGoalFound = false;
		m_uiStep = 0U;

		//Init
		releaseNodeMap();
		m_Map = _Map;
		m_Size = m_Map->getSize();
		allocNodeMap();
		//TODO resetNodeMap();

		m_Begin = _Begin;
		m_End = _End;
		m_Path = _Path;

		//Lance le pathfinding
		m_Clock.restart();
		m_Algorithms[_Algo];
		m_ElapsedTime = m_Clock.getElapsedTime().asMicroseconds();

		//Print console (temps et résultat)
		std::cout << m_sAlgosNames[_Algo] << " computed in " << m_ElapsedTime << " microseconds (" << m_ElapsedTime * 0.001f << " ms), with a total of " << m_uiStep << " steps." << std::endl;
		m_bGoalFound ? std::cout << "Path length: " << _Path->size() << std::endl << std::endl : std::cout << "Goal not found." << std::endl;

		if( m_bGoalFound && _SmoothPath )
		{
			m_Clock.restart();
			smoothPath(_Path, _CaseSize);
			m_ElapsedTime = m_Clock.getElapsedTime().asMicroseconds();
			std::cout << "Smoothed in " << m_ElapsedTime << "ms. Final path: " << _Path->size() << "." << std::endl;
		}
		
		std::cout << std::endl;
	}

	void Pathfinding::smoothPath(PathfindingPathCntr* _Path, const float& _CaseSize)
	{
		for( size_t i(0); i < _Path->size() - 1; i++ )
		{
			size_t j = i + 1;
			sf::Vector2i Difference = (*_Path)[j] - (*_Path)[i];
			j++;

			while( j < _Path->size() && (*_Path)[j] == ((*_Path)[j - 1] + Difference) )
				j++;
			
			if( j > 2 )
				_Path->erase(_Path->begin() + (i + 1), _Path->begin() + (j - 1));
		}
	}


	void Pathfinding::zPath()
	{
		unsigned int uiListCurrentSize(0U);
		int iExpandedNodesNumber(0);
		stPATHFINDING_GRID_NODE* TempCase = NULL;
		sf::Vector2i* TempVector = NULL;

		//On démarre de la fin
		m_NodeMap[m_Map->getIndex(m_End)].bTested = true;
		m_ZPathList.push(m_End);

		//Recherche jusqu'à trouver le point d'arrivée, ou qu'on ai testé tous les points
		while( !m_bGoalFound && (uiListCurrentSize = m_ZPathList.size()) )
		{
			m_uiStep++;

			//Parcours des éléments de la liste pour l'étape actuelle
			for( unsigned int i(0U); i < uiListCurrentSize; i++ )
			{
				TempVector = &m_ZPathList.front();

				//Cases adjacentes (haut, droite, bas, gauche)
				iExpandedNodesNumber = computeExpandedCases4(*TempVector);

				//Parcours les cases adjacentes valides
				for( int j(0); j < iExpandedNodesNumber; j++ )
				{
					TempCase = &m_NodeMap[m_ExpandedNodes[j].iIndex];

					//Si la case n'a pas encore été testée
					if( !TempCase->bTested )
					{
						TempCase->bTested = true;
						TempCase->uiStep = m_uiStep;
						m_ZPathList.push(m_ExpandedNodes[j].Coords);

						//Si on trouve le but
						if( m_ExpandedNodes[j].Coords == m_Begin )
						{
							m_bGoalFound = true;
							j = eEXPANDED_CASES_NUMBER_4;
							i = uiListCurrentSize;
						}
					}
				}
				//Enlève le node testé
				m_ZPathList.pop();
			}
		}

		//Libére les nodes restants
		while( m_ZPathList.size() )
			m_ZPathList.pop();

		//Calcule le chemin si on en a trouvé un
		if( m_bGoalFound )
		{
			TempVector = &m_Begin;
			bool bEndFound(false);
			size_t VectorSize(0);

			m_Path->push_back(*TempVector);

			while( !bEndFound )
			{
				iExpandedNodesNumber = computeExpandedCases8(*TempVector);

				for( int i(0); i < iExpandedNodesNumber; i++ )
				{
					TempCase = &m_NodeMap[m_ExpandedNodes[i].iIndex];

					//Si la case a été calculée
					if( TempCase->bTested )
					{
						VectorSize = m_Path->size() - 1;

						//Si c'est la suite du chemin (poids case)
						if( TempCase->uiStep < m_NodeMap[m_Map->getIndex(*TempVector)].uiStep )
							*TempVector = m_ExpandedNodes[i].Coords;
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
		stPATHFINDING_NODE *NodeToExpand(NULL), *Temp(NULL);
		int iExpandedNodesNumber(0);
		stPATHFINDING_GRID_NODE* ExpandedCase(NULL);
		float fCost(0.0f);

		//On fixe le sommet initial
		m_OpenList.push_back(new stPATHFINDING_NODE(m_Begin));

		//Algo
		while( m_OpenList.size() > 0 && !m_bGoalFound )
		{
			m_uiStep++;
			NodeToExpand = findSmallestNode(m_OpenList);
			iExpandedNodesNumber = computeExpandedCases8(NodeToExpand->GridCoords);

			for( int i = 0; i < iExpandedNodesNumber; i++ )
			{
				ExpandedCase = &m_NodeMap[m_ExpandedNodes[i].iIndex];

				//Si le noeud a été "verrouillé", il n'est plus utilisable
				if( ExpandedCase->List != eLIST_POSITION::eCloseList )
				{
					//Coût déplacement
					i > 4 ? fCost = SQUARE((float)m_Map->getTerrainType(m_ExpandedNodes[i].Coords) * 1.4f) : fCost = SQUARE((float)m_Map->getTerrainType(m_ExpandedNodes[i].Coords));
					fCost += NodeToExpand->fEstimatedTotalCost;

					//Le noeud n'avait pas encore été calculé
					if( ExpandedCase->List == eLIST_POSITION::eNone )
					{
						stPATHFINDING_NODE* NewNode = new stPATHFINDING_NODE(m_ExpandedNodes[i].Coords, NodeToExpand, sf::Vector2i(0, 0), 0.0f, 0.0f, fCost);
						m_OpenList.push_back(NewNode);

						ExpandedCase->bTested = true;
						ExpandedCase->uiStep = m_uiStep;
						ExpandedCase->List = eOpenList;

						//Si on trouve le but
						if( m_ExpandedNodes[i].Coords == m_End )
						{
							m_bGoalFound = true;
							i = iExpandedNodesNumber;
						}
					}
					//Le noeud avait déjà été atteint
					/*else
					{
						Temp = searchInList(m_ExpandedNodes[i].Coords, m_OpenList);

						//Si le chemin actuel est plus court que celui calculé auparavant
						if( fCost < Temp->fEstimatedTotalCost )
						{
							Temp->ParentPtr = NodeToExpand;
							Temp->fEstimatedTotalCost = fCost;

							ExpandedCase->uiStep = m_uiStep;
						}
					}*/
				}
			}

			//On transfére le noeud choisi de l'open à la close list
			m_CloseList.push_back(NodeToExpand);
			m_NodeMap[m_Map->getIndex(NodeToExpand->GridCoords)].List = eCloseList;
			m_OpenList.remove(NodeToExpand);
		}

		//Chemin
		if( m_bGoalFound )
		{
			Temp = m_OpenList.back();

			while( Temp->ParentPtr )
			{
				m_Path->push_back(Temp->GridCoords);
				Temp = Temp->ParentPtr;
			}

			m_Path->push_back(Temp->GridCoords);
		}

		//Libére les nodes restants
		m_OpenList.clear();
		m_CloseList.clear();
	}

	void Pathfinding::aStar()
	{
		float cost = 1.0f;
		int iExpandedNodesNumber(0);

		stPATHFINDING_NODE* smallest = 0;
		stPATHFINDING_GRID_NODE* currCase = 0;
		stPATHFINDING_NODE* newNode = 0;
		stPATHFINDING_NODE* Temp = NULL;

		// Algorithm
		float f = (float)math::Calc_DistanceSquared(m_Begin.x, m_Begin.y, m_End.x, m_End.y);
		m_OpenList.push_back(new stPATHFINDING_NODE(m_Begin, NULL, sf::Vector2i(0, 0), f, f));

		while( m_OpenList.size() > 0 )
		{
			m_uiStep++;

			smallest = findSmallestNode(m_OpenList);

			if( smallest->GridCoords == m_End )
			{
				m_bGoalFound = true;
				break;
			}

			iExpandedNodesNumber = computeExpandedCases8(smallest->GridCoords);

			for( int i = 0; i < iExpandedNodesNumber; i++ )
			{
				currCase = &m_NodeMap[m_ExpandedNodes[i].iIndex];

				//Coût diagonales
				i > 4 ? cost = SQUARE((float)m_Map->getTerrainType(m_ExpandedNodes[i].Coords) * 1.4f) : cost = SQUARE((float)m_Map->getTerrainType(m_ExpandedNodes[i].Coords));

				if( m_NodeMap[m_ExpandedNodes[i].iIndex].List == eNone )
				{
					newNode = new stPATHFINDING_NODE(m_ExpandedNodes[i].Coords, smallest);
					newNode->fCostSoFar = newNode->ParentPtr->fCostSoFar + cost;
					newNode->fHeuristic = (float)math::Calc_DistanceSquared(newNode->GridCoords.x, newNode->GridCoords.y, m_End.x, m_End.y);
					newNode->fEstimatedTotalCost = newNode->fCostSoFar + newNode->fHeuristic;

					currCase->bTested = true;
					currCase->uiStep = m_uiStep;
					currCase->List = eOpenList;

					m_OpenList.push_back(newNode);
				}
				/*else if( m_NodeMap[m_ExpandedNodes[i].iIndex].List == eOpenList )
				{
					Temp = searchInList(m_ExpandedNodes[i].Coords, m_OpenList);

					if( Temp->fCostSoFar > smallest->fCostSoFar + cost )
					{
						Temp->ParentPtr = smallest;
						Temp->fCostSoFar = Temp->ParentPtr->fCostSoFar + cost;
						Temp->fEstimatedTotalCost = Temp->fCostSoFar + Temp->fHeuristic;
					}
				}
				else
				{
					Temp = searchInList(m_ExpandedNodes[i].Coords, m_CloseList);

					if( Temp->fCostSoFar > smallest->fCostSoFar + cost )
					{
						Temp->ParentPtr = smallest;
						Temp->fCostSoFar = Temp->ParentPtr->fCostSoFar + cost;
						Temp->fEstimatedTotalCost = Temp->fCostSoFar + Temp->fHeuristic;

						m_OpenList.push_back(Temp);
						m_NodeMap[m_Map->getIndex(Temp->GridCoords)].List = eOpenList;
						m_CloseList.remove(smallest);
					}
				}*/
			}

			m_CloseList.push_back(smallest);
			m_NodeMap[m_Map->getIndex(smallest->GridCoords)].List = eCloseList;
			m_OpenList.remove(smallest);
		}

		// Final path
		if( m_bGoalFound )
		{
			stPATHFINDING_NODE* curr = findSmallestNode(m_OpenList);

			while( curr->ParentPtr )
			{
				m_Path->push_back(curr->GridCoords);
				curr = curr->ParentPtr;
			}

			m_Path->push_back(curr->GridCoords);
		}

		//Libére les nodes restants
		m_OpenList.clear();
		m_CloseList.clear();
	}

	void Pathfinding::jps()
	{
		// http://users.cecs.anu.edu.au/~dharabor/data/papers/harabor-grastien-aaai11.pdf
		//TODO: passe les murs en diagonale parfois
		//		probleme lorsque la sortie est inaccessible sur une GRANDE map

		float cost = 1.0f;
		int iExpandedNodesNumber(0);

		stPATHFINDING_NODE* smallest = 0;
		stPATHFINDING_GRID_NODE* currCase = 0;
		stPATHFINDING_NODE* newNode = 0;
		stPATHFINDING_NODE* Temp = NULL;

		// Algorithm
		float f = (float)math::Calc_DistanceSquared(m_Begin.x, m_Begin.y, m_End.x, m_End.y);
		m_OpenList.push_back(new stPATHFINDING_NODE(m_Begin, NULL, sf::Vector2i(0, 0), f, f));

		while( m_OpenList.size() > 0 )
		{
			m_uiStep++;

			smallest = findSmallestNode(m_OpenList);

			if( smallest->GridCoords == m_End )
			{
				m_bGoalFound = true;
				break;
			}
		
			iExpandedNodesNumber = 0;
			identifySuccessors(smallest->GridCoords, m_Begin, m_End, &iExpandedNodesNumber);

			for( int i = 0; i < iExpandedNodesNumber; i++ )
			{
				currCase = &m_NodeMap[m_ExpandedNodes[i].iIndex];

				//Coût diagonales
				i > 4 ? cost = SQUARE((float)m_Map->getTerrainType(m_ExpandedNodes[i].Coords) * 1.4f) : cost = SQUARE((float)m_Map->getTerrainType(m_ExpandedNodes[i].Coords));

				if( m_NodeMap[m_ExpandedNodes[i].iIndex].List == eNone )
				{
					newNode = new stPATHFINDING_NODE(m_ExpandedNodes[i].Coords, smallest);
					newNode->fCostSoFar = newNode->ParentPtr->fCostSoFar + cost;
					newNode->fHeuristic = (float)math::Calc_DistanceSquared(newNode->GridCoords.x, newNode->GridCoords.y, m_End.x, m_End.y);
					newNode->fEstimatedTotalCost = newNode->fCostSoFar + newNode->fHeuristic;

					currCase->bTested = true;
					currCase->uiStep = m_uiStep;
					currCase->List = eOpenList;
					
					m_OpenList.push_back(newNode);
				}
				else if( m_NodeMap[m_ExpandedNodes[i].iIndex].List == eOpenList )
				{
					Temp = searchInList(m_ExpandedNodes[i].Coords, m_OpenList);

					if( Temp->fCostSoFar > smallest->fCostSoFar + cost )
					{
						Temp->ParentPtr = smallest;
						Temp->fCostSoFar = Temp->ParentPtr->fCostSoFar + cost;
						Temp->fEstimatedTotalCost = Temp->fCostSoFar + Temp->fHeuristic;
					}
				}
				else
				{
					Temp = searchInList(m_ExpandedNodes[i].Coords, m_CloseList);

					if( Temp->fCostSoFar > smallest->fCostSoFar + cost )
					{
						Temp->ParentPtr = smallest;
						Temp->fCostSoFar = Temp->ParentPtr->fCostSoFar + cost;
						Temp->fEstimatedTotalCost = Temp->fCostSoFar + Temp->fHeuristic;

						m_OpenList.push_back(Temp);
						m_NodeMap[m_Map->getIndex(Temp->GridCoords)].List = eOpenList;
						m_CloseList.remove(smallest);
					}

				}
			}

			m_CloseList.push_back(smallest);
			m_NodeMap[m_Map->getIndex(smallest->GridCoords)].List = eCloseList;
			m_OpenList.remove(smallest);
		}

		// Final path
		if( m_bGoalFound )
		{
			stPATHFINDING_NODE* prev = 0;
			stPATHFINDING_NODE* curr = findSmallestNode(m_OpenList);

			while( curr->ParentPtr )
			{
				m_Path->push_back(curr->GridCoords);

				prev = curr;
				curr = curr->ParentPtr;
			}

			m_Path->push_back(curr->GridCoords);
		}

		//Libére les nodes restants
		m_OpenList.clear();
		m_CloseList.clear();
	}
}