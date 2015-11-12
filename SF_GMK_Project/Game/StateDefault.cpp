#include "stdafx.h"

#include "IncludesProjet.hpp"

#include <omp.h>
#include <conio.h>
#include <iostream>

using namespace sfgmk;

StateDefault::StateDefault()
{
}

StateDefault::~StateDefault()
{
	ENTITY_MANAGER->freeEntityVector();
	DATA_MANAGER->unloadLevel(m_sRessourcesPath);
	PARALLAXE->unloadLevel(m_sRessourcesPath);
	SOUND_MANAGER->unloadLevel(m_sRessourcesPath);
}


void StateDefault::init()
{
	sfgmk::CONSOLE.command("/freecam");
	sfgmk::CONSOLE.command("/physic");
	sfgmk::CONSOLE.command("/selector");
	sfgmk::CONSOLE.command("/life");
	sfgmk::CONSOLE.command("/ai");
	sfgmk::CONSOLE.command("/id");

	for( int i(0); i < 25; i++ )
	{
		int iInitialPv = RAND(1, 100);
		EntityWithPv* entity = new EntityWithPv(sf::Vector3f((float)RAND(100, 1180), (float)RAND(100, 620), 0.0f), iInitialPv, RAND(1, iInitialPv));
		entity->getSprite()->setAnimation(DATA_MANAGER->getAnimation("goomba"));
		entity->addLifeBar(true);

		if( RAND(0, 1) == 0 )
			entity->addObbCollider();
		else
			entity->addSphereCollider();

		ADD_ENTITY(entity);
	}

	//Démo IA (voir classe Goomba)
	Goomba* NewCleverGoomba = new Goomba();
	ADD_ENTITY(NewCleverGoomba);
}

void StateDefault::update()
{
	//Déplacement du goomba si on le sélectionne
	if( INPUT_MANAGER->MOUSE_BUTTON(sf::Mouse::Right) == KEY_PRESSED )
	{
		DynamicArray<Entity*>* SelectedEntities = SELECTOR->getSelectedEntities();
		for( unsigned int i(0); i < SelectedEntities->getElementNumber(); i++ )
		{
			if( (*SelectedEntities)[i]->getAiFsm() )
				((Goomba*)((*SelectedEntities)[i]))->setDestination(INPUT_MANAGER->MOUSE.getWorldPosition());
		}
	}
}

void StateDefault::deinit()
{
}


void StateDefault::draw()
{
}