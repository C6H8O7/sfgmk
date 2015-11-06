#include "stdafx.h"

#include "IncludesProjet.hpp"

#include <omp.h>
#include <conio.h>
#include <iostream>
#include <bitset>

using namespace sfgmk;

StateDefault::StateDefault()
{
}

StateDefault::~StateDefault()
{
	ENTITY_MANAGER->freeEntityVector();
	DATA_MANAGER->unloadLevel(m_sRessourcesPath);
	PARALLAXE.unloadLevel(m_sRessourcesPath);
	SOUND_MANAGER->unloadLevel(m_sRessourcesPath);
}


/*void GoombaIdle()
{
	std::cout << "IDLE" << std::endl;
}

void GoombaMove()
{
	std::cout << "MOVE" << std::endl;
}*/

void StateDefault::init()
{
	sfgmk::CONSOLE.command("/freecam");
	sfgmk::CONSOLE.command("/physic");
	sfgmk::CONSOLE.command("/selector");

	for( int i(0); i < 100; i++ )
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

	//Démo IA
	enum eGoombaAiState
	{
		eIdle = 0,
		eMove,
		eGoombaAiState_NUMBER
	};

	/*sfgmk::FoncterTemplateInstance<CLASS_NULL, void>* PtrIdle = new sfgmk::FoncterTemplateInstance<CLASS_NULL, void>(&GoombaIdle);
	sfgmk::FoncterTemplateInstance<CLASS_NULL, void>* PtrMove = new sfgmk::FoncterTemplateInstance<CLASS_NULL, void>(&GoombaMove);

	Entity* NewEntity = new Entity(sf::Vector3f(250.0f, 250.0f, 0.0f));
	NewEntity->getSprite()->setAnimation(DATA_MANAGER->getAnimation("goomba"));

	NewEntity->addAiFsm(eIdle);
	NewEntity->addAiState(eIdle, PtrIdle);
	NewEntity->addAiState(eMove, PtrMove);
	
	ADD_ENTITY(NewEntity);*/
}

void StateDefault::update()
{
}

void StateDefault::deinit()
{
}


void StateDefault::draw()
{
}