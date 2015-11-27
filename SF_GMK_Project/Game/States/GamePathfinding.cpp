#include "stdafx.h"
#include "../../IncludesProjet.hpp"

using namespace sfgmk;


#define ARRAY_SIZE_X 64
#define ARRAY_SIZE_Y 64
#define ARRAY_CASE_SIZE 16.0f
#define MAP_POS_X 350.0f
#define MAP_POS_Y 50.0f
#define FILL_COLOR sf::Color(0, 100, 0, 50);
#define PATH_COLOR sf::Color(0, 255, 0, 150)

StateGamePathfinding::StateGamePathfinding() : m_Begin(sf::Vector2i(0, 0)), m_End(sf::Vector2i(ARRAY_SIZE_X - 1, ARRAY_SIZE_Y - 1)), m_uiCurrentAlgo(0U), m_bPathFindingComputing(false), m_llExecutionTime(0LL), m_PathFindingThread(NULL)
{
	m_CaseArray = (stCASE**)malloc(ARRAY_SIZE_X * sizeof(stCASE*));
	for( int i(0); i < ARRAY_SIZE_X; i++ )
		m_CaseArray[i] = (stCASE*)malloc(ARRAY_SIZE_Y * sizeof(stCASE));

	//Tableau d'algos de pathfinding
	sfgmk::FoncterTemplateInstance<StateGamePathfinding, void>* zPathPtr = new sfgmk::FoncterTemplateInstance<StateGamePathfinding, void>(this, &StateGamePathfinding::zPath);
	m_PathfindingAlgos.m_FunctionsArray.pushBack(zPathPtr);

	sfgmk::FoncterTemplateInstance<StateGamePathfinding, void>* dijkstraPtr = new sfgmk::FoncterTemplateInstance<StateGamePathfinding, void>(this, &StateGamePathfinding::dijkstra);
	m_PathfindingAlgos.m_FunctionsArray.pushBack(dijkstraPtr);

	sfgmk::FoncterTemplateInstance<StateGamePathfinding, void>* astarPtr = new sfgmk::FoncterTemplateInstance<StateGamePathfinding, void>(this, &StateGamePathfinding::astar);
	m_PathfindingAlgos.m_FunctionsArray.pushBack(astarPtr);

	//Foncter du thread de pathfinding
	m_LaunchPathfinding = new sfgmk::FoncterTemplateInstance<StateGamePathfinding, void>(this, &StateGamePathfinding::computePathfinding);
	m_PathFindingThread.SetFunc(m_LaunchPathfinding);
}

StateGamePathfinding::~StateGamePathfinding()
{
	m_Path.clear();
	clearList();

	for( int i(0); i < ARRAY_SIZE_Y; i++ )
		free(m_CaseArray[i]);
	free(m_CaseArray);

	SAFE_DELETE(m_LaunchPathfinding);
}


void StateGamePathfinding::init()
{
	m_Font = DATA_MANAGER->getFont("sfgmk_ConsoleFont1");
	m_StepText.setFont(m_Font);
	m_StepText.setCharacterSize((unsigned int)(ARRAY_CASE_SIZE * 0.5f));
	m_StepText.setColor(sf::Color::White);

	//String algos
	m_AlgoStringArray[eZpath] = "Z-Path";
	m_AlgoStringArray[eDijkstra] = "Dijkstra";
	m_AlgoStringArray[eAStar] = "A*";

	//Textes HUD
	m_InstructionText.setFont(m_Font);
	m_InstructionText.setCharacterSize(16U);
	m_InstructionText.setColor(sf::Color::White);
	m_InstructionText.setString("Left click to place Start\nRight click to place End\nMiddle click to place Walls\nMiddle click + shift to remove Walls\nEnter to compute path\n\"Z\" and \"S\" to change algorythm.");
	m_InstructionText.setPosition(sf::Vector2f(10.0f, 10.0f));
	
	m_AlgoText.setFont(m_Font);
	m_AlgoText.setCharacterSize(32U);

	//Init tableau
	for( int i(0); i < ARRAY_SIZE_X; i++ )
	{
		for( int j(0); j < ARRAY_SIZE_Y; j++ )
		{
			m_CaseArray[i][j].bIswall = false;
			m_CaseArray[i][j].Predecessor = NULL;
		}
	}

	initArray();
}

