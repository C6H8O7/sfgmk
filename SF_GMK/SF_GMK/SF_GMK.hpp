/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		03/01/2015
	@brief		Point d'entrée

--------------------------------------------------------------------------------------------------*/

#ifndef SF_GMK_HPP
#define SF_GMK_HPP

#ifdef SFGMK_API_DLL
	#ifdef SFGMK_EXPORT
		#define SFGMK_API __declspec(dllexport)
	#else
		#define SFGMK_API __declspec(dllimport)
	#endif
#else
	#define SFGMK_API
#endif

#define SFGMK_DEBUG

#define SFGML_ENABLE_DIR_DEBUG false

	//SFML (à laisser au début pour éviter conflits entre "headers windows" et implémentation de la SFML
	#include <SFML\Config.hpp>
	#include <SFML\Graphics.hpp>
	#include <SFML\System.hpp>
	#include <SFML\Window.hpp>

	//C
	#include <Windows.h>
	#include <assert.h>
	#include <psapi.h>
	#include <stdio.h>
	#include <tchar.h>
	#include <time.h>
	#include <limits.h>

	//C++
	#include <algorithm>
	#include <mutex>
	#include <thread>
	#include <cstdlib>
	#include <iostream>
	#include <fstream>
	#include <sstream>
	#include <functional>
	#include <map>
	#include <string>
	#include <vector>
	#include <random>
	#include <queue>
	#include <list>

	//Monitoring
	#pragma comment(lib, "Powrprof.lib")
	#include <Powrprof.h>

	//Gestion manette
	#include <XInput.h>
	#pragma comment(lib, "XInput9_1_0.lib")

	//Compression
	#include <zlib-1.2.3-lib\include\zlib.h>

	//Fmod
	#include <fmodex\fmod.hpp>

	//Include C parcours dossier
	#include "dirent/dirent.h"

	//tinyxml2
	#include "Tool\Parser\tinyxml2.h"

	//SF_GMK
	#include "Tool\DesignPattern\SingletonTemplate.hpp"
	#include "Tool\DynamicArray.hpp"
	#include "Tool\Warden.hpp"
	#include "Tool\FoncterTemplate.hpp"
	#include "Tool\Function.hpp"
	#include "Tool\GenericList.hpp"
	#include "Tool\ThreadTemplate.hpp"
	#include "Tool\Parser\Parser.hpp"

	#include "Graphic\ShapeDrawer.hpp"

	#include "Math\Math.hpp"
	#include "Math\Curb\Curb.hpp"
	#include "Math\Curb\Bezier.hpp"
	#include "Math\Curb\CatmullRom.hpp"
	#include "Math\Matrix\Matrix22.hpp"
	#include "Math\Matrix\MatrixRotation22.hpp"
	#include "Math\Curb\CoordinateSystem\GraphCurb.hpp"
	#include "Math\Curb\CoordinateSystem\Graph.hpp"

	#include "Physic\Collider.hpp"
	#include "Physic\SphereCollider.hpp"
	#include "Physic\ObbCollider.hpp"

	#include "Graphic\Animation.hpp"
	#include "Graphic\Sprite.hpp"
	#include "Graphic\FX\LifeBar\LifeBar.hpp"

	#include "Engine\Manager\MsgManager\Msg.hpp"
	#include "Engine\Manager\MsgManager\MsgActor.hpp"

	#include "Map\TiledMap.hpp"

	#include "Engine\Manager\AiManager\FSM.hpp"
	#include "Entity\Entity.hpp"
	#include "Entity\EntityWithPv.hpp"

	#include "Entity\Transformation\EntityTransformation.hpp"
	#include "Entity\Transformation\TransformationTranslation.hpp"
	#include "Entity\Transformation\TransformationRotation.hpp"
	#include "Entity\Transformation\TransformationBezier.hpp"
	#include "Entity\Transformation\TransformationInterpolation.hpp"

	#include "Graphic\FX\LightningBolt\LightningBolt.hpp"
	#include "Graphic\FX\Text\FxText.hpp"
	#include "Graphic\FX\Text\LightningText.hpp"
	#include "Graphic\FX\Grid\PointMass.hpp"
	#include "Graphic\FX\Grid\Spring.hpp"
	#include "Graphic\FX\Shader\Bloom.hpp"
	#include "Graphic\FX\Grid\Grid.hpp"
	#include "Graphic\FX\Grid\BlackHole.hpp"
	#include "Graphic\FX\LifeBar\FloatingDamage.hpp"

	#include "Graphic\FX\Shader\PostShader.hpp"
	#include "Graphic\FX\Shader\ShaderWave.hpp"

	#include "Engine\Manager\GraphicManager\Camera.hpp"
	#include "Engine\Manager\GraphicManager\Parallaxe.hpp"

	#include "Engine\Manager\InputManager\Joystick.hpp"
	#include "Engine\Manager\InputManager\Keyboard.hpp"
	#include "Engine\Manager\InputManager\Mouse.hpp"

	#include "Engine\Manager\DebugManager\Console.hpp"
	#include "Engine\Manager\DebugManager\Selector.hpp"

	#include "Engine\Manager\StateMachineManager\State.hpp"
	#include "Engine\Manager\StateMachineManager\LoadingState\StateLoading.hpp"
	#include "Engine\Manager\StateMachineManager\StateMachine.hpp"

	#include "Engine\Manager\AiManager\AIManager.hpp"
	#include "Engine\Manager\DataManager\DataManager.hpp"
	#include "Engine\Manager\PhysicManager\PhysicManager.hpp"
	#include "Engine\Manager\SoundManager\SoundManager.hpp"
	#include "Engine\Manager\StateMachineManager\StateMachineManager.hpp"
	#include "Engine\Manager\InputManager\InputManager.hpp"
	#include "Engine\Manager\EntityManager\EntityManager.hpp"
	#include "Engine\Manager\GraphicManager\GraphicManager.hpp"
	#include "Engine\Manager\DebugManager\DebugManager.hpp"
	#include "Engine\Manager\MsgManager\MsgManager.hpp"

	#include "Engine\Core\Core.hpp"
 
namespace sfgmk
{ 
	#define EMPTY_COLOR sf::Color(0, 0, 0, 0)

	#define DATA_PATH std::string("../data/sfgmk")

	//Core
	#define CORE							engine::Core::getSingleton()
	#define TIME_DELTA						CORE->getTimeDelta()

	//Managers
	#define AI_MANAGER						engine::AIManager::getSingleton()
	#define DATA_MANAGER					engine::DataManager::getSingleton()
	#define DEBUG_MANAGER					engine::DebugManager::getSingleton()
	#define ENTITY_MANAGER					engine::EntityManager::getSingleton()
	#define GRAPHIC_MANAGER					engine::GraphicManager::getSingleton()
	#define INPUT_MANAGER					engine::InputManager::getSingleton()
	#define PHYSIC_MANAGER					engine::PhysicManager::getSingleton()
	#define SOUND_MANAGER					engine::SoundManager::getSingleton()
	#define STATE_MACHINE_MANAGER			engine::StateMachineManager::getSingleton()
	#define MESSAGE_MANAGER					sfgmk::engine::MsgManager::GetInstance() // reste  - utiliser le singleton de SFGMK
	
	#define ADD_ENTITY(a)					sfgmk::engine::EntityManager::getSingleton()->addEntity(a)
	#define FREE_ENTITY_VECTOR				sfgmk::engine::EntityManager::getSingleton()->freeEntityVector()
	#define CAMERA							sfgmk::engine::GraphicManager::getSingleton()->getCurrentCamera()
	#define CONSOLE							DEBUG_MANAGER->getConsole()
	#define PARALLAXE						GRAPHIC_MANAGER->getParallaxe()
	#define ADD_TO_HUD(a)					GRAPHIC_MANAGER->addSpriteToHud(a)
	#define SHAPE_DRAWER					GRAPHIC_MANAGER->getShapeDrawer()

	#define SELECTOR						DEBUG_MANAGER->getSelector()

	//States
	#define CHANGE_STATE(a)					STATE_MACHINE_MANAGER->getStateMachine()->changeState(a);
	#define CHANGE_STATE_WITH_LOADING(a, b)	STATE_MACHINE_MANAGER->getStateMachine()->changeState(a, b);
	#define STATE_MACHINE					STATE_MACHINE_MANAGER->getStateMachine()
	#define CURRENT_STATE					STATE_MACHINE_MANAGER->getStateMachine()->Get_CurrentState()

	//INPUT
	#define KEY_UP							sfgmk::engine::eKeyStates::eKEY_UP
	#define KEY_PRESSED						sfgmk::engine::eKeyStates::eKEY_PRESSED
	#define KEY_DOWN						sfgmk::engine::eKeyStates::eKEY_DOWN
	#define KEY_RELEASED					sfgmk::engine::eKeyStates::eKEY_RELEASED

	//Keyboard
	#define KEYBOARD						getKeyboard()
	#define KEYBOARD_KEY(a)					KEYBOARD.getKeyState(a)

	//Mouse
	#define MOUSE							getMouse()
	#define MOUSE_BUTTON(a)					MOUSE.getButtonState(a)
	#define MOUSE_WINDOW_POS				INPUT_MANAGER->MOUSE.getWindowPosition()
	#define MOUSE_WORLD_POS					INPUT_MANAGER->MOUSE.getWorldPosition()

	//Joystick
	#define JOYSTICK(a)						INPUT_MANAGER->getJoystick(a)
	#define JOYSTICK_GET_BUTTON(a, b)		JOYSTICK(a).getButtonState(b)
	#define JOYSTICK_GET_AXIS(a, b)			JOYSTICK(a).getAxis(b)   

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