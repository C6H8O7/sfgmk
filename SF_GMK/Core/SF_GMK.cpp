// SF_GMK.cpp : définit le point d'entrée pour l'application console.
//

#include "../ProjetSample/IncludesProjet.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	//Cache la console / souris
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	//GAME_MANAGER->Get_RenderWindow()->setMouseCursorVisible(false);
	
	//Init random
	srand((unsigned int)time(NULL));

	//Init fenêtre V2
	SFGMK_CORE::getGraphicManager()->init(sf::VideoMode(1280, 720), "SF_GMK", sf::Style::Default);

	//Camera V2
	sfgmk::Camera* MyCamera = new sfgmk::Camera();
	MyCamera->setSize(sf::Vector2f(sfgmk::engine::Core::getGraphicManager()->getRenderWindow()->getSize()));
	MyCamera->setCenter(MyCamera->getSize() * 0.5f);
	SFGMK_CORE::getGraphicManager()->registerCamera("MainCamera", MyCamera);
	SFGMK_CORE::getGraphicManager()->setCurrentCamera("MainCamera");

	//Init states V2
	SFGMK_CORE::getStateMachineManager()->RegisterState<StateDefault>(sfgmk::eSFGMK_STATES::eStateDefault, "data/sfgmk/StateDefault");
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