void StateGamePathfinding::update()
{
	//Position actuelle souris
	sf::Vector2i FocusedCase = getMouseCase(sf::Vector2f(MAP_POS_X, MAP_POS_Y));

	//Inputs
	if( isInCases(FocusedCase) )
	{
		//Set start
		if( INPUT_MANAGER->MOUSE.getButtonState(sf::Mouse::Left) == KEY_PRESSED )
			m_Begin = FocusedCase;

		//Set end
		else if( INPUT_MANAGER->MOUSE.getButtonState(sf::Mouse::Right) == KEY_PRESSED )
			m_End = FocusedCase;

		//Wall
		else if( INPUT_MANAGER->MOUSE.getButtonState(sf::Mouse::Middle) == KEY_DOWN )
		{
			//Remove wall
			if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::LShift) == KEY_DOWN )
				m_CaseArray[FocusedCase.x][FocusedCase.y].bIswall = false;
			//Set wall
			else
				m_CaseArray[FocusedCase.x][FocusedCase.y].bIswall = true;
		}
	}

	//Change current algo
	if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::S) == KEY_PRESSED && m_uiCurrentAlgo < ePATHFINDING_ALGOS_NUMBER - 1 )
		m_uiCurrentAlgo++;
	else if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::Z) == KEY_PRESSED && m_uiCurrentAlgo > 0 )
		m_uiCurrentAlgo--;

	//Lancement pathfinding
	if( !m_bPathFindingComputing && INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::Return) == KEY_PRESSED )
	{
		m_Path.clear();
		initArray();
		m_bPathFindingComputing = true;

		m_PathFindingThread.Wait();
		m_PathFindingThread.Reset();
		m_PathFindingThread.Launch();
	}
}

void StateGamePathfinding::deinit()
{
}


void StateGamePathfinding::draw()
{
	sf::RenderTexture* Render = GRAPHIC_MANAGER->getRenderTexture();
	unsigned int uiSizeX = GRAPHIC_MANAGER->getRenderWindow()->getSize().x - 150U;

	//Instructions
	Render->draw(m_InstructionText);

	for( int i(0); i < ePATHFINDING_ALGOS_NUMBER; i++ )
	{
		if( i == m_uiCurrentAlgo )
			m_AlgoText.setColor(sf::Color::Red);
		else
			m_AlgoText.setColor(sf::Color::White);

		m_AlgoText.setString(m_AlgoStringArray[i]);
		m_AlgoText.setPosition((float)uiSizeX, (i + 1 ) * 25.0f);
		Render->draw(m_AlgoText);
	}

	//Draw start and end
	SHAPE_DRAWER.drawRectangle(sf::Vector2f(MAP_POS_X + m_Begin.x * ARRAY_CASE_SIZE, MAP_POS_Y + m_Begin.y * ARRAY_CASE_SIZE), sf::Vector2f(ARRAY_CASE_SIZE, ARRAY_CASE_SIZE), sf::Color::Green);
	SHAPE_DRAWER.drawRectangle(sf::Vector2f(MAP_POS_X + m_End.x * ARRAY_CASE_SIZE, MAP_POS_Y + m_End.y * ARRAY_CASE_SIZE), sf::Vector2f(ARRAY_CASE_SIZE, ARRAY_CASE_SIZE), sf::Color::Red);

	//Draw cases
	for( int i(0); i < ARRAY_SIZE_X; i++ )
	{
		for( int j(0); j < ARRAY_SIZE_Y; j++ )
		{	
			//Outline
			SHAPE_DRAWER.drawRectangle(sf::Vector2f(MAP_POS_X + i * ARRAY_CASE_SIZE, MAP_POS_Y + j * ARRAY_CASE_SIZE), sf::Vector2f(ARRAY_CASE_SIZE, ARRAY_CASE_SIZE), m_CaseArray[i][j].FillColor, -1, sf::Color::Blue);

			//Fill
			if( m_CaseArray[i][j].bIswall )
				SHAPE_DRAWER.drawRectangle(sf::Vector2f(MAP_POS_X + i * ARRAY_CASE_SIZE, MAP_POS_Y + j * ARRAY_CASE_SIZE), sf::Vector2f(ARRAY_CASE_SIZE, ARRAY_CASE_SIZE), sf::Color(150, 150, 150, 255));

			//Step
			if( m_CaseArray[i][j].bTested )
			{
				m_StepText.setString(std::to_string(m_CaseArray[i][j].iStep));
				m_StepText.setPosition(sf::Vector2f(MAP_POS_X, MAP_POS_Y) + sf::Vector2f(ARRAY_CASE_SIZE * 0.5f, ARRAY_CASE_SIZE * 0.5f) + sf::Vector2f(i * ARRAY_CASE_SIZE, j * ARRAY_CASE_SIZE) - sf::Vector2f(m_StepText.getGlobalBounds().width * 0.5f, m_StepText.getGlobalBounds().height * 0.5f));
				Render->draw(m_StepText);
			}
		}
	}

	//Draw path si on a fait un calcul
	if( m_Path.size() )
	{
		float fCircleRadius = ARRAY_CASE_SIZE * 0.25f;
		for( size_t i(0); i < m_Path.size(); i++ )
			SHAPE_DRAWER.drawCircle(sf::Vector2f(MAP_POS_X + m_Path[i].x * ARRAY_CASE_SIZE + fCircleRadius, MAP_POS_Y + m_Path[i].y * ARRAY_CASE_SIZE + fCircleRadius), fCircleRadius, 8U, PATH_COLOR);
	}
}


