#include "stdafx.h"
#include "../../IncludesProjet.hpp"

using namespace sfgmk;


#define ARRAY_SIZE_X 16
#define ARRAY_SIZE_Y 16
#define ARRAY_CASE_SIZE 32.0f
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
	sfgmk::FoncterTemplateInstance<StateGamePathfinding, void>* zPathptr = new sfgmk::FoncterTemplateInstance<StateGamePathfinding, void>(this, &StateGamePathfinding::zPath);
	m_PathfindingAlgos.m_FunctionsArray.pushBack(zPathptr);

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

	m_InstructionText.setFont(m_Font);
	m_InstructionText.setCharacterSize(16U);
	m_InstructionText.setColor(sf::Color::White);
	m_InstructionText.setString("Left click to place Start\nRight click to place End\nMiddle click to place Walls\nMiddle click + shift to remove Walls\nEnter to compute path");
	m_InstructionText.setPosition(sf::Vector2f(10.0f, 10.0f));
	
	//Init tableau
	initArray();
	
	for( int i(0); i < ARRAY_SIZE_X; i++ )
	{
		for( int j(0); j < ARRAY_SIZE_Y; j++ )
			m_CaseArray[i][j].bIswall = false;
	}
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

	//Instructions
	Render->draw(m_InstructionText);

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
				m_StepText.setString(std::to_string(m_CaseArray[i][j].uiStep));
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
			m_CaseArray[i][j].uiStep = UINT_MAX;
			m_CaseArray[i][j].FillColor = sf::Color::Transparent;
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


void StateGamePathfinding::computePathfinding()
{
	std::cout << "Start pathfinding thread" << std::endl;

	m_CaseArray[m_Begin.x][m_Begin.y].uiStep = 0U;
	m_CaseArray[m_End.x][m_End.y].uiStep = 0U;
	
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
	unsigned int uiStep = 0U;
	unsigned int uiListCurrentSize(0U);
	sf::Vector2i NextCases[eNEXT_CASES_NUMBER_4];
	stCASE* TempCase = NULL;
	sf::Vector2i* TempVector;
	m_CaseArray[m_End.x][m_End.y].bTested = true;

	//On démarre de la fin
	m_List.push(m_End);

	while( !bStartFound && m_List.size() )
	{
		uiStep++;
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
						TempCase->uiStep = uiStep;
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
	
	//Libére la liste de nodes
	clearList();

	//Stocke le chemin calculé
	if( bStartFound )
		zPathComputeFoundPath(uiStep - 1U);

	std::cout << "Z-Path algo achieved ( " + std::to_string(uiStep) + " steps) in ";
}

void StateGamePathfinding::zPathComputeFoundPath(unsigned int _Step)
{
	_Step -= 2U; //-2 car on ne compte pas le départ et l'arrivée

	sf::Vector2i NextCases[eNEXT_CASES_NUMBER_8];
	sf::Vector2i TempCaseIndexs = m_Begin;
	int iIndex(0);

	//Mémorisation path
	m_Path.push_back(m_Begin);

	for( unsigned int i(0); i < _Step; i++ )
	{
		iIndex++;
		computeNextCases8(TempCaseIndexs, NextCases);

		for( int j(0); j < eNEXT_CASES_NUMBER_8; j++ )
		{
			//Si on est pas en dehors du tableau
			if( isInCases(NextCases[j]) )
			{
				//Si on est pas un mur
				if( !(m_CaseArray[NextCases[j].x][NextCases[j].y].bIswall) )
				{
					if( m_CaseArray[NextCases[j].x][NextCases[j].y].uiStep < m_CaseArray[TempCaseIndexs.x][TempCaseIndexs.y].uiStep )
						TempCaseIndexs = NextCases[j];
				}
			}
		}
		m_Path.push_back(TempCaseIndexs);
	}
	m_Path.push_back(m_End);
}