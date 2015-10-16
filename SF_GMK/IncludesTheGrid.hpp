#ifndef INCLUDES_THE_GRID_HPP
#define INCLUDES_THE_GRID_HPP


enum eSTATES
{
	eIntroState = 1,
	eMenuState,
	eDraftState,
	eGameState,
	eScoreState,
	eOptionState,
	eCreditState,
	eSTATES_NUMBER
};


#include "Entities.hpp"

#include "Shot.hpp"
#include "Laser.hpp"

#include "Player.hpp"
#include "ShipType.hpp"
#include "ShipTypeI.hpp"

#include "Intro.hpp"
#include "Menu.hpp"
#include "Draft.hpp"
#include "Game.hpp"
#include "Score.hpp"
#include "Option.hpp"
#include "Credit.hpp"


#endif