void StateGamePathfinding::initArray()
{
	for( int i(0); i < ARRAY_SIZE_X; i++ )
	{
		for( int j(0); j < ARRAY_SIZE_Y; j++ )
		{
			m_CaseArray[i][j].bTested = false;
			m_CaseArray[i][j].FillColor = sf::Color::Transparent;

			m_CaseArray[i][j].iStep = -1;

			m_CaseArray[i][j].fWeight = -1.0f;
			SAFE_DELETE(m_CaseArray[i][j].Predecessor);
		}
	}
}

void StateGamePathfinding::clearList()
{
	while( !m_List.empty() )
		m_List.pop();
}


sf::Vector2i StateGamePathfinding::getMouseCase(const sf::Vector2f& _MapDecal)
{
	sf::Vector2f Mouse = MOUSE_WORLD_POS;
	
	return sf::Vector2i((int)(Mouse.x / ARRAY_CASE_SIZE - _MapDecal.x / ARRAY_CASE_SIZE), (int)(Mouse.y / ARRAY_CASE_SIZE - _MapDecal.y / ARRAY_CASE_SIZE));
}

bool StateGamePathfinding::isInCases(const sf::Vector2i& _Position)
{
	return (_Position.x >= 0 && _Position.x < ARRAY_SIZE_X && _Position.y >= 0 && _Position.y < ARRAY_SIZE_Y);
}

bool StateGamePathfinding::checkDiagonalWall(const sf::Vector2i& _CaseOne, const sf::Vector2i& _CaseTwo)
{
	sf::Vector2i CasesToTest[2] = { sf::Vector2i(_CaseOne.x, _CaseTwo.y), sf::Vector2i(_CaseTwo.x, _CaseOne.y) };

	if( m_CaseArray[CasesToTest[0].x][CasesToTest[0].y].bIswall && m_CaseArray[CasesToTest[1].x][CasesToTest[1].y].bIswall )
		return true;
	else 
		return false;
}


void StateGamePathfinding::computePathfinding()
{
	std::cout << "Start pathfinding thread" << std::endl;

	m_CaseArray[m_Begin.x][m_Begin.y].iStep = 0;
	m_CaseArray[m_End.x][m_End.y].iStep = 0;

	m_Path.clear();
	
	//Lance le pathfinding
	m_llExecutionTime = measureFoncterExecutionTime(m_PathfindingAlgos.m_FunctionsArray[m_uiCurrentAlgo]);

	//Affichage temps d'exécution
	std::string sBuffer = std::to_string((double)(m_llExecutionTime * 0.001f));
	sBuffer = sBuffer.substr(0, sBuffer.find('.') + 3);
	std::cout << sBuffer + " ms" << std::endl;

	//Libére 
	m_bPathFindingComputing = false;
	std::cout << "End of pathfinding thread" << std::endl << std::endl;
}

void StateGamePathfinding::computeNextCases4(sf::Vector2i _CurrentCase, sf::Vector2i _Array[eNEXT_CASES_NUMBER_4])
{
	_Array[eTop] = _CurrentCase + sf::Vector2i(0, -1);
	_Array[eRight] = _CurrentCase + sf::Vector2i(1, 0);
	_Array[eBot] = _CurrentCase + sf::Vector2i(0, 1);
	_Array[eLeft] = _CurrentCase + sf::Vector2i(-1, 0);
}

void StateGamePathfinding::computeNextCases8(sf::Vector2i _CurrentCase, sf::Vector2i _Array[eNEXT_CASES_NUMBER_8])
{
	_Array[eTop] = _CurrentCase + sf::Vector2i(0, -1);
	_Array[eRight] = _CurrentCase + sf::Vector2i(1, 0);
	_Array[eBot] = _CurrentCase + sf::Vector2i(0, 1);
	_Array[eLeft] = _CurrentCase + sf::Vector2i(-1, 0);

	_Array[eTopLeft] = _CurrentCase + sf::Vector2i(-1, -1);
	_Array[eTopRight] = _CurrentCase + sf::Vector2i(1, -1);
	_Array[eBotRight] = _CurrentCase + sf::Vector2i(1, 1);
	_Array[eBotLeft] = _CurrentCase + sf::Vector2i(-1, 1);
}


