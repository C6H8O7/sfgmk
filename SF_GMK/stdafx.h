// stdafx.h�: fichier Include pour les fichiers Include syst�me standard,
//				ou les fichiers Include sp�cifiques aux projets qui sont utilis�s fr�quemment,
//				et sont rarement modifi�s
//


#pragma once

	#define _CRT_SECURE_NO_WARNINGS

	//SFML (� laisser au d�but pour �viter conflits entre "headers windows" et impl�mentation de la SFML
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
	#include <iostream>
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

	// TODO: faites r�f�rence ici aux en-t�tes suppl�mentaires n�cessaires au programme

	//SF_GMK
	#include "Function.hpp"
	#include "Cfunctions.hpp"
	#include "GenericList.hpp"
	#include "DynamicArray.hpp"
	#include "ThreadTemplate.hpp"
	#include "SF_GMK.hpp"

	#include "Math.hpp"
	#include "Curb.hpp"
	#include "Bezier.hpp"
	#include "CatmullRom.hpp"
	#include "Matrix22.hpp"
	#include "MatrixRotation22.hpp"
	#include "GraphCurb.hpp"
	#include "Graph.hpp"

	#include "Joystick.hpp"
	#include "Keyboard.hpp"
	#include "Mouse.hpp"

	#include "Parser.hpp"

	#include "Collider.hpp"
	#include "SphereCollider.hpp"
	#include "ObbCollider.hpp"

	#include "Animation.hpp"
	#include "SpriteComponent.hpp"
	#include "DeadTimer.hpp"
	#include "Circle.hpp"
	#include "Rotation.hpp"
	#include "Translation.hpp"
	#include "Sprite.hpp"
	#include "Entity.hpp"
	#include "EntityWithPv.hpp"
	#include "EntityTransformation.hpp"
	#include "TransformationTranslation.hpp"
	#include "TransformationRotation.hpp"
	#include "TransformationBezier.hpp"
	#include "TransformationInterpolation.hpp"

	#include "FloatingDamage.hpp"
	#include "LightningBolt.hpp"
	#include "FxText.hpp"
	#include "LightningText.hpp"
	#include "PointMass.hpp"
	#include "Spring.hpp"
	#include "Bloom.hpp"
	#include "Grid.hpp"
	#include "BlackHole.hpp"

	#include "PostShader.hpp"
	#include "ShaderWave.hpp"

	#include "Camera.hpp"
	#include "Parallaxe.hpp"

	#include "SingletonTemplate.hpp"
	#include "DataManager.hpp"
	#include "PhysicManager.hpp"
	#include "SoundManager.hpp"
	#include "Console.hpp"
	#include "State.hpp"
	#include "StateLoading.hpp"
	#include "StateMachine.hpp"
	#include "GameManager.hpp"
	#include "InputManager.hpp"

	//The Grid
	#include "IncludesProjet.hpp"