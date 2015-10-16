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
	#include "Core\Tool\Function.hpp"
	#include "Core\Tool\GenericList.hpp"
	#include "Core\Tool\DynamicArray.hpp"
	#include "Core\Tool\ThreadTemplate.hpp"
	#include "Core\SF_GMK.hpp"

	#include "Core\Math\Math.hpp"
	#include "Core\Math\Curb\Curb.hpp"
	#include "Core\Math\Curb\Bezier.hpp"
	#include "Core\Math\Curb\CatmullRom.hpp"
	#include "Core\Math\Matrix\Matrix22.hpp"
	#include "Core\Math\Matrix\MatrixRotation22.hpp"
	#include "Core\Math\Curb\CoordinateSystem\GraphCurb.hpp"
	#include "Core\Math\Curb\CoordinateSystem\Graph.hpp"

	#include "Core\Manager\InputManager\Joystick.hpp"
	#include "Core\Manager\InputManager\Keyboard.hpp"
	#include "Core\Manager\InputManager\Mouse.hpp"

	#include "Core\Tool\Parser\Parser.hpp"

	#include "Core\Manager\PhysicManager\Collider.hpp"
	#include "Core\Manager\PhysicManager\SphereCollider.hpp"
	#include "Core\Manager\PhysicManager\ObbCollider.hpp"

	#include "Core\Manager\GraphicManager\Animation.hpp"
	#include "Core\Manager\GraphicManager\Sprite.hpp"
	#include "Core\Manager\EntityManager\Entity.hpp"
	#include "Core\Manager\EntityManager\EntityWithPv.hpp"
	#include "Core\Manager\EntityManager\EntityTransformation.hpp"
	#include "Core\Manager\EntityManager\TransformationTranslation.hpp"
	#include "Core\Manager\EntityManager\TransformationRotation.hpp"
	#include "Core\Manager\EntityManager\TransformationBezier.hpp"
	#include "Core\Manager\EntityManager\TransformationInterpolation.hpp"

	#include "Core\Manager\GraphicManager\FX\LightningBolt\LightningBolt.hpp"
	#include "Core\Manager\GraphicManager\FX\Text\FxText.hpp"
	#include "Core\Manager\GraphicManager\FX\Text\LightningText.hpp"
	#include "Core\Manager\GraphicManager\FX\Grid\PointMass.hpp"
	#include "Core\Manager\GraphicManager\FX\Grid\Spring.hpp"
	#include "Core\Manager\GraphicManager\FX\Shader\Bloom.hpp"
	#include "Core\Manager\GraphicManager\FX\Grid\Grid.hpp"
	#include "Core\Manager\GraphicManager\FX\Grid\BlackHole.hpp"

	#include "Core\Manager\GraphicManager\FX\Shader\PostShader.hpp"
	#include "Core\Manager\GraphicManager\FX\Shader\ShaderWave.hpp"

	#include "Core\Manager\GraphicManager\Camera.hpp"
	#include "Core\Manager\GraphicManager\Parallaxe.hpp"

	#include "Core\Tool\DesignPattern\SingletonTemplate.hpp"
	#include "Core\Manager\DataManager\DataManager.hpp"
	#include "Core\Manager\PhysicManager\PhysicManager.hpp"
	#include "Core\Manager\SoundManager\SoundManager.hpp"
	#include "Core\Manager\DebugManager\Console.hpp"
	#include "Core\Manager\StateMachineManager\State.hpp"
	#include "Core\Manager\StateMachineManager\LoadingState\StateLoading.hpp"
	#include "Core\Manager\StateMachineManager\StateMachine.hpp"
	#include "Core\Manager\GameManager.hpp"
	#include "Core\Manager\InputManager\InputManager.hpp"

	//Projet
	#include "ProjetSample\IncludesProjet.hpp"