void StateGamePathfinding::zPath()
{
	std::cout << "Start Z-Path algo." << std::endl;

	bool bStartFound(false);
	unsigned int iStep = 0;
	unsigned int uiListCurrentSize(0U);
	sf::Vector2i NextCases[eNEXT_CASES_NUMBER_4];
	stCASE* TempCase = NULL;
	sf::Vector2i* TempVector;
	m_CaseArray[m_End.x][m_End.y].bTested = true;

	//On démarre de la fin
	m_List.push(m_End);

	if( !(m_Begin == m_End) )
	{
		while( !bStartFound && m_List.size() )
		{
			iStep++;
			uiListCurrentSize = m_List.size();

			for( unsigned int i(0U); i < uiListCurrentSize; i++ )
			{
				TempVector = &m_List.front();

				//Cases adjacentes
				computeNextCases4(*TempVector, NextCases);

				for( int j(0); j < eNEXT_CASES_NUMBER_4; j++ )
				{
					//Si on est pas en dehors du tableau
					if( isInCases(NextCases[j]) )
					{
						TempCase = &m_CaseArray[NextCases[j].x][NextCases[j].y];

						//Si on a pas encore été testé, et qu'on est pas un mur
						if( !(TempCase->bIswall) && !(TempCase->bTested) )
						{
							TempCase->bTested = true;
							TempCase->iStep = iStep;
							TempCase->FillColor = FILL_COLOR;

							m_List.push(NextCases[j]);
						}

						//Si on trouve le point de départ
						if( NextCases[j].x == m_Begin.x && NextCases[j].y == m_Begin.y )
						{
							bStartFound = true;
							j = eNEXT_CASES_NUMBER_4;
							i = uiListCurrentSize;
						}
					}
				}
				m_List.pop();
			}
		}
	}
	
	//Libére la liste de nodes
	clearList();

	//Stocke le chemin calculé
	if( bStartFound )
		zPathComputeFoundPath(iStep);

	std::cout << "Z-Path algo achieved ( " + std::to_string(iStep) + " steps) in ";
}

void StateGamePathfinding::zPathComputeFoundPath(unsigned int _Step)
{
	sf::Vector2i NextCases[eNEXT_CASES_NUMBER_8];
	sf::Vector2i TempCaseIndexs = m_Begin;
	bool bEndFound(false);

	//Mémorisation path
	m_Path.push_back(TempCaseIndexs);

	if( _Step == 0 )
		m_Path.push_back(m_End);

	else
	{
		while( !bEndFound )
		{
			computeNextCases8(TempCaseIndexs, NextCases);

			for( int j(0); j < eNEXT_CASES_NUMBER_8; j++ )
			{
				//Si on est pas en dehors du tableau
				if( isInCases(NextCases[j]) )
				{
					//Si la case a été calculée
					if( m_CaseArray[NextCases[j].x][NextCases[j].y].iStep != -1 )
					{
						//Si on est pas un mur et qu'on ne traverse pas un mur en diagonale
						if( !(m_CaseArray[NextCases[j].x][NextCases[j].y].bIswall) && !checkDiagonalWall(m_Path[m_Path.size() - 1], NextCases[j]) )
						{
							if( m_CaseArray[NextCases[j].x][NextCases[j].y].iStep < m_CaseArray[TempCaseIndexs.x][TempCaseIndexs.y].iStep )
								TempCaseIndexs = NextCases[j];
						}
					}
				}
			}
			m_Path.push_back(TempCaseIndexs);
			if( TempCaseIndexs == m_End )
				bEndFound = true;
		}
	}
}


