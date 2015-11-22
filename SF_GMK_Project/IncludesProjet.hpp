#ifndef INCLUDES_PROJET_HPP
#define INCLUDES_PROJET_HPP


enum eSTATES
{
	eStateLoading = 0,
	eStateMenu,
	eStateGame,
	eStateGamePathfinding,
	eSTATES_NUMBER
};

#include "Game\Goomba.hpp"

#include "Game\States\Menu.hpp"
#include "Game\States\Game.hpp"
#include "Game\States\GamePathfinding.hpp"


#endif