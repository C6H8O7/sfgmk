#include "stdafx.h"

#include "IncludesProjet.hpp"

#include <omp.h>
#include <conio.h>
#include <iostream>

using namespace sfgmk;

std::string huehuehue(std::string& _str)
{
	int i = 0;

	int a, b;
	char osef[20];

	sscanf_s(_str.c_str(), "%s %d %d", osef, 20, &a, &b);

	i = a + b;

	return std::string("CA FAIT ") + std::to_string(i) + " BATARD !\n";
}

StateDefault::StateDefault()
{
	sfgmk::DATA_MANAGER->loadLevel(m_sRessourcesPath);
	//sfgmk::PARALLAXE.loadLevel(m_sRessourcesPath);
}

StateDefault::~StateDefault()
{
	sfgmk::DATA_MANAGER->unloadLevel(m_sRessourcesPath);
	sfgmk::PARALLAXE.unloadLevel(m_sRessourcesPath);
}

void StateDefault::init()
{
	sfgmk::GRAPHIC_MANAGER->getCurrentCamera()->setFreeMove();
	sfgmk::PHYSIC_MANAGER->setDraw(true);

	sfgmk::engine::ConsoleDev::getSingleton()->registerCommand("/somme", huehuehue);

	/*for( int i(0); i < 100; i++ )
	{
		sfgmk::Entity* NewEntity = new sfgmk::Entity(sf::Vector3f(RAND(100, 1180), RAND(100, 620), 0.0f));
		NewEntity->getSprite()->setTexture(sfgmk::DATA_MANAGER->getTexture("goomba1"));
		//int iOrigine = RAND(0, 10);
		//NewEntity->setOrigin(0.1f * iOrigine, 0.1f * iOrigine);*/
		/*int iScale = RAND(1, 4);
		NewEntity->setScale(0.25f * iScale, 0.25f * iScale);*/
		/*if( RAND(0, 1) == 0 )
			NewEntity->addObbCollider();
		else
			NewEntity->addSphereCollider();
		ADD_ENTITY(NewEntity);*/
	//}
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