void StateGamePathfinding::dijkstra()
{
	std::cout << "Start Dijkstra algo." << std::endl;

	bool bStartFound(false);
	unsigned int iStep = 0;
	unsigned int uiListCurrentSize(0U);
	sf::Vector2i NextCases[eNEXT_CASES_NUMBER_4];
	stCASE* TempCase = NULL;
	sf::Vector2i* TempVector;
	m_CaseArray[m_Begin.x][m_Begin.y].bTested = true;
	m_CaseArray[m_Begin.x][m_Begin.y].fWeight = 0.0f;

	//On démarre de la fin
	m_List.push(m_Begin);

	while( !bStartFound && m_List.size() )
	{
		iStep++;
		uiListCurrentSize = m_List.size();

		for( unsigned int i(0U); i < uiListCurrentSize; i++ )
		{
			TempVector = &m_List.front();

			//Cases adjacentes
			computeNextCases4(*TempVector, NextCases);

			for( int j(0); j < eNEXT_CASES_NUMBER_8; j++ )
			{
				//Si on est pas en dehors du tableau
				if( isInCases(NextCases[j]) )
				{
					TempCase = &m_CaseArray[NextCases[j].x][NextCases[j].y];

					//Si on a pas encore été testé, et qu'on est pas un mur
					if( !(TempCase->bIswall) && !(TempCase->bTested) && !checkDiagonalWall(*TempVector, NextCases[j]) && TempCase->fWeight < (m_CaseArray[TempVector->x][TempVector->y].fWeight + 1.0f) )
					{
						TempCase->bTested = true;
						TempCase->fWeight = m_CaseArray[TempVector->x][TempVector->y].fWeight + 1.0f;
						TempCase->FillColor = FILL_COLOR;
						TempCase->Predecessor = new sf::Vector2i(*TempVector);

						m_List.push(NextCases[j]);
					}

					//Si on trouve le point de départ
					if( NextCases[j].x == m_Begin.x && NextCases[j].y == m_Begin.y )
					{
						bStartFound = true;
						j = eNEXT_CASES_NUMBER_8;
						i = uiListCurrentSize;
					}
				}
			}
			m_List.pop();
		}
	}

	//Libére la liste de nodes
	clearList();

	std::cout << "Dijkstra algo achieved ( " /*+ std::to_string(uiStep) + " steps) in "*/;
}

int StateGamePathfinding::astar_search_in_list(sf::Vector2i _node, std::vector<NodeAStar*>& _list)
{
	for (unsigned int i(0); i < _list.size(); i++)
		if (_node == _list[i]->grid_node)
			return (int)i;
	
	return -1;
}

void StateGamePathfinding::astar_remove_from_list(NodeAStar* _node, std::vector<NodeAStar*>& _list, bool _delete)
{
	for (unsigned int i(0); i < _list.size(); i++)
	{
		if (_node == _list[i])
		{
			_list[i] = _list[_list.size() - 1];
			_list.pop_back();

			if (_delete)
				delete _node;
		}
	}
}

StateGamePathfinding::NodeAStar* StateGamePathfinding::astar_find_smallest(std::vector<NodeAStar*>& _list)
{
	float value = -1.0f;
	NodeAStar* node = 0;

	for (NodeAStar*& _node : _list)
	{
		if (_node->estimated_total_cost < value || value < 0.0f)
		{
			node = _node;
			value = _node->estimated_total_cost;
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

	while (open_list.size() > 0)
	{
		NodeAStar* smallest = astar_find_smallest(open_list);

		if (smallest->grid_node == m_End)
		{
			found = true;
			break;
		}

		sf::Vector2i expanded_nodes[8];
		computeNextCases8(smallest->grid_node, expanded_nodes);

		for (int i = 0; i < 8; i++)
		{
			if (isInCases(expanded_nodes[i]))
			{
				stCASE* node = &m_CaseArray[expanded_nodes[i].x][expanded_nodes[i].y];

				if (!node->bIswall && astar_search_in_list(expanded_nodes[i], closed_list) < 0 && astar_search_in_list(expanded_nodes[i], open_list) < 0)
				{
					cost = 1.0f;

					if (i >= 4)
						cost = 1.4f;

					NodeAStar* newNode = new NodeAStar(expanded_nodes[i], smallest);
					newNode->cost_so_far = newNode->parent->cost_so_far + cost;
					newNode->heuristic = astar_heuristic(newNode->grid_node);
					newNode->estimated_total_cost = newNode->cost_so_far + newNode->heuristic;

					node->bTested = true;
					node->iStep = (int)newNode->estimated_total_cost;

					open_list.push_back(newNode);
				}
			}
		}

		astar_remove_from_list(smallest, open_list, false);

		closed_list.push_back(smallest);
	}

	std::cout << (found ? "FOUND!" : "Not Found :( !") << std::endl;

	// Final path

	if (found)
	{
		NodeAStar* curr = astar_find_smallest(open_list);

		while (curr->parent)
		{
			m_Path.push_back(curr->grid_node);
			curr = curr->parent;
		}

		m_Path.push_back(curr->grid_node);
	}

	std::cout << "AStar algo achieved ( ";
}