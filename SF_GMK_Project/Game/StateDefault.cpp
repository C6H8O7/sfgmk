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
	sfgmk::PARALLAXE.loadLevel(m_sRessourcesPath);
}

StateDefault::~StateDefault()
{
	sfgmk::DATA_MANAGER->unloadLevel(m_sRessourcesPath);
	sfgmk::PARALLAXE.unloadLevel(m_sRessourcesPath);
}

void StateDefault::init()
{
	sfgmk::GRAPHIC_MANAGER->getCurrentCamera()->setFreeMove();

	sfgmk::engine::ConsoleDev::getSingleton()->registerCommand("/somme", huehuehue);
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