#include "stdafx.h"
#include "../IncludesProjet.hpp"

using namespace sfgmk;


StateGame::StateGame()
{
}

StateGame::~StateGame()
{
}


void StateGame::init()
{
	sfgmk::CONSOLE.command("/freecam");
	//sfgmk::CONSOLE.command("/physic");
	sfgmk::CONSOLE.command("/selector");
	sfgmk::CONSOLE.command("/life");
	sfgmk::CONSOLE.command("/ai");
	//sfgmk::CONSOLE.command("/id");
	sfgmk::CONSOLE.command("/hud");

	//Démo IA (voir classe Goomba)
	Goomba* NewCleverGoomba = NULL;
	for( int i(0); i < 33; i++ )
	{
		NewCleverGoomba = new Goomba();
		//NewCleverGoomba->setRelativOrigin(0.5f, 0.5f);
		int iscale = RAND(1, 4);
		NewCleverGoomba->setScale(iscale * 0.5f, iscale * 0.5f);
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

	FoncterTemplateInstance<StateGame, void>* HudDrawFunction = new FoncterTemplateInstance<StateGame, void>(this, &StateGame::drawHud);
	GRAPHIC_MANAGER->setHudDrawFunction(HudDrawFunction);
	
	//TILED
	TiledMap* m_map = new TiledMap;
	m_map->loadFromFile(std::string("../Data/states/Game/tiled/desert.tmx"));
	GRAPHIC_MANAGER->setMap(m_map);
}

void StateGame::update()
{
	// test pour sturer la pompe a message lul
	/*if (INPUT_MANAGER->MOUSE_BUTTON(sf::Mouse::Right) == KEY_PRESSED)
	{
		for (int i = 0; i < 100;++i) {


			int* valeurBidon = new int;
			*valeurBidon = 4;
			MESSAGE_MANAGER->SendMsg(1, 1, valeurBidon, sizeof(int), MSG_TIME_TO_LIVE + 1, engine::MSG_TAG::TAG_UNKNOWN);
		}
	
	if (INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::W) == KEY_PRESSED)
	{
		sfgmk::CONSOLE.command("/warden");
	}*/

	//Déplacement de goomba si on en sélectionne
	if (INPUT_MANAGER->MOUSE_BUTTON(sf::Mouse::Right) == KEY_PRESSED)
	{
		sf::Vector2f mouse_pos = INPUT_MANAGER->MOUSE.getWorldPosition();

		void *data = malloc(8);
		*(float*)((unsigned int)data) = mouse_pos.x;
		*(float*)((unsigned int)data + 4) = mouse_pos.y;

		MESSAGE_MANAGER->SendMsgToGroup(1, 0, data, 8, 0.0f, engine::MSG_TAG::TAG_FLOAT);
	}

	//Test floating damage (attention c fait a l'arrache, une fois que y'en a un qu'est mort les pointeurs sont foireux)
	if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::F7) == KEY_PRESSED )
	{
		int iGoomba = RAND(1, 5);
		Goomba* Target = (Goomba*)ENTITY_MANAGER->getEntityVector()[iGoomba];
		if( Target != NULL )
		{
			int iDmg = RAND(1, 25);
			Target->removePv(iDmg, true);
		}
	}

	//Reload niveau
	if (INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::F5) == KEY_PRESSED)
		CHANGE_STATE_WITH_LOADING(eStateLoading, eStateGame);
}

void StateGame::deinit()
{
	sfgmk::CONSOLE.command("/freecam");
	sfgmk::CONSOLE.command("/physic");
	sfgmk::CONSOLE.command("/selector");
	sfgmk::CONSOLE.command("/life");
	sfgmk::CONSOLE.command("/ai");
	sfgmk::CONSOLE.command("/id");
	sfgmk::CONSOLE.command("/hud");

	GRAPHIC_MANAGER->removeMap();
	GRAPHIC_MANAGER->cleanHud();

	ENTITY_MANAGER->freeEntityVector();
	DATA_MANAGER->unloadLevel(m_sRessourcesPath);
	PARALLAXE->unloadLevel(m_sRessourcesPath);
	SOUND_MANAGER->unloadLevel(m_sRessourcesPath);
}


void StateGame::draw()
{
}


void StateGame::drawHud()
{
	if( DEBUG_MANAGER->getSelector() )
	{
		sf::RenderTexture* Render = GRAPHIC_MANAGER->getRenderTexture();
		DynamicArray<Entity*>* SelectedEntities = DEBUG_MANAGER->getSelector()->getSelectedEntities();
		Sprite* TempSprite = new Sprite;
		sf::Vector2f Size;
		sf::Vector2f Scale;
		sf::Vector2f Position;

		float fDesiredSize = 32.0f;
		float fMarge = 8.0f;
		sf::Vector2f HudPosition(260.0f, 592.5f);
		sf::Vector2u MaxEnt(16U, 3U);

		sf::RectangleShape Rect;
		Rect.setSize(sf::Vector2f(fDesiredSize, fDesiredSize));
		Rect.setOutlineThickness(2.0f);
		Rect.setOutlineColor(sf::Color(0, 77, 0, 255));
		Rect.setFillColor(sf::Color(100, 200, 100, 50));

		Render->setView(Render->getDefaultView());

		sf::Vector2u PlaceAccount(0U, 0U);

		for( unsigned int i(0U); i < SelectedEntities->getElementNumber(); i++ )
		{
			PlaceAccount.y = i / MaxEnt.x;
			PlaceAccount.x = i - PlaceAccount.y * MaxEnt.x;
			
			memcpy(TempSprite, (*SelectedEntities)[i]->getSprite(), sizeof(Sprite));

			if( TempSprite->getIsAnimation() )
				TempSprite->setAnimation(*(*SelectedEntities)[i]->getSprite()->getAnimation(), false);

			Size = TempSprite->getSize();
			Scale = TempSprite->getScale();
			TempSprite->setScale(Scale.x * (fDesiredSize / Size.x), Scale.y * (fDesiredSize / Size.y));
			
			Position = HudPosition + sf::Vector2f(PlaceAccount.x * fDesiredSize + PlaceAccount.x * fMarge, PlaceAccount.y * (fDesiredSize + fMarge));

			Rect.setPosition(Position);
			Render->draw(Rect);

			TempSprite->setPosition(Position);
			TempSprite->setColor(sf::Color(150, 255, 150, 255));
			Render->draw(*TempSprite);

			if( TempSprite->getIsAnimation() )
				TempSprite->deleteAnimation();
		}
		delete TempSprite;

		Render->setView(*CAMERA);
	}
}