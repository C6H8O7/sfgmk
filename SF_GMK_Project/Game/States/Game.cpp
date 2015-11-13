#include "stdafx.h"
#include "../IncludesProjet.hpp"

using namespace sfgmk;


StateGame::StateGame()
{
}

StateGame::~StateGame()
{
	ENTITY_MANAGER->freeEntityVector();
	DATA_MANAGER->unloadLevel(m_sRessourcesPath);
	PARALLAXE->unloadLevel(m_sRessourcesPath);
	SOUND_MANAGER->unloadLevel(m_sRessourcesPath);
}


void StateGame::init()
{
	sfgmk::CONSOLE.command("/freecam");
	sfgmk::CONSOLE.command("/physic");
	sfgmk::CONSOLE.command("/selector");
	sfgmk::CONSOLE.command("/life");
	sfgmk::CONSOLE.command("/ai");
	sfgmk::CONSOLE.command("/id");
	sfgmk::CONSOLE.command("/hud");

	//Démo IA (voir classe Goomba)
	Goomba* NewCleverGoomba = NULL;
	for( int i(0); i < 5; i++ )
	{
		NewCleverGoomba = new Goomba();
		ADD_ENTITY(NewCleverGoomba);
	}

	//HUD
	Sprite* HudSprite = new Sprite();
	HudSprite->setTexture(DATA_MANAGER->getTexture("hud"));
	HudSprite->setColor(sf::Color(255, 255, 255, 225));
	float fScale = (float)GRAPHIC_MANAGER->getRenderWindow()->getSize().x / (float)HudSprite->getTexture()->getSize().x;
	HudSprite->setScale(fScale, fScale);
	HudSprite->setPosition(0.0f, (float)GRAPHIC_MANAGER->getRenderWindow()->getSize().y - HudSprite->getSize().y);
	ADD_TO_HUD(HudSprite);

	//TILED
	TiledMap* m_map = new TiledMap;
	m_map->loadFromFile(std::string("../Data/states/Game/tiled/desert.tmx"));
	GRAPHIC_MANAGER->setMap(m_map);
}

void StateGame::update()
{
	//Déplacement de goomba si on en sélectionne
	if (INPUT_MANAGER->MOUSE_BUTTON(sf::Mouse::Right) == KEY_PRESSED)
	{
		sf::Vector2f mouse_pos = INPUT_MANAGER->MOUSE.getWorldPosition();

		void *data = malloc(8);
		*(float*)((unsigned int)data) = mouse_pos.x;
		*(float*)((unsigned int)data + 4) = mouse_pos.y;

		MESSAGE_MANAGER->SendMsgToGroup(1, 0, data, 8);
	}

	//Test floating damage (attention c fait a l'arrache, une fois que y'en a un qu'est mort les pointeurs sont foireux)
	if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::D) == KEY_PRESSED )
	{
		int iGoomba = RAND(1, 5);
		Goomba* Target = (Goomba*)ENTITY_MANAGER->getEntityVector()[iGoomba];
		if( Target != NULL )
		{
			int iDmg = RAND(1, 25);
			Target->removePv(iDmg);
			FloatingDamage* NewFloating = new FloatingDamage(iDmg, Target->getInitialPv(), Target);
			ADD_ENTITY(NewFloating);
		}
	}
}

void StateGame::deinit()
{
	GRAPHIC_MANAGER->removeMap();
}


void StateGame::draw()
{
}