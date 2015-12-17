#include "stdafx.h"
#include "../../IncludesProjet.hpp"

using namespace sfgmk;


#define FILE_DIR std::string("../data/states/GamePathfinding/pathfindingMap")
#define ARRAY_CASE_SIZE 16.0f
#define HUD_SIZE sf::Vector2f(128.0f, 32.0f)
#define BEGIN_COLOR sf::Color::Green
#define END_COLOR sf::Color::Red
#define CASE_OUTLINE_COLOR sf::Color::Blue
#define WALL_COLOR sf::Color(150, 150, 150, 255)
#define PATH_COLOR sf::Color(200, 255, 0, 150)
#define EXPLORATION_COLOR sf::Color(100, 255, 0, 100)

StateGamePathfinding::StateGamePathfinding() : m_uiAlgoChosen(0U), m_Begin(sf::Vector2i(-1, -1)), m_End(sf::Vector2i(-1, -1)), m_RenderExploration(NULL)
{
}

StateGamePathfinding::~StateGamePathfinding()
{
	m_MapFileName.clear();
	m_Path.clear();
}


void StateGamePathfinding::init()
{
	CONSOLE.command("/freecam");

	//Init datas
	m_Font = DATA_MANAGER->getFont("sfgmk_ConsoleFont1");
	m_PathfindingText.setFont(m_Font);
	m_PathfindingText.setCharacterSize((unsigned int)(ARRAY_CASE_SIZE * 0.5f));
	m_PathfindingText.setColor(sf::Color::White);

	m_HudText.setFont(m_Font);
	m_HudText.setCharacterSize(16U);
	m_HudText.setColor(sf::Color::White);

	m_sAlgosNames[eZpath] = "Z-Path";
	m_sAlgosNames[eDijkstra] = "Dijkstra";
	m_sAlgosNames[eAStar] = "A*";
	m_sAlgosNames[eJps] = "Jps";
	m_sAlgosNames[eAStarKcc] = "A*Kcc";
	m_sAlgosNames[eJpsKcc] = "JpsKcc";

	//Fichiers de map
	DIR* LevelRepertory = NULL;
	struct dirent* ReadFile = NULL;

	LevelRepertory = opendir(FILE_DIR.c_str());

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
	sf::Vector2i FocusedCase = getMouseCase(MOUSE_WORLD_POS, HUD_SIZE);

	//Clic dans la grille
	if( isInCases(FocusedCase) )
	{
		if( !m_Map.getTerrainType(FocusedCase.x, FocusedCase.y) == ePATHFINDING_TERRAIN_TYPE::eWALL )
		{
			//Set start
			if( INPUT_MANAGER->MOUSE.getButtonState(sf::Mouse::Left) == KEY_PRESSED )
				m_Begin = FocusedCase;

			//Set end
			else if( INPUT_MANAGER->MOUSE.getButtonState(sf::Mouse::Right) == KEY_PRESSED )
				m_End = FocusedCase;
		}

		//Wall
		if( INPUT_MANAGER->MOUSE.getButtonState(sf::Mouse::Middle) == KEY_DOWN )
		{
			//Remove wall
			if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::LShift) == KEY_DOWN )
				m_Map.setTerrainType(FocusedCase.x, FocusedCase.y, ePATHFINDING_TERRAIN_TYPE::eGROUND);
			
			//Set wall
			else
				m_Map.setTerrainType(FocusedCase.x, FocusedCase.y, ePATHFINDING_TERRAIN_TYPE::eWALL);
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
				if( MouseWorld.x >= Position.x &&  MouseWorld.x <= FileName.length() * fTextSize * 0.5f && MouseWorld.y >= Position.y && MouseWorld.y <= Position.y + fTextSize )
				{
					m_Path.clear();
					m_Pathfinding.resetMap();
					m_Map.loadMapFromFile((std::string(FILE_DIR + '/' + FileName).c_str()), m_Begin, m_End);
				}
				
				iIndex++;
				Position.y += fTextSize;
			}

			SAFE_DELETE(m_RenderExploration);
			m_RenderExploration = new sf::RenderTexture();
			m_RenderExploration->create(ARRAY_CASE_SIZE * m_Map.getMapWidth(), ARRAY_CASE_SIZE * m_Map.getMapHeight());
			m_RenderExploration->clear(sf::Color::Transparent);
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

		std::cout << "Pathfinding computing..." << std::endl;
		//std::thread* NewPathfindingThread  = new std::thread(&Pathfinding::computePathfinding, &m_Pathfinding, &m_Path, ePATHFINDING_ALGOS(m_uiAlgoChosen), &m_Map, m_Begin, m_End);
		m_Pathfinding.computePathfinding(&m_Path, ePATHFINDING_ALGOS(m_uiAlgoChosen), &m_Map, m_Begin, m_End);
		drawExploration();
	}
}

