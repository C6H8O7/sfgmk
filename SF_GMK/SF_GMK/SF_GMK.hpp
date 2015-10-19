/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		03/01/2015
	@brief		Point d'entrée

--------------------------------------------------------------------------------------------------*/

#ifndef SF_GMK_HPP
#define SF_GMK_HPP

 
namespace sfgmk
{ 
	enum eSTATES
	{
		eLoadingState = 0,
		eStateDefault = 1,
		eSTATES_NUMBER
	};

	#define EMPTY_COLOR sf::Color(0, 0, 0, 0)

	#define DATA_PATH std::string("../data/sfgmk/")

	//Core
	#define CORE						engine::Core::getSingleton()

	//Managers
	#define DATA_MANAGER				engine::DataManager::getSingleton()
	//debugmanager
	#define ENTITY_MANAGER				engine::EntityManager::getSingleton()
	#define GRAPHIC_MANAGER				engine::GraphicManager::getSingleton()
	#define INPUT_MANAGER				engine::InputManager::getSingleton()
	#define PHYSIC_MANAGER				engine::PhysicManager::getSingleton()
	#define SOUND_MANAGER				engine::SoundManager::getSingleton()
	#define STATE_MACHINE_MANAGER		engine::StateMachineManager::getSingleton()
	
	#define ADD_ENTITY(a)				sfgmk::engine::EntityManager::getSingleton()->addEntity(a)
	#define FREE_ENTITY_VECTOR			sfgmk::engine::EntityManager::getSingleton()->freeEntityVector()
	#define CAMERA						sfgmk::engine::GraphicManager::getSingleton()->getCurrentCamera()
	#define CONSOLE						sfgmk::engine::ConsoleDev::getSingleton();
	#define PARALLAXE					GRAPHIC_MANAGER->getParallaxe()
	#define TIME_DELTA					CORE->getTimeDelta()

	//States
	#define CHANGE_STATE(a)				STATE_MACHINE_MANAGER->getStateMachine()->changeState(a);
	#define STATE_MACHINE				STATE_MACHINE_MANAGER->getStateMachine()
	#define CURRENT_STATE				STATE_MACHINE_MANAGER->getStateMachine()->Get_CurrentState()

	//Inputs
	
	//Keyboard
	#define KEYBOARD					INPUT_MANAGER->getKeyboard()
	#define KEYBOARD_KEY(a)				KEYBOARD.getKeyState(a)

	//Mouse
	#define MOUSE						INPUT_MANAGER->getMouse()
	#define MOUSE_WINDOW_POS			MOUSE.getWindowPosition()
	#define MOUSE_WORLD_POS				MOUSE.getWorldPosition()

	//Joystick
	#define JOYSTICK(a)					INPUT_MANAGER->getJoystick(a)
	#define JOYSTICK_GET_BUTTON(a, b)	JOYSTICK(a).getButtonState(b)
	#define JOYSTICK_GET_AXIS(a, b)		JOYSTICK(a).getAxis(b)   

	//Manette XBOX-360
	#define BUTTON_A 0
	#define BUTTON_B 1
	#define BUTTON_X 2
	#define BUTTON_Y 3
	#define BUTTON_LB 4
	#define BUTTON_RB 5
	#define BUTTON_BACK 6
	#define BUTTON_START 7
	#define BUTTON_JOYSTICK_LEFT 8
	#define BUTTON_JOYSTICK_RIGHT 9
	//touche 'XBOX'								==> pas récupérable apparemment
	//appuie sur le centre du pad directionnel	==> //

	#define JOYSTICK_LEFT_HORIZONTAL sf::Joystick::X //-100 à gauche, 100 à droite
	#define JOYSTICK_LEFT_VERTICAL sf::Joystick::Y //-100 en haut, 100 en bas
	#define TRIGGER_RIGHT sf::Joystick::Z //0 -> -99
	#define TRIGGER_LEFT sf::Joystick::Z //0 -> 99
	#define JOYSTICK_RIGHT_VERTICAL sf::Joystick::R //-100 en haut, 100 en bas
	#define JOYSTICK_RIGHT_HORIZONTAL sf::Joystick::U //-100 à gauche, 100 à droite
	#define DIRECTIONAL_CROSS_VERTICAL sf::Joystick::PovX //100 en haut, -100 en bas
	#define DIRECTIONAL_CROSS_HORIZONTAL sf::Joystick::PovY //-100 à gauche, 100 à droite
}


#endif