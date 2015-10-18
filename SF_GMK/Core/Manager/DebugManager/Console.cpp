namespace sfgmk
{
	ConsoleDev::ConsoleDev() : m_bOpacity(false), m_fTimer(0.0f), m_bIsActive(false), m_bIsSeizureActive(false), m_sSeizureBuffer(""), m_sConsoleString(""), m_iConsoleStringLine(0), m_fCpuUsagePercent(0.0f)
	{
		//Init
		m_ConsoleRender.create(CONSOLE_SIZE_X, CONSOLE_SIZE_Y);
		initCpuUsage();

		for( int i(0); i < 60; i++ )
			m_iFpsArray[i] = 0;
		
		//Rendu
		m_FpsCurbImageOriginal.loadFromFile(SFGMK_DATA_PATH + "console/fpsTab.png");
		m_Font[0].loadFromFile(SFGMK_DATA_PATH + "console/fontSquare.ttf");
		m_Font[1].loadFromFile(SFGMK_DATA_PATH + "console/fontCOMUNICA_TYPE.ttf");
		m_Texture[0].loadFromFile(SFGMK_DATA_PATH + "console/background.png");
		m_ConsoleSprite.setTexture(m_Texture[0], true);
		m_ConsoleSprite.setPosition(0.0f, 0.0f);

		//Texts
		for( int i(0); i < eCONSOLE_DEV_TEXT::eCONSOLE_DEV_TEXT_NUMBER; i++ )
		{
			if( i < eCONSOLE_DEV_TEXT::eSeizure )
			{
				m_TextArray[i].setFont(m_Font[0]);
				m_TextArray[i].setCharacterSize(16);
				m_TextArray[i].setColor(sf::Color(100, 200, 100, 255));
			}
			else
			{
				m_TextArray[i].setFont(m_Font[1]);
				m_TextArray[i].setCharacterSize(22);
				m_TextArray[i].setColor(sf::Color::White);
			}
		}

		m_TextArray[eCONSOLE_DEV_TEXT::eEntity].setCharacterSize(11);

		m_TextArray[eCONSOLE_DEV_TEXT::eCpu].setPosition(25.0f, 20.0f);
		m_TextArray[eCONSOLE_DEV_TEXT::eFmod].setPosition(25.0f, 40.0f);
		m_TextArray[eCONSOLE_DEV_TEXT::eRam].setPosition(25.0f, 60.0f);
		m_TextArray[eCONSOLE_DEV_TEXT::eState].setPosition(25.0f, 90.0f);
		m_TextArray[eCONSOLE_DEV_TEXT::eParallaxe].setPosition(25.0f, 110.0f);
		m_TextArray[eCONSOLE_DEV_TEXT::eEntity].setPosition(360.0f, 130.0f);
		m_TextArray[eCONSOLE_DEV_TEXT::eFps].setPosition(30.0f, 151.0f);
		m_TextArray[eCONSOLE_DEV_TEXT::eSeizure].setPosition(25.0f, 450.0f);
		m_TextArray[eCONSOLE_DEV_TEXT::eConsoleText].setPosition(30.0f, 225.0f);


		m_FpsCurbSprite.setPosition(95.0f, m_TextArray[eCONSOLE_DEV_TEXT::eFps].getPosition().y - 18);

		//Camera
		m_Texture[1].loadFromFile(SFGMK_DATA_PATH + "console/camera.png");
		m_CameraSprite.setTexture(m_Texture[1], true);
		m_CameraSprite.setPosition(500.0f, 0.0f);
		m_CameraText.setFont(m_Font[0]);
		m_CameraText.setCharacterSize(12);
		m_CameraText.setColor(sf::Color(100, 200, 100, 255));
		m_CameraText.setPosition(505.0f, 68.0f);
	}

	ConsoleDev::~ConsoleDev()
	{
	}


	bool ConsoleDev::setActive(bool _Boolean)
	{
		m_bIsActive = _Boolean;
		m_fTimer = 0.0f;

		return m_bIsActive;
	}

	bool ConsoleDev::setActive()
	{
		m_bIsActive = !m_bIsActive;
		m_fTimer = 0.0f;

		return m_bIsActive;
	}

	void ConsoleDev::update(float _TimeDelta)
	{
		//Update des valeurs de "performance"
		m_fTimer += _TimeDelta;
		if( m_fTimer >= CONSOLE_UPDATE_TIMING )
		{
			m_fTimer = 0.0f;

			updateFps(_TimeDelta);
			updateFpsCurb();

			if( m_bIsActive )
			{
				updateFpsDraw();
				updateFmodCharge();
				memoryUsage();
				cpuUsage();
				updateCounters();
			}
		}
		
		if( m_bIsActive )
		{
			//Console opaque ou non
			if( sfgmk::InputManager::getSingleton()->getKeyboard().getKeyState(sf::Keyboard::Tab) == sfgmk::KeyStates::KEY_PRESSED )
				m_bOpacity = !m_bOpacity;

			//Update saisie
			updateSeizure();
		}
	}

	bool ConsoleDev::display(sf::RenderTexture* _Render, sf::Vector2f _CameraOrigin)
	{
		if( !m_bIsActive )
			return false;

		float fScale = 1.0f / GRAPHIC_MANAGER->getCurrentCamera()->getZoomFactor(); //Pour que la console garde sa taille quand on a zoomé/dézoomé la vue

		//Draw
			//Console
			m_bOpacity ? m_ConsoleRender.clear(sf::Color(0, 0, 0, 255)) : m_ConsoleRender.clear(sf::Color(0, 0, 0, 0));
		
			m_ConsoleRender.draw(m_ConsoleSprite, sf::RenderStates::Default);
			for( int i(0); i < eCONSOLE_DEV_TEXT::eCONSOLE_DEV_TEXT_NUMBER; ++i )
				m_ConsoleRender.draw(m_TextArray[i]);
			m_ConsoleRender.draw(m_FpsCurbSprite);

			//Caméra (si le free move est activée)
			Camera* CurrentCam = GRAPHIC_MANAGER->getCurrentCamera();
			if( CurrentCam->getFreeMove() )
			{
				sf::Vector2i CameraPosition = (sf::Vector2i)CurrentCam->getCenter();
				std::string sX = std::to_string(CameraPosition.x);
				std::string sY = std::to_string(CameraPosition.y);
				std::string sZoom = std::to_string(CurrentCam->getZoomFactor());
				sZoom = sZoom.substr(0, sZoom.find('.') + 3);
				m_CameraText.setString("X:\t" + sX + "\nY:\t" + sY + "\n\n\t" + sZoom + " X");

				m_ConsoleRender.draw(m_CameraSprite);
				m_ConsoleRender.draw(m_CameraText);
			}

		m_ConsoleRender.display();
		m_RenderSprite.setTexture(m_ConsoleRender.getTexture(), true);
		m_RenderSprite.setPosition(_CameraOrigin);
		m_RenderSprite.setScale(fScale, fScale);
		_Render->draw(m_RenderSprite);

		return true;
	}


	int ConsoleDev::updateFps(float _fTimeDelta)
	{
		//Décalage du tableau
		for( int i(0); i < CONSOLE_FPS_SAMPLING - 1; ++i )
			m_iFpsArray[i] = m_iFpsArray[i + 1];
		m_iFpsArray[CONSOLE_FPS_SAMPLING - 1] = (int)(1.0f / _fTimeDelta);

		return m_iFpsArray[CONSOLE_FPS_SAMPLING - 1];
	}

	void ConsoleDev::updateFpsDraw()
	{
		char cFps[16] = { 0 };  //Généralement les FPS max sont compris entre 1000 et 5000, donc chaîne de 5 minimum pour la variable, + le texte
		int iFps = m_iFpsArray[CONSOLE_FPS_SAMPLING - 1];
		sprintf_s(cFps, "FPS:%d", iFps);

		//Change la couleur du texte en fonction du framerate
		if( iFps >= 60 )
			m_TextArray[eCONSOLE_DEV_TEXT::eFps].setColor(sf::Color::Green);
		else if( iFps >= 30 )
			m_TextArray[eCONSOLE_DEV_TEXT::eFps].setColor(sf::Color::Yellow);
		else
			m_TextArray[eCONSOLE_DEV_TEXT::eFps].setColor(sf::Color::Red);

		//Set le nombre de fps au texte
		m_TextArray[eCONSOLE_DEV_TEXT::eFps].setString(cFps);
	}

	void ConsoleDev::updateFpsCurb()
	{
		m_FpsCurbImage = m_FpsCurbImageOriginal;
		int iFps(0);
		int iRange(0);
		sf::Color Color;

		for( int i(0); i < CONSOLE_FPS_SAMPLING; ++i )
		{
			iFps = m_iFpsArray[i] >> 1; //Comme on a définie 120 comme maximum dans l'affichage console, on divise les valeurs par 2

			if( iFps > 60 )
				iFps = 60;

			//Dessin du graphe
			if( iFps )
			{
				if( iFps >= 30 )
					Color = sf::Color::Green;
				else if( iFps >= 15 )
					Color = sf::Color::Yellow;
				else
					Color = sf::Color::Red;

				//Point
				m_FpsCurbImage.setPixel(i * 4 + 2, 62 - iFps, Color);

				//Courbe
				Color.a = 50;
				if( i > 0 )
				{
					iRange = (iFps - iRange) / 4; //Ecart entre l'itération actuelle et la précédente

					m_FpsCurbImage.setPixel(i * 4 + 1, 62 - iFps + iRange, Color);
					m_FpsCurbImage.setPixel(i * 4, 62 - iFps + iRange * 2, Color);
					m_FpsCurbImage.setPixel(i * 4 - 1, 62 - iFps + iRange * 3, Color);
				}
			}

			iRange = iFps; //On garde la valeur de l'itération précédente
		}

		m_FpsCurbTexture.loadFromImage(m_FpsCurbImage);
		m_FpsCurbSprite.setTexture(m_FpsCurbTexture);
	}

	float ConsoleDev::updateFmodCharge()
	{
		char cCharge[24] = { 0 };
		float fValue = SoundManager::getSingleton()->getCpuCharge();

		sprintf_s(cCharge, "FMOD system:  %2.2f %%", fValue);
		m_TextArray[eCONSOLE_DEV_TEXT::eFmod].setString(cCharge);

		return fValue;
	}

	void ConsoleDev::updateSeizure()
	{
		sf::Vector2i MousePosition = SFGMK_MOUSE_WINDOW_POS;
		int iMouseLeftClick = SFGMK_MOUSE.getButtonState(sf::Mouse::Left);

		//Activation mode saisie texte
		if( iMouseLeftClick == KEY_PRESSED && (MousePosition.x > 0 && MousePosition.x < CONSOLE_SIZE_X && MousePosition.y > 0 && MousePosition.y < CONSOLE_SIZE_Y) )
		   m_bIsSeizureActive = true;
		else if( iMouseLeftClick == KEY_PRESSED )
			m_bIsSeizureActive = false;

		//Update de la saisie texte
		if( m_bIsSeizureActive )
		{
			char cLastChar(0);
			int iSize = m_sSeizureBuffer.length();

			//Envoyer commande
			if( sfgmk::InputManager::getSingleton()->getKeyboard().getKeyState(sf::Keyboard::Return) == sfgmk::KeyStates::KEY_PRESSED )
			{
				command();
				m_sSeizureBuffer.clear();
			}
			else if( iSize > 0 && sfgmk::InputManager::getSingleton()->getKeyboard().getKeyState(sf::Keyboard::BackSpace) == sfgmk::KeyStates::KEY_PRESSED )
				m_sSeizureBuffer.pop_back();
			else if( iSize < CONSOLE_SEIZURE_SIZE && (cLastChar = sfgmk::InputManager::getSingleton()->getKeyboard().getLastChar()) )
				m_sSeizureBuffer.push_back(cLastChar);

			//Update text
			if( iSize == 0 && m_fTimer > 0.5f )
				m_TextArray[eCONSOLE_DEV_TEXT::eSeizure].setString("_");
			else
				m_TextArray[eCONSOLE_DEV_TEXT::eSeizure].setString(m_sSeizureBuffer);

			//Update affichage
			m_TextArray[eCONSOLE_DEV_TEXT::eConsoleText].setString(m_sConsoleString);
		}
	}

	void ConsoleDev::command()
	{
		std::transform(m_sSeizureBuffer.begin(), m_sSeizureBuffer.end(), m_sSeizureBuffer.begin(), ::tolower);

		//Caméra libre
		if( m_sSeizureBuffer == "/freecam" )
		{
			bool bIsFreeMove = GRAPHIC_MANAGER->getCurrentCamera()->setFreeMove();
			bIsFreeMove ? m_sConsoleString += "Camera libre activee\n" : m_sConsoleString += "Camera libre desactivee\n";
		}
		//Draw colliders
		else if( m_sSeizureBuffer == "/physic" )
		{
			bool bIsPhysicDraw = sfgmk::PhysicManager::getSingleton()->getDraw();
			sfgmk::PhysicManager::getSingleton()->setDraw(!bIsPhysicDraw);
			bIsPhysicDraw ? m_sConsoleString += "Affichage physique desactivee\n" : m_sConsoleString += "Affichage physique activee\n";
		}
		else
			m_sConsoleString += m_sSeizureBuffer + '\n';

		m_iConsoleStringLine++;

		if( m_iConsoleStringLine > CONSOLE_STRING_MAX_LINE )
			m_sConsoleString = m_sConsoleString.substr(m_sConsoleString.find('\n') + 1);
	}

	void ConsoleDev::updateCounters()
	{
		sfgmk::GameManager* GameMan = sfgmk::GameManager::getSingleton();
		sfgmk::Parallaxe* GameParallaxe = &sfgmk::GameManager::getSingleton()->getParallaxe();
		stGameManagerTimers* Timers = &GameMan->getTimers();
		
		//Current state
		std::string sStateUpdate = std::to_string(Timers->dStateUpdate);
		sStateUpdate = sStateUpdate.substr(0, sStateUpdate.find('.') + 3);
		std::string sStateDisplay = std::to_string(Timers->dStateDraw);
		sStateDisplay = sStateDisplay.substr(0, sStateDisplay.find('.') + 3);
	
		m_TextArray[eCONSOLE_DEV_TEXT::eState].setString("State:\tUpdate:  " + sStateUpdate + " ms\t" + "Draw:  " + sStateDisplay + " ms");

		//Parallaxe
		std::string sEntity("Entity: " + std::to_string(GameMan->getEntityNumber()) + "  ");
		std::string sLayer("Layer: " + std::to_string(GameParallaxe->getLayerAccount()) + "  ");
		std::string sDraw("Draw: " + std::to_string(GameParallaxe->getDrawAccount()) + "  ");
		m_TextArray[eCONSOLE_DEV_TEXT::eParallaxe].setString("Parallaxe:\t" + sEntity + sLayer + sDraw);

		//Entity
		std::string sUpdate = std::to_string(Timers->dEntityUpdate);
		sUpdate = sUpdate.substr(0, sUpdate.find('.') + 3);
		std::string sSort = std::to_string(Timers->dEntitySort);
		sSort = sSort.substr(0, sSort.find('.') + 3);
		std::string sParallaxe = std::to_string(Timers->dParallaxeComputation);
		sParallaxe = sParallaxe.substr(0, sParallaxe.find('.') + 3);
		std::string sDisplay = std::to_string(Timers->dParallaxeDisplay);
		sDisplay = sDisplay.substr(0, sDisplay.find('.') + 3);
		std::string sPhysic = std::to_string(Timers->dPhysic);
		sPhysic = sPhysic.substr(0, sPhysic.find('.') + 3);

		m_TextArray[eCONSOLE_DEV_TEXT::eEntity].setString("\tEntity system:\nUpdate: " + sUpdate + " ms"
																			 + "\nSort: " + sSort + " ms"
																			 + "\nParallaxe: " + sParallaxe + " ms"
																			 + "\nPhysic: " + sPhysic + " ms"
																			 + "\nDisplay: " + sDisplay + " ms");
	}


	void ConsoleDev::memoryUsage()
	{
		char cMemory[64] = { 0 };

		MEMORYSTATUS stat;
		GlobalMemoryStatus(&stat);

		PROCESS_MEMORY_COUNTERS pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));

		float fTotalRam(stat.dwTotalPhys / 1024.0f / 1024.0f / 1024.0f);
		float fFreeRam(stat.dwAvailPhys / 1024.0f / 1024.0f / 1024.0f);
		float fUsedRam(fTotalRam - fFreeRam);
		std::string sUsedRam(std::to_string(fUsedRam));
		sUsedRam = sUsedRam.substr(0, 4);
		std::string sTotalRam(std::to_string(fTotalRam));
		sTotalRam = sTotalRam.substr(0, 4);

		float fTotalRam2(pmc.PeakWorkingSetSize / 1024.0f / 1024.0f);
		std::string sTotalRam2(std::to_string(fTotalRam2));
		sTotalRam2 = sTotalRam2.substr(0, 3);
		sUsedRam = sUsedRam.substr(0, 3);
		float fUsedRam2(pmc.WorkingSetSize / 1024.0f / 1024.0f);
		std::string sUsedRam2(std::to_string(fUsedRam2));
		sUsedRam2 = sUsedRam2.substr(0, 3);

		std::string PhysicRam("RAM:  " + sUsedRam2 + " Mo / " + sTotalRam2 + " Mo\t\t" + sUsedRam + " Go / " + sTotalRam + " Go");
		m_TextArray[eCONSOLE_DEV_TEXT::eRam].setString(PhysicRam);
	}

	void ConsoleDev::initCpuUsage()
	{
		SYSTEM_INFO SysInfo;
		FILETIME Time, Sys, User;

		GetSystemInfo(&SysInfo);
		m_NumProcessors = SysInfo.dwNumberOfProcessors;

		GetSystemTimeAsFileTime(&Time);
		memcpy(&m_LastCPU, &Time, sizeof(FILETIME));

		m_CurrentProcess = GetCurrentProcess();
		GetProcessTimes(m_CurrentProcess, &Time, &Time, &Sys, &User);

		memcpy(&m_LastSysCPU, &Sys, sizeof(FILETIME));
		memcpy(&m_LastUserCPU, &User, sizeof(FILETIME));
	}

	float ConsoleDev::cpuUsage()
	{
		FILETIME Time, Sys, User;
		ULARGE_INTEGER NowUsage, SysUsage, UserUsage;
		char cCpuUSage[64] = { 0 };

		//CPU usage %
		GetSystemTimeAsFileTime(&Time);
		memcpy(&NowUsage, &Time, sizeof(FILETIME));

		GetProcessTimes(m_CurrentProcess, &Time, &Time, &Sys, &User);
		memcpy(&SysUsage, &Sys, sizeof(FILETIME));
		memcpy(&UserUsage, &User, sizeof(FILETIME));

		m_fCpuUsagePercent = (float)((SysUsage.QuadPart - m_LastSysCPU.QuadPart) + (UserUsage.QuadPart - m_LastUserCPU.QuadPart));
		m_fCpuUsagePercent /= (float)(NowUsage.QuadPart - m_LastCPU.QuadPart);

		m_fCpuUsagePercent /= (float)m_NumProcessors;
		m_LastCPU = NowUsage;
		m_LastUserCPU = UserUsage;
		m_LastSysCPU = SysUsage;

		m_fCpuUsagePercent *= 100.0f;

		//CPU frequency mhz
		SYSTEM_INFO si = { 0 };
		::GetSystemInfo(&si);
		const int size = si.dwNumberOfProcessors * sizeof(PROCESSOR_POWER_INFORMATION);
		LPBYTE pBuffer = new BYTE[size];
		PPROCESSOR_POWER_INFORMATION ppi = { 0 };

		NTSTATUS status = ::CallNtPowerInformation(ProcessorInformation, NULL, 0, pBuffer, size);
		if( status >= 0 )
			ppi = (PPROCESSOR_POWER_INFORMATION)pBuffer;
		else
			std::cout << "CallNtPowerInformation failed. Status: " << status << std::endl;

		sprintf_s(cCpuUSage, "CPU:  %2.2f %%  (%d logical cores, %d MHz)", m_fCpuUsagePercent, m_NumProcessors, ppi->CurrentMhz);
		m_TextArray[eCONSOLE_DEV_TEXT::eCpu].setString(cCpuUSage);

		delete[]pBuffer;
		return m_fCpuUsagePercent;
	}
}