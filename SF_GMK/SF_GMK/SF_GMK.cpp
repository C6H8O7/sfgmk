// SF_GMK.cpp : définit le point d'entrée pour l'application console.
//

#include "../ProjetSample/IncludesProjet.hpp"

int _tmain(int argc, char* argv[])
{
	//Cache la console / souris
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	//GAME_MANAGER->Get_RenderWindow()->setMouseCursorVisible(false);
	
	//Init random
	srand((unsigned int)time(NULL));

	//Init fenêtre V2
	sfgmk::GRAPHIC_MANAGER->init(sf::VideoMode(1280, 720), "SF_GMK", sf::Style::Default);

	//Init states V2
	sfgmk::STATE_MACHINE_MANAGER->RegisterState<StateDefault>(sfgmk::eSTATES::eStateDefault, "../data/states/stateDefault");
	sfgmk::STATE_MACHINE->init(sfgmk::eSTATES::eStateDefault);

	while( sfgmk::GRAPHIC_MANAGER->getRenderWindow()->isOpen() )
	{
		sfgmk::CORE->preLoop();
		sfgmk::CORE->loop();
		sfgmk::CORE->postLoop();
	}

	// TODO : release des singletons

	return 0;
}