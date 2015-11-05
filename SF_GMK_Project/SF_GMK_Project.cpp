// SF_GMK_Project.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"

#include "SF_GMK.hpp"

#include "Game\IncludesProjet.hpp"

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

	//Init states V2
	sfgmk::STATE_MACHINE_MANAGER->RegisterState<sfgmk::engine::StateLoading>(sfgmk::eSTATES::eStateLoading, DATA_PATH + "/stateLoading");
	sfgmk::STATE_MACHINE_MANAGER->RegisterState<StateDefault>(sfgmk::eSTATES::eStateDefault, "../data/states/stateDefault");
	sfgmk::STATE_MACHINE->init(sfgmk::eSTATES::eStateLoading, sfgmk::eSTATES::eStateDefault);

	while( sfgmk::GRAPHIC_MANAGER->getRenderWindow()->isOpen() )
	{
		sfgmk::CORE->preLoop();
		sfgmk::CORE->loop();
		sfgmk::CORE->postLoop();
	}

	//Release des singletons
	sfgmk::DATA_MANAGER->releaseSingleton();
	sfgmk::DEBUG_MANAGER->releaseSingleton();
	sfgmk::ENTITY_MANAGER->releaseSingleton();
	sfgmk::GRAPHIC_MANAGER->releaseSingleton();
	sfgmk::INPUT_MANAGER->releaseSingleton();
	sfgmk::PHYSIC_MANAGER->releaseSingleton();
	sfgmk::SOUND_MANAGER->releaseSingleton();
	sfgmk::STATE_MACHINE_MANAGER->releaseSingleton();

	sfgmk::CORE->releaseSingleton();
	
	return 0;
}