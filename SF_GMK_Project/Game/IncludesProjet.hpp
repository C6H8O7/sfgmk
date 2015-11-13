#ifndef INCLUDES_PROJET_HPP
#define INCLUDES_PROJET_HPP


enum eSTATES
{
	eStateLoading = 0,
	eStateMenu,
	eStateGame,
	eSTATES_NUMBER
};

#include "Goomba.hpp"

#include "States\Menu.hpp"
#include "States\Game.hpp"


#endif