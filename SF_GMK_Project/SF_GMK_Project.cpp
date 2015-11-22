// SF_GMK_Project.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "SF_GMK.hpp"

#include "IncludesProjet.hpp"

int main()
{
	//Cache la console / souris
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	//GAME_MANAGER->Get_RenderWindow()->setMouseCursorVisible(false);

	//Init random
	srand((unsigned int)time(NULL));

	/*DeleteFileA("../Data/sfgmk/texture/Thumbs.db");
	DeleteFileA("../Data/states/stateDefault/texture/Thumbs.db");
	DeleteFileA("../Data/states/stateDefault/layer/Thumbs.db");*/

	//Init fenêtre V2
	sfgmk::GRAPHIC_MANAGER->init(sf::VideoMode(1280, 720), "SF_GMK", sf::Style::Default);

	//Init managers
	sfgmk::CORE->initManagers();

	//Init states V2
	sfgmk::STATE_MACHINE_MANAGER->RegisterState<sfgmk::engine::StateLoading>(eSTATES::eStateLoading, DATA_PATH + "/stateLoading");
	sfgmk::STATE_MACHINE_MANAGER->RegisterState<StateMenu>(eSTATES::eStateMenu, "../data/states/Menu");
	sfgmk::STATE_MACHINE_MANAGER->RegisterState<StateGame>(eSTATES::eStateGame, "../data/states/Game");
	sfgmk::STATE_MACHINE_MANAGER->RegisterState<StateGamePathfinding>(eSTATES::eStateGamePathfinding, "../data/states/GamePathfinding");
	sfgmk::STATE_MACHINE->init(eSTATES::eStateGamePathfinding);

	//Boucle de jeu
	while( sfgmk::GRAPHIC_MANAGER->getRenderWindow()->isOpen() )
	{
		sfgmk::CORE->preLoop();
		sfgmk::CORE->loop();
		sfgmk::CORE->postLoop();
	}
	
	//Release des singletons
	sfgmk::CORE->releaseManagers();
	sfgmk::WardenReg::getSingleton()->releaseSingleton();
	sfgmk::CORE->releaseSingleton();
	
	return 0;
}