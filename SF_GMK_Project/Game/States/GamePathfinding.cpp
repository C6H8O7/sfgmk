#include "stdafx.h"
#include "../../IncludesProjet.hpp"

using namespace sfgmk;


#define FILE_DIR "../data/states/GamePathfinding/pathfindingMap"
#define ARRAY_CASE_SIZE 32.0f
#define HUD_SIZE sf::Vector2f(ARRAY_CASE_SIZE * 4.0f, ARRAY_CASE_SIZE)
#define CASE_OUTLINE_COLOR sf::Color::Blue
#define WALL_COLOR sf::Color(150, 150, 150, 255)
#define PATH_COLOR sf::Color(127, 255, 0, 100)

StateGamePathfinding::StateGamePathfinding() : m_ArraySize(0, 0), m_Begin(-1, -1), m_End(-1, -1), m_CaseArray(NULL), m_RenderCases(NULL), m_RenderCasesState(NULL), m_uiAlgoChosen(0U)
{
	m_Font = DATA_MANAGER->getFont("sfgmk_ConsoleFont1");
	m_PathfindingText.setFont(m_Font);
	m_PathfindingText.setCharacterSize((unsigned int)(ARRAY_CASE_SIZE * 0.25f));
	m_PathfindingText.setColor(sf::Color::White);

	m_HudText.setFont(m_Font);
	m_HudText.setCharacterSize(16U);
	m_HudText.setColor(sf::Color::White);
}

StateGamePathfinding::~StateGamePathfinding()
{
	m_MapFileName.clear();
	m_Path.clear();
	deleteArray();
	SAFE_DELETE(m_RenderCases);
	SAFE_DELETE(m_RenderCasesState);
}


void StateGamePathfinding::init()
{
	//Algos
	m_sAlgosNames[eZpath] = "Z-Path";
	m_sAlgosNames[eDijkstra] = "Dijkstra";
	m_sAlgosNames[eAStar] = "A*";

	//Fichiers de map
	DIR* LevelRepertory = NULL;
	struct dirent* ReadFile = NULL;

	LevelRepertory = opendir(FILE_DIR);

	if( LevelRepertory )
	{
		//Caractères '.' et ".."
		for( int i(0); i < 2; i++ )
			readdir(LevelRepertory);

		while( (ReadFile = readdir(LevelRepertory)) != NULL ) //On récupére tous les fichiers contenus
			m_MapFileName.push_back(ReadFile->d_name);
		closedir(LevelRepertory);
	}
}

void StateGamePathfinding::update()
{
	ShapeDrawer* Drawer = &SHAPE_DRAWER;
	sf::Vector2f MouseWorld = MOUSE_WORLD_POS;
	sf::Vector2i MouseWindow = MOUSE_WINDOW_POS;
	sf::Vector2i FocusedCase = getMouseCase(MOUSE_WORLD_POS, HUD_SIZE);

	//Clic dans la grille
	if( isInCases(FocusedCase) )
	{
		if( !m_CaseArray[FocusedCase.x][FocusedCase.y].bIswall )
		{
			//Set start
			if( INPUT_MANAGER->MOUSE.getButtonState(sf::Mouse::Left) == KEY_PRESSED )
			{
				drawCase(sf::Vector2f((float)m_Begin.x, (float)m_Begin.y), sf::Color::Black);
				m_Begin = FocusedCase;
				drawCase(sf::Vector2f((float)FocusedCase.x, (float)FocusedCase.y), sf::Color::Green);
			}

			//Set end
			else if( INPUT_MANAGER->MOUSE.getButtonState(sf::Mouse::Right) == KEY_PRESSED )
			{
				drawCase(sf::Vector2f((float)m_End.x, (float)m_End.y), sf::Color::Black);
				m_End = FocusedCase;
				drawCase(sf::Vector2f((float)FocusedCase.x, (float)FocusedCase.y), sf::Color::Red);
			}
		}

		//Wall
		if( INPUT_MANAGER->MOUSE.getButtonState(sf::Mouse::Middle) == KEY_DOWN )
		{
			//Remove wall
			if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::LShift) == KEY_DOWN )
			{
				m_CaseArray[FocusedCase.x][FocusedCase.y].bIswall = false;
				drawCase(sf::Vector2f((float)FocusedCase.x, (float)FocusedCase.y), sf::Color::Black);
			}
			//Set wall
			else
			{
				m_CaseArray[FocusedCase.x][FocusedCase.y].bIswall = true;
				drawCase(sf::Vector2f((float)FocusedCase.x, (float)FocusedCase.y), WALL_COLOR);
			}
		}
	}
	//Clic HUD
	else
	{
		if( INPUT_MANAGER->MOUSE.getButtonState(sf::Mouse::Left) == KEY_PRESSED )
		{
			//Chargement fichier
			float fTextSize = (float)m_HudText.getCharacterSize();
			sf::Vector2f Position = sf::Vector2f(0.0f, fTextSize);
			int iIndex(1);

			for( std::string& FileName : m_MapFileName )
			{
				if( MouseWindow.x >= Position.x &&  MouseWindow.x <= FileName.length() * fTextSize * 0.5f && MouseWindow.y >= Position.y && MouseWindow.y <= Position.y + fTextSize )
					loadFile(FileName);

				iIndex++;
				Position.y += fTextSize;
			}
		}
	}

	//Change current algo
	if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::S) == KEY_PRESSED && m_uiAlgoChosen < ePATHFINDING_ALGOS_NUMBER - 1 )
		m_uiAlgoChosen++;
	else if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::Z) == KEY_PRESSED && m_uiAlgoChosen > 0 )
		m_uiAlgoChosen--;

	//Lancement pathfinding
	if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::Return) == KEY_PRESSED )
	{
		m_Path.clear();

		for( int i(0); i < m_ArraySize.x; i++ )
		{
			for( int j(0); j < m_ArraySize.y; j++ )
			{
				m_CaseArray[i][j].bTested = false;
				m_CaseArray[i][j].uiStep = 0;
			}
		}

		std::cout << "Pathfinding computing..." << std::endl;
		m_Pathfinding.computePathfinding(&m_Path, (const ePATHFINDING_ALGOS)m_uiAlgoChosen, m_CaseArray, m_ArraySize, m_Begin, m_End);
	}
}

