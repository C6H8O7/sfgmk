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
	SFGMK_CORE::getGraphicManager()->init(sf::VideoMode(1280, 720), "SF_GMK", sf::Style::Default);

	//Init states V2
	SFGMK_CORE::getStateMachineManager()->RegisterState<StateDefault>(sfgmk::eSFGMK_STATES::eStateDefault, SFGMK_DATA_PATH + "stateDefault");
	SFGMK_CORE::getStateMachineManager()->getStateMachine()->init(sfgmk::eSFGMK_STATES::eStateDefault);

	while (sfgmk::engine::Core::getGraphicManager()->getRenderWindow()->isOpen())
	{
		SFGMK_CORE::getSingleton()->preLoop();
		SFGMK_CORE::getSingleton()->loop();
		SFGMK_CORE::getSingleton()->postLoop();
	}

	// TODO : release des singletons

	return 0;
}