void StateGamePathfinding::deinit()
{
}


void StateGamePathfinding::draw()
{
	sf::RenderTexture* Render = GRAPHIC_MANAGER->getRenderTexture();

	//Draw fichiers
	float fTextSize = (float)m_HudText.getCharacterSize();

	m_HudText.setColor(sf::Color::Yellow);
	m_HudText.setPosition(sf::Vector2f(0.0f, 0.0f));
	m_HudText.setString("Files:");
	Render->draw(m_HudText);

	m_HudText.setColor(sf::Color::White);
	for( std::string& FileName : m_MapFileName )
	{
		m_HudText.setString(FileName);
		m_HudText.setPosition(m_HudText.getPosition() + sf::Vector2f(0.0f, fTextSize));
		Render->draw(m_HudText);
	}

	//Draw algos
	m_HudText.setPosition(m_HudText.getPosition() + sf::Vector2f(0.0f, fTextSize * 2.0f));
	m_HudText.setColor(sf::Color::Yellow);
	m_HudText.setString("Algo:");
	Render->draw(m_HudText);

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

	//Draw map
	sf::Vector2f DecalX(0.0f, ARRAY_CASE_SIZE);
	sf::Vector2f DecalY(ARRAY_CASE_SIZE, 0.0f);
	sf::Vertex LineX[] = { sf::Vertex(sf::Vector2f(HUD_SIZE.x, HUD_SIZE.y), CASE_OUTLINE_COLOR), sf::Vertex(sf::Vector2f(HUD_SIZE.x + ARRAY_CASE_SIZE * m_Map.getMapWidth(), HUD_SIZE.y), CASE_OUTLINE_COLOR) };
	sf::Vertex LineY[] = { sf::Vertex(sf::Vector2f(HUD_SIZE.x, HUD_SIZE.y), CASE_OUTLINE_COLOR), sf::Vertex(sf::Vector2f(HUD_SIZE.x, HUD_SIZE.y + ARRAY_CASE_SIZE * m_Map.getMapHeight()), CASE_OUTLINE_COLOR) };
	
	for( int i(0); i <= m_Map.getMapHeight(); i++ )
	{
		Render->draw(LineX, 2, sf::Lines);
		LineX[0].position += DecalX;
		LineX[1].position += DecalX;
	}
	for( int i(0); i <= m_Map.getMapWidth(); i++ )
	{
		Render->draw(LineY, 2, sf::Lines);
		LineY[0].position += DecalY;
		LineY[1].position += DecalY;
	}

	//Draw walls
	sf::RectangleShape Case(sf::Vector2f(ARRAY_CASE_SIZE - 2.0f, ARRAY_CASE_SIZE - 2.0f));
	Case.setFillColor(WALL_COLOR);
	Case.setOutlineThickness(0);

	for( int i(0); i < m_Map.getMapWidth(); i++ )
	{
		for( int j(0); j < m_Map.getMapHeight(); j++ )
		{
			if( m_Map.getTerrainType(i, j) == eWALL )
			{
				Case.setPosition(sf::Vector2f(HUD_SIZE.x + i * ARRAY_CASE_SIZE + 1.0f, HUD_SIZE.y + j * ARRAY_CASE_SIZE + 1.0f));
				Render->draw(Case);
			}
		}
	}

	//Draw exploration
	if( m_Path.size() )
		Render->draw(m_ExplorationSprite);

	//Draw begin / end
	Case.setFillColor(BEGIN_COLOR);
	Case.setPosition(sf::Vector2f(HUD_SIZE.x + m_Begin.x * ARRAY_CASE_SIZE + 1.0f, HUD_SIZE.y + m_Begin.y * ARRAY_CASE_SIZE + 1.0f));
	Render->draw(Case);

	Case.setFillColor(END_COLOR);
	Case.setPosition(sf::Vector2f(HUD_SIZE.x + m_End.x * ARRAY_CASE_SIZE + 1.0f, HUD_SIZE.y + m_End.y * ARRAY_CASE_SIZE + 1.0f));
	Render->draw(Case);
}


