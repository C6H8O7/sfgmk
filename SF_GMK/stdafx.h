// stdafx.h : fichier Include pour les fichiers Include système standard,
//				ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
//				et sont rarement modifiés
//


#pragma once

	#define _CRT_SECURE_NO_WARNINGS

	//SFML (à laisser au début pour éviter conflits entre "headers windows" et implémentation de la SFML
	#include <SFML\Config.hpp>
	#include <SFML\Graphics.hpp>
	#include <SFML\System.hpp>
	#include <SFML\Window.hpp>

	#include "targetver.h"

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
	#include <functional>
	#include <map>
	#include <string>
	#include <vector>
	#include <random>

#pragma comment(lib, "Powrprof.lib")
	#include <Powrprof.h>

	//Gestion manette
	#include <XInput.h>
		#pragma comment(lib, "XInput9_1_0.lib")

	// Compression
	#include <zlib-1.2.3-lib\include\zlib.h>

	//Fmod
	#include <fmodex\fmod.hpp>

	//Include C parcours dossier
	#include "dirent/dirent.h"

	// TODO: faites référence ici aux en-têtes supplémentaires nécessaires au programme

	//SF_GMK
	#include "Tool\Function.hpp"
	#include "Tool\GenericList.hpp"
	#include "Tool\DynamicArray.hpp"
	#include "Tool\ThreadTemplate.hpp"
	#include "SF_GMK.hpp"

	#include "Math\Math.hpp"
	#include "Math\Curb\Curb.hpp"
	#include "Math\Curb\Bezier.hpp"
	#include "Math\Curb\CatmullRom.hpp"
	#include "Math\Matrix\Matrix22.hpp"
	#include "Math\Matrix\MatrixRotation22.hpp"
	#include "Math\Curb\CoordinateSystem\GraphCurb.hpp"
	#include "Math\Curb\CoordinateSystem\Graph.hpp"

	#include "Engine\Manager\InputManager\Joystick.hpp"
	#include "Engine\Manager\InputManager\Keyboard.hpp"
	#include "Engine\Manager\InputManager\Mouse.hpp"

	#include "Tool\Parser\Parser.hpp"

	#include "Physic\Collider.hpp"
	#include "Physic\SphereCollider.hpp"
	#include "Physic\ObbCollider.hpp"

	#include "Graphic\Animation.hpp"
	#include "Graphic\Sprite.hpp"
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

	#include "Graphic\FX\Shader\PostShader.hpp"
	#include "Graphic\FX\Shader\ShaderWave.hpp"

	#include "Engine\Manager\GraphicManager\Camera.hpp"
	#include "Engine\Manager\GraphicManager\Parallaxe.hpp"

	#include "Tool\DesignPattern\SingletonTemplate.hpp"
	#include "Engine\Manager\DataManager\DataManager.hpp"
	#include "Engine\Manager\PhysicManager\PhysicManager.hpp"
	#include "Engine\Manager\SoundManager\SoundManager.hpp"
	#include "Engine\Manager\DebugManager\Console.hpp"

	#include "Engine\Manager\StateMachineManager\State.hpp"
	#include "Engine\Manager\StateMachineManager\LoadingState\StateLoading.hpp"
	#include "Engine\Manager\StateMachineManager\StateMachine.hpp"
	#include "Engine\Manager\StateMachineManager\StateMachineManager.hpp"

	#include "Engine\Manager\InputManager\InputManager.hpp"


#include "Engine\Manager\EntityManager\EntityManager.hpp"
#include "Engine\Manager\GraphicManager\GraphicManager.hpp"
#include "Engine\Core\Core.hpp"