void StateGamePathfinding::deinit()
{
}


void StateGamePathfinding::draw()
{
	sf::RenderTexture* Render = GRAPHIC_MANAGER->getRenderTexture();
	ShapeDrawer* Drawer = &SHAPE_DRAWER;

	//Draw fichiers
	float fTextSize = (float)m_HudText.getCharacterSize();

	m_HudText.setColor(sf::Color::White);
	m_HudText.setPosition(sf::Vector2f(0.0f, 0.0f));
	m_HudText.setString("Files:");
	Render->draw(m_HudText);
	for( std::string& FileName : m_MapFileName )
	{
		m_HudText.setString(FileName);
		m_HudText.setPosition(m_HudText.getPosition() + sf::Vector2f(0.0f, fTextSize));
		Render->draw(m_HudText);
	}

	//Draw algos
	m_HudText.setPosition(m_HudText.getPosition() + sf::Vector2f(0.0f, fTextSize));
	for( unsigned int i(0); i < ePATHFINDING_ALGOS_NUMBER; i++ )
	{
		if( m_uiAlgoChosen == i )
			m_HudText.setColor(sf::Color::Red);
		else
			m_HudText.setColor(sf::Color::White);

		m_HudText.setString(m_sAlgosNames[i]);
		m_HudText.setPosition(m_HudText.getPosition() + sf::Vector2f(0.0f, fTextSize));
		Render->draw(m_HudText);
	}

	//Draw cases
	Render->draw(m_RenderCasesSprite);
	Render->draw(m_RenderCasesSpriteState);

	//Draw path
	for( int i(0); i < m_Path.size(); i++ )
		Drawer->drawCircle(sf::Vector2f(m_Path[i].x * ARRAY_CASE_SIZE + HUD_SIZE.x, m_Path[i].y * ARRAY_CASE_SIZE + HUD_SIZE.y), ARRAY_CASE_SIZE * 0.5f, 8, PATH_COLOR);
}


void StateGamePathfinding::initArray()
{
	//Allocation mémoire
	m_CaseArray = (stPATHFINDING_CASE**)malloc(sizeof(stPATHFINDING_CASE*) * m_ArraySize.x);
	if( m_CaseArray == NULL )
		std::cout << "Error" << std::endl;
	else
	{
		for( int i(0); i < m_ArraySize.x; i++ )
		{
			m_CaseArray[i] = (stPATHFINDING_CASE*)malloc(sizeof(stPATHFINDING_CASE) * m_ArraySize.y);
			if( m_CaseArray[i] == NULL )
				std::cout << "Error" << std::endl;
		}
	}

	//Init cases
	for( int i(0); i < m_ArraySize.x; i++ )
	{
		for( int j(0); j < m_ArraySize.y; j++ )
		{
			m_CaseArray[i][j].bTested = false;
			m_CaseArray[i][j].bIswall = false;
			m_CaseArray[i][j].uiStep = 0;
		}
	}
}

void StateGamePathfinding::deleteArray()
{
	if( m_CaseArray != NULL )
	{
		for( int i(0); i < m_ArraySize.x; i++ )
		{
			free(m_CaseArray[i]);
			m_CaseArray[i] = NULL;
		}
		free(m_CaseArray);
		m_CaseArray = NULL;
	}
}


void StateGamePathfinding::loadFile(const std::string& _FileName)
{
	FILE* FileToLoad = NULL;
	std::string sFileName(FILE_DIR);
	sFileName += ("/" + _FileName);
	fopen_s(&FileToLoad, sFileName.c_str(), "r");

	if( FileToLoad )
	{
		std::cout << "Load file " << _FileName << std::endl;

		m_Begin = m_End = sf::Vector2i(-1, -1);
		m_Path.clear();

		//Taille map
		deleteArray();
		fscanf_s(FileToLoad, "%d %d", &m_ArraySize.x, &m_ArraySize.y);
		initArray();
		
		//Map
		char cBuffer = '\0';
		int iX(0), iY(0);

		fgetc(FileToLoad); //'\n' première ligne
		while( (cBuffer = fgetc(FileToLoad)) != EOF )
		{
			if( cBuffer == '\n' )
			{

				iX++;
				iY = 0;
			}
			else
			{
				if( cBuffer == 48 )
					m_CaseArray[iX][iY].bIswall = false;
				else if( cBuffer == 49 )
					m_CaseArray[iX][iY].bIswall = true;

				iY++;
			}
		}

		fclose(FileToLoad);

		//Dessin des cases
		ShapeDrawer* Drawer = &SHAPE_DRAWER;

		SAFE_DELETE(m_RenderCases);
		m_RenderCases = new sf::RenderTexture();
		m_RenderCases->create((unsigned int)(ARRAY_CASE_SIZE * m_ArraySize.x), (unsigned int)(ARRAY_CASE_SIZE * m_ArraySize.y));
		m_RenderCases->clear();

		SAFE_DELETE(m_RenderCasesState);
		m_RenderCasesState = new sf::RenderTexture();
		m_RenderCasesState->create((unsigned int)(ARRAY_CASE_SIZE * m_ArraySize.x), (unsigned int)(ARRAY_CASE_SIZE * m_ArraySize.y));
		m_RenderCasesState->clear(sf::Color::Transparent);

		for( int i(0); i < m_ArraySize.x; i++ )
		{
			for( int j(0); j < m_ArraySize.y; j++ )
			{
				Drawer->drawRectangle(sf::Vector2f(i * ARRAY_CASE_SIZE, j * ARRAY_CASE_SIZE), sf::Vector2f(ARRAY_CASE_SIZE, ARRAY_CASE_SIZE), m_RenderCases, sf::Color::Transparent, -1.0, CASE_OUTLINE_COLOR);
				if( m_CaseArray[j][i].bIswall )
					drawCase(sf::Vector2f((float)i, (float)j), WALL_COLOR);
			}
		}

		m_RenderCases->display();
		m_RenderCasesSprite.setTexture(m_RenderCases->getTexture(), true);
		m_RenderCasesSprite.setPosition(HUD_SIZE);

		m_RenderCasesState->display();
		m_RenderCasesSpriteState.setTexture(m_RenderCasesState->getTexture(), true);
		m_RenderCasesSpriteState.setPosition(HUD_SIZE);
	}
}


sf::Vector2i StateGamePathfinding::getMouseCase(const sf::Vector2f& _MouseWorldPos, const sf::Vector2f& _MapDecal)
{
	return sf::Vector2i((int)(_MouseWorldPos.x / ARRAY_CASE_SIZE - _MapDecal.x / ARRAY_CASE_SIZE), (int)(_MouseWorldPos.y / ARRAY_CASE_SIZE - _MapDecal.y / ARRAY_CASE_SIZE));
}

bool StateGamePathfinding::isInCases(const sf::Vector2i& _Position)
{
	return (_Position.x >= 0 && _Position.x < m_ArraySize.x && _Position.y >= 0 && _Position.y < m_ArraySize.y);
}


void StateGamePathfinding::drawCase(const sf::Vector2f& _Position, const sf::Color& _Color)
{
	SHAPE_DRAWER.drawRectangle(sf::Vector2f(_Position.x * ARRAY_CASE_SIZE + 1.0f, _Position.y * ARRAY_CASE_SIZE + 1.0f), sf::Vector2f(ARRAY_CASE_SIZE - 2.0f, ARRAY_CASE_SIZE - 2.0f), m_RenderCasesState, _Color);
}