sf::Vector2i StateGamePathfinding::getMouseCase(const sf::Vector2f& _MouseWorldPos, const sf::Vector2f& _MapDecal)
{
	return sf::Vector2i((int)((_MouseWorldPos.x - _MapDecal.x) / ARRAY_CASE_SIZE), (int)((_MouseWorldPos.y - _MapDecal.y) / ARRAY_CASE_SIZE));
}

bool StateGamePathfinding::isInCases(const sf::Vector2i& _Position)
{
	return (_Position.x >= 0 && _Position.x < m_Map.getMapWidth() && _Position.y >= 0 && _Position.y < m_Map.getMapHeight());
}


void StateGamePathfinding::drawExploration()
{
	m_RenderExploration->clear(sf::Color::Transparent);
	sf::Vector2f TextSize;

	//Draw exploration
	sf::RectangleShape Case(sf::Vector2f(ARRAY_CASE_SIZE - 2.0f, ARRAY_CASE_SIZE - 2.0f));
	Case.setFillColor(EXPLORATION_COLOR);
	Case.setOutlineThickness(0);

	stPATHFINDING_SIMPLIFIED_NODE** SimplifiedMap = m_Pathfinding.getSimplifiedMap();
	if( SimplifiedMap )
	{
		for( int i(0); i < m_Map.getMapWidth(); i++ )
		{
			for( int j(0); j < m_Map.getMapHeight(); j++ )
			{
				if( SimplifiedMap[i][j].bTested )
				{
					Case.setPosition(sf::Vector2f(i * ARRAY_CASE_SIZE + 1.0f, j * ARRAY_CASE_SIZE + 1.0f));
					m_RenderExploration->draw(Case);
				}
			}
		}
	}

	//Draw path
	sf::CircleShape Circle(ARRAY_CASE_SIZE * 0.5f, 8);
	Circle.setFillColor(PATH_COLOR);
	Circle.setOutlineThickness(0);

	for( size_t i(0); i < m_Path.size(); i++ )
	{
		Circle.setPosition(sf::Vector2f(m_Path[i].x * ARRAY_CASE_SIZE, m_Path[i].y * ARRAY_CASE_SIZE));
		m_RenderExploration->draw(Circle);
	}

	//Indices
	/*for( int i(0); i < m_ArraySize.x; i++ )
	{
		for( int j(0); j < m_ArraySize.y; j++ )
		{
			if( m_CaseArray[i][j].bTested )
			{
				drawCase(m_RenderExploration, sf::Vector2f((float)i, (float)j), EXPLORATION_COLOR);

				m_PathfindingText.setString(std::to_string(m_CaseArray[i][j].uiStep));
				TextSize = sf::Vector2f(m_PathfindingText.getGlobalBounds().width, m_PathfindingText.getGlobalBounds().height);
				m_PathfindingText.setOrigin(TextSize.x * 0.5f, TextSize.y * 0.5f);
				m_PathfindingText.setPosition(sf::Vector2f((float)i * ARRAY_CASE_SIZE + ARRAY_CASE_SIZE * 0.5f, (float)j * ARRAY_CASE_SIZE + ARRAY_CASE_SIZE * 0.5f));

				m_RenderExploration->draw(m_PathfindingText);
			}
		}
	}*/

	m_RenderExploration->display();
	m_ExplorationSprite.setTexture(m_RenderExploration->getTexture(), true);
	m_ExplorationSprite.setPosition(HUD_SIZE);
}