// SF_GMK.cpp : définit le point d'entrée pour l'application console.
//


int _tmain(int argc, _TCHAR* argv[])
{
	//Cache la console / souris
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	//GAME_MANAGER->Get_RenderWindow()->setMouseCursorVisible(false);
	
	//Init random
	srand((unsigned int)time(NULL));
	
	//Init fenêtre
	GAME_MANAGER->init(sf::VideoMode::getDesktopMode(), "SF_GMK", sf::Style::Default);
	//GAME_MANAGER->getRenderWindow()->setVerticalSyncEnabled(true);
	//GAME_MANAGER->getRenderWindow()->setFramerateLimit(120); //Ne PAS utiliser vSync et frameRateLimit simultanément

	//Camera
	sfgmk::Camera* MyCamera = new sfgmk::Camera();
	MyCamera->setSize(sf::Vector2f(GAME_MANAGER->getRenderWindow()->getSize()));
	MyCamera->setCenter(MyCamera->getSize() * 0.5f);
	GAME_MANAGER->registerCamera("MainCamera", MyCamera);
	GAME_MANAGER->setCurrentCamera("MainCamera");

	//Enregistrement des états du jeu
	STATE_BANK->RegisterState<sfgmk::StateLoading>(sfgmk::eSFGMK_STATES::eLoadingState, "data/sfgmk/stateLoading");

	STATE_BANK->RegisterState<StateDefault>(sfgmk::eSFGMK_STATES::eStateDefault, "data/sfgmk/StateDefault");

	//Etat de départ
	GAME_MANAGER->getStateMachine()->init(sfgmk::eSFGMK_STATES::eStateDefault);
	
	//Loop
	while( GAME_MANAGER->getRenderWindow()->isOpen() )
	{
		GAME_MANAGER->preLoop();
		GAME_MANAGER->loop();
		GAME_MANAGER->postLoop();
	}

	sfgmk::StateBank::releaseSingleton();
	sfgmk::DataManager::releaseSingleton();
	sfgmk::PhysicManager::releaseSingleton();
	sfgmk::GameManager::releaseSingleton();
	sfgmk::ConsoleDev::releaseSingleton();
	sfgmk::SoundManager::releaseSingleton();
	sfgmk::InputManager::releaseSingleton();

	return 0;
}