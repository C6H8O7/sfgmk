namespace sfgmk
{
	namespace engine
	{
		ConsoleDev::ConsoleDev() : m_bOpacity(false), m_fTimer(0.0f), m_fDisplayTimer(0.0f), m_bIsActive(false), m_bIsSeizureActive(false), m_sSeizureBuffer(""), m_iMinFps(0), m_iMaxFps(0), m_fCpuUsagePercent(0.0f), m_iEnteredCommandsIndex(-1), m_uiConsoleDefillindex(0U)
		{
			//Init
			m_ConsoleRender.create(CONSOLE_SIZE_X, CONSOLE_SIZE_Y);
			initCpuUsage();

			for( int i(0); i < 60; i++ )
				m_iFpsArray[i] = 0;

			m_LastSecondFps.m_fTime = 0.0f;
			m_LastSecondFps.uiFrames = 0U;
			
			//Rendu
			m_FpsCurbImageOriginal = DATA_MANAGER->getTexture("sfgmk_fpsTab").copyToImage();
			m_Font[0] = DATA_MANAGER->getFont("sfgmk_ConsoleFont1");
			m_Font[1] = DATA_MANAGER->getFont("sfgmk_ConsoleFont2");
			m_Texture[0] = DATA_MANAGER->getTexture("sfgmk_backgroundConsole");
			m_ConsoleSprite.setTexture(m_Texture[0], true);
			m_ConsoleSprite.setPosition(0.0f, 0.0f);
			m_CursorSprite.setTexture(DATA_MANAGER->getTexture("sfgmk_cursorConsole"));

			//Texts
			for( int i(0); i < eCONSOLE_DEV_TEXT::eCONSOLE_DEV_TEXT_NUMBER; i++ )
			{
				if( i < eCONSOLE_DEV_TEXT::eSeizureConsoleText )
				{
					m_TextArray[i].setFont(m_Font[0]);
					m_TextArray[i].setColor(sf::Color(100, 200, 100, 255));
					if( i > eFmodConsoleText )
						m_TextArray[i].setCharacterSize(16);
					else
						m_TextArray[i].setCharacterSize(20);
				}
				else
				{
					m_TextArray[i].setFont(m_Font[1]);
					m_TextArray[i].setCharacterSize(22);
					m_TextArray[i].setColor(sf::Color::White);
				}
			}

			m_TextArray[eCONSOLE_DEV_TEXT::eCpuConsoleText].setPosition(32.0f, 35.0f);
			m_TextArray[eCONSOLE_DEV_TEXT::eRamConsoleText].setPosition(32.0f, 52.0f);
			m_TextArray[eCONSOLE_DEV_TEXT::eFmodConsoleText].setPosition(32.0f, 69.0f);
			
			m_TextArray[eCONSOLE_DEV_TEXT::eInputConsoleText].setPosition(32.0f, 90.0f);
			m_TextArray[eCONSOLE_DEV_TEXT::eSoundConsoleText].setPosition(32.0f, 104.0f);
			m_TextArray[eCONSOLE_DEV_TEXT::eStateConsoleText].setPosition(32.0f, 118.0f);
			m_TextArray[eCONSOLE_DEV_TEXT::eAIConsoleText].setPosition(32.0f, 132.0f);
			m_TextArray[eCONSOLE_DEV_TEXT::eEntityConsoleText].setPosition(32.0f, 146.0f);
			m_TextArray[eCONSOLE_DEV_TEXT::ePhysicConsoleText].setPosition(32.0f, 160.0f);
			m_TextArray[eCONSOLE_DEV_TEXT::eGraphicConsoleText].setPosition(32.0f, 174.0f);
			m_TextArray[eCONSOLE_DEV_TEXT::eMsgConsoleText].setPosition(32.0f, 188.0f);
			m_TextArray[eCONSOLE_DEV_TEXT::eDebugConsoleText].setPosition(32.0f, 202.0f);

			m_TextArray[eCONSOLE_DEV_TEXT::eTotalTimeUpdate].setPosition(140.0f, 218.0f);
			m_TextArray[eCONSOLE_DEV_TEXT::eTotalTimeDraw].setPosition(295.0f, 218.0f);

			m_TextArray[eCONSOLE_DEV_TEXT::eEntityCounter].setPosition(352.0f, 245.0f);
			m_TextArray[eCONSOLE_DEV_TEXT::eEntityCounter].setCharacterSize(12);
			m_TextArray[eCONSOLE_DEV_TEXT::eParallaxeCounter].setPosition(352.0f, 270.0f);
			m_TextArray[eCONSOLE_DEV_TEXT::eParallaxeCounter].setCharacterSize(12);

			m_TextArray[eCONSOLE_DEV_TEXT::eFpsConsoleText].setPosition(32.0f, 241.0f);
			m_FpsCurbSprite.setPosition(95.0f, m_TextArray[eCONSOLE_DEV_TEXT::eFpsConsoleText].getPosition().y - 7.0f);

			m_TextArray[eCONSOLE_DEV_TEXT::eSeizureConsoleText].setPosition(26.0f, 652.0f);

			//Camera
			m_Texture[1] = DATA_MANAGER->getTexture("sfgmk_camera");
			m_CameraSprite.setTexture(m_Texture[1], true);
			m_CameraSprite.setPosition(512.0f, 0.0f);
			m_CameraText.setFont(m_Font[0]);
			m_CameraText.setCharacterSize(18);
			m_CameraText.setColor(sf::Color(100, 200, 100, 255));
			m_CameraText.setPosition(517.0f, 68.0f);
		}

		ConsoleDev::~ConsoleDev()
		{
			ClearPtrCntr(m_sConsoleStrings);
			m_Commands.clear();
			m_CommandsWithArgs.clear();
			m_EnteredCommands.clear();
		}


		bool ConsoleDev::setActive()
		{
			m_bIsActive = !m_bIsActive;
			m_fTimer = 0.0f;
			m_ConsoleRender.clear(EMPTY_COLOR);

			return m_bIsActive;
		}

		void ConsoleDev::update(float _TimeDelta)
		{
			m_fTimer += _TimeDelta;
			m_fDisplayTimer += _TimeDelta;

			//Stocke les fps pour faire une moyenne sur 1 seconde
			m_LastSecondFps.m_fTime += _TimeDelta;
			m_LastSecondFps.uiFrames++;

			//Update des valeurs de "performance"
			if( m_fTimer >= CONSOLE_UPDATE_TIMING )
			{
				m_fTimer = 0.0f;

				//Moyenne fps depuis derni�re update
				updateFps(m_LastSecondFps.m_fTime / (float)m_LastSecondFps.uiFrames);
				m_LastSecondFps.m_fTime = 0.0f;
				m_LastSecondFps.uiFrames = 0U;

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
				if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::Tab) == KEY_PRESSED )
					m_bOpacity = !m_bOpacity;

				//Update saisie
				updateSeizure();

				//Display
				if( m_fDisplayTimer >= CONSOLE_DISPLAY_TIMING )
				{
					display();
					m_fDisplayTimer = 0.0f;
				}
			}
		}

		void ConsoleDev::display()
		{
			//Draw
				//Console
				m_bOpacity ? m_ConsoleRender.clear(sf::Color(0, 0, 0, 255)) : m_ConsoleRender.clear(EMPTY_COLOR);

				m_ConsoleRender.draw(m_ConsoleSprite, sf::RenderStates::Default);
				for( int i(0); i < eCONSOLE_DEV_TEXT::eCONSOLE_DEV_TEXT_NUMBER - 1; ++i )
					m_ConsoleRender.draw(m_TextArray[i]);

				//Commandes entr�es
				unsigned int uiStart;
				(int)(m_sConsoleStrings.size() - CONSOLE_STRING_MAX_LINE) < 0 ? uiStart = 0U : uiStart = (int)(m_sConsoleStrings.size() - CONSOLE_STRING_MAX_LINE);

				for( unsigned int i(uiStart - m_uiConsoleDefillindex), j(0); i < m_sConsoleStrings.size() && i < uiStart + CONSOLE_STRING_MAX_LINE - m_uiConsoleDefillindex; i++, j++ )
				{
					m_TextArray[eCONSOLE_DEV_TEXT::eConsoleText].setString(m_sConsoleStrings[i]->sString);
					m_TextArray[eCONSOLE_DEV_TEXT::eConsoleText].setColor(m_sConsoleStrings[i]->Color);
					m_TextArray[eCONSOLE_DEV_TEXT::eConsoleText].setPosition(sf::Vector2f(32.0f, 322.0f) + sf::Vector2f(0.0f, j * 20.0f));
					m_ConsoleRender.draw(m_TextArray[eCONSOLE_DEV_TEXT::eConsoleText]);
				}

				m_ConsoleRender.draw(m_FpsCurbSprite);
				
				//Cursor
				m_CursorSprite.setPosition(479.0f, CURSOR_MAX_Y - (CURSOR_MAX_Y - CURSOR_MIN_Y) * ((float)m_uiConsoleDefillindex / (m_sConsoleStrings.size() - CONSOLE_STRING_MAX_LINE)));
				m_ConsoleRender.draw(m_CursorSprite);

				//Cam�ra (si le free move est activ�e)
				Camera* CurrentCam = GRAPHIC_MANAGER->getCurrentCamera();
				if( CurrentCam->getFreeMove() )
				{
					sf::Vector2i CameraPosition = (sf::Vector2i)CurrentCam->getCenter();
					std::string sX = std::to_string(CameraPosition.x);
					std::string sY = std::to_string(CameraPosition.y);
					std::string sZoom = std::to_string(CurrentCam->getZoomFactor());
					sZoom = sZoom.substr(0, sZoom.find('.') + 3);
					m_CameraText.setString("X: " + sX + "\nY: " + sY + "\n " + sZoom + " X");

					m_ConsoleRender.draw(m_CameraSprite);
					m_ConsoleRender.draw(m_CameraText);
				}

				m_ConsoleRender.display();
				m_RenderSprite.setTexture(m_ConsoleRender.getTexture(), true);
		}

		void ConsoleDev::draw(sf::RenderTexture* _Render)
		{
			_Render->setView(_Render->getDefaultView());
			_Render->draw(m_RenderSprite);
			_Render->setView(*CAMERA);

		}

		void ConsoleDev::setDisplayTimer(const float& _Value)
		{
			m_fDisplayTimer = _Value;
		}


		int ConsoleDev::updateFps(float _fTimeDelta)
		{
			//D�calage du tableau
			for( int i(0); i < CONSOLE_FPS_SAMPLING - 1; i++ )
				m_iFpsArray[i] = m_iFpsArray[i + 1];
			m_iFpsArray[CONSOLE_FPS_SAMPLING - 1] = (int)(1.0f / _fTimeDelta);

			return m_iFpsArray[CONSOLE_FPS_SAMPLING - 1];
		}

		void ConsoleDev::updateFpsDraw()
		{
			char cFps[64] = { 0 };
			int iFps = m_iFpsArray[CONSOLE_FPS_SAMPLING - 1];
			sprintf_s(cFps, "Max:%d\nFPS:%d\nMin:%d", m_iMaxFps, iFps, m_iMinFps);

			//Change la couleur du texte en fonction du framerate
			if( iFps >= 60 )
				m_TextArray[eCONSOLE_DEV_TEXT::eFpsConsoleText].setColor(sf::Color::Green);
			else if( iFps >= 30 )
				m_TextArray[eCONSOLE_DEV_TEXT::eFpsConsoleText].setColor(sf::Color::Yellow);
			else
				m_TextArray[eCONSOLE_DEV_TEXT::eFpsConsoleText].setColor(sf::Color::Red);

			//Set le nombre de fps au texte
			m_TextArray[eCONSOLE_DEV_TEXT::eFpsConsoleText].setString(cFps);
		}

		void ConsoleDev::updateFpsCurb()
		{
			m_FpsCurbImage = m_FpsCurbImageOriginal;
			int iFps(0);
			int iRange(0);
			sf::Color Color;
			m_iMinFps = m_iMaxFps = m_iFpsArray[CONSOLE_FPS_SAMPLING - 1];

			for( int i(0); i < CONSOLE_FPS_SAMPLING; ++i )
			{
				//Min / max
				if( m_iFpsArray[i] != 0 ) //Par d�faut le tableau est � 0
				{
					if( m_iMinFps > m_iFpsArray[i] )
						m_iMinFps = m_iFpsArray[i];
					if( m_iMaxFps < m_iFpsArray[i] )
						m_iMaxFps = m_iFpsArray[i];
				}

				iFps = m_iFpsArray[i] >> 1; //Comme on a d�finie 120 comme maximum dans l'affichage console, on divise les valeurs par 2

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
						iRange = (iFps - iRange) / 4; //Ecart entre l'it�ration actuelle et la pr�c�dente

						m_FpsCurbImage.setPixel(i * 4 + 1, 62 - iFps + iRange, Color);
						m_FpsCurbImage.setPixel(i * 4, 62 - iFps + iRange * 2, Color);
						m_FpsCurbImage.setPixel(i * 4 - 1, 62 - iFps + iRange * 3, Color);
					}
				}

				iRange = iFps; //On garde la valeur de l'it�ration pr�c�dente
			}

			m_FpsCurbTexture.loadFromImage(m_FpsCurbImage);
			m_FpsCurbSprite.setTexture(m_FpsCurbTexture);
		}

		float ConsoleDev::updateFmodCharge()
		{
			char cCharge[32] = { 0 };
			float fValue = SoundManager::getSingleton()->getCpuCharge();

			sprintf_s(cCharge, "FMOD:  %2.2f %%", fValue);
			m_TextArray[eCONSOLE_DEV_TEXT::eFmodConsoleText].setString(cCharge);

			return fValue;
		}

		void ConsoleDev::updateSeizure()
		{
			sf::Vector2i MousePosition = MOUSE_WINDOW_POS;
			int iMouseLeftClick = INPUT_MANAGER->MOUSE.getButtonState(sf::Mouse::Left);

			//Activation mode saisie texte
			if( iMouseLeftClick == KEY_PRESSED && (MousePosition.x > 0 && MousePosition.x < CONSOLE_SIZE_X && MousePosition.y > 0 && MousePosition.y < CONSOLE_SIZE_Y) )
			{
				m_bIsSeizureActive = true;
				m_iEnteredCommandsIndex = -1;
			}
			else if( iMouseLeftClick == KEY_PRESSED )
			{
				m_bIsSeizureActive = false;
				m_iEnteredCommandsIndex = -1;
			}

			//Update de la saisie texte
			if( m_bIsSeizureActive )
			{
				char cLastChar(0);
				int iSize = m_sSeizureBuffer.length();
			
				//Parcours des strings de la console
				if( INPUT_MANAGER->MOUSE.getWheelState() == 1 && m_uiConsoleDefillindex < m_sConsoleStrings.size() - CONSOLE_STRING_MAX_LINE )
					m_uiConsoleDefillindex++;
				else if( INPUT_MANAGER->MOUSE.getWheelState() == -1 && m_uiConsoleDefillindex > 0 )
					m_uiConsoleDefillindex--;

				//Parcours des commandes entr�es
				if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::Up) == KEY_PRESSED && m_iEnteredCommandsIndex < (int)(m_EnteredCommands.size() - 1) )
				{
					m_iEnteredCommandsIndex++;
					m_sSeizureBuffer = m_EnteredCommands[m_EnteredCommands.size() - m_iEnteredCommandsIndex - 1];
				}
				else if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::Down) == KEY_PRESSED )
				{
					if( m_iEnteredCommandsIndex > 0 )
					{
						m_iEnteredCommandsIndex--;
						m_sSeizureBuffer = m_EnteredCommands[m_EnteredCommands.size() - m_iEnteredCommandsIndex - 1];
					}
					else
					{
						m_iEnteredCommandsIndex = -1;
						m_sSeizureBuffer = "";
					}
				}

				//Envoyer commande
				if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::Return) == KEY_PRESSED )
				{
					command(m_sSeizureBuffer);
					m_EnteredCommands.push_back(m_sSeizureBuffer);
					m_sSeizureBuffer.clear();
					m_iEnteredCommandsIndex = -1;
				}
				else if( iSize > 0 && INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::BackSpace) == KEY_PRESSED )
				{
					m_sSeizureBuffer.pop_back();
					m_iEnteredCommandsIndex = -1;
				}
				else if( iSize < CONSOLE_SEIZURE_SIZE && (cLastChar = INPUT_MANAGER->KEYBOARD.getLastChar()) )
				{
					m_sSeizureBuffer.push_back(cLastChar);
					m_iEnteredCommandsIndex = -1;
				}

				//Update text
				if( iSize == 0 && m_fTimer > 0.5f )
					m_TextArray[eCONSOLE_DEV_TEXT::eSeizureConsoleText].setString("_");
				else
					m_TextArray[eCONSOLE_DEV_TEXT::eSeizureConsoleText].setString(m_sSeizureBuffer);
			}
		}

		void ConsoleDev::updateCounters()
		{
			sMANAGER_EXECUTION_TIMES* Timers = CORE->getManagersExecutionTimes();
			std::string sBuffers[2];
			signed long long llTotals[2] = { (signed long long)0.0, (signed long long)0.0 };

			//Calcul du total
			for( int i(0); i < eMANAGERS_NUMBER; i++ )
			{
				llTotals[0] += Timers[i].llUpdate;
				llTotals[1] += Timers[i].llDraw;
			}

			sBuffers[0] = std::to_string((double)(llTotals[0] * 0.001f));
			sBuffers[0] = sBuffers[0].substr(0, sBuffers[0].find('.') + 3);
			m_TextArray[eCONSOLE_DEV_TEXT::eTotalTimeUpdate].setString(sBuffers[0] + "ms (" + std::to_string((int)(1.0f / (llTotals[0] * 0.000001f))) + " FPS)");
														
			sBuffers[1] = std::to_string((double)(llTotals[1] * 0.001f));
			sBuffers[1] = sBuffers[1].substr(0, sBuffers[1].find('.') + 3);
			m_TextArray[eCONSOLE_DEV_TEXT::eTotalTimeDraw].setString(sBuffers[1] + "ms (" + std::to_string((int)(1.0f / (llTotals[1] * 0.000001f))) + " FPS)");

			//Affichage chaque manager
			sBuffers[0] = std::to_string((double)(Timers[eInputManager].llUpdate * 0.001f));
			sBuffers[0] = sBuffers[0].substr(0, sBuffers[0].find('.') + 3);
			sBuffers[1] = std::to_string((double)(Timers[eInputManager].llDraw * 0.001f));
			sBuffers[1] = sBuffers[1].substr(0, sBuffers[1].find('.') + 3);
			m_TextArray[eCONSOLE_DEV_TEXT::eInputConsoleText].setString("Input:       \tUpdate: " + sBuffers[0] + "ms");

			sBuffers[0] = std::to_string((double)(Timers[eSoundManager].llUpdate * 0.001f));
			sBuffers[0] = sBuffers[0].substr(0, sBuffers[0].find('.') + 3);
			m_TextArray[eCONSOLE_DEV_TEXT::eSoundConsoleText].setString("Sound:       \tUpdate: " + sBuffers[0] + "ms");

			sBuffers[0] = std::to_string((double)(Timers[eStateMachineManager].llUpdate * 0.001f));
			sBuffers[0] = sBuffers[0].substr(0, sBuffers[0].find('.') + 3);
			sBuffers[1] = std::to_string((double)(Timers[eStateMachineManager].llDraw * 0.001f));
			sBuffers[1] = sBuffers[1].substr(0, sBuffers[1].find('.') + 3);
			m_TextArray[eCONSOLE_DEV_TEXT::eStateConsoleText].setString("StateMachine:\tUpdate: " + sBuffers[0] + "ms\t\t"
																		+ "Draw: " + sBuffers[1] + "ms");

			sBuffers[0] = std::to_string((double)(Timers[eAIManager].llUpdate * 0.001f));
			sBuffers[0] = sBuffers[0].substr(0, sBuffers[0].find('.') + 3);
			sBuffers[1] = std::to_string((double)(Timers[eAIManager].llDraw * 0.001f));
			sBuffers[1] = sBuffers[1].substr(0, sBuffers[1].find('.') + 3);
			m_TextArray[eCONSOLE_DEV_TEXT::eAIConsoleText].setString("AI:          \tUpdate: " + sBuffers[0] + "ms\t\t"
																	+ "Draw: " + sBuffers[1] + "ms");

			sBuffers[0] = std::to_string((double)(Timers[eEntityManager].llUpdate * 0.001f));
			sBuffers[0] = sBuffers[0].substr(0, sBuffers[0].find('.') + 3);
			m_TextArray[eCONSOLE_DEV_TEXT::eEntityConsoleText].setString("Entity:      \tUpdate: " + sBuffers[0] + "ms");

			sBuffers[0] = std::to_string((double)(Timers[ePhysicManager].llUpdate * 0.001f));
			sBuffers[0] = sBuffers[0].substr(0, sBuffers[0].find('.') + 3);
			sBuffers[1] = std::to_string((double)(Timers[ePhysicManager].llDraw * 0.001f));
			sBuffers[1] = sBuffers[1].substr(0, sBuffers[1].find('.') + 3);
			m_TextArray[eCONSOLE_DEV_TEXT::ePhysicConsoleText].setString("Physic:      \tUpdate: " + sBuffers[0] + "ms\t\t"
																		+ "Draw: " + sBuffers[1] + "ms");

			sBuffers[0] = std::to_string((double)(Timers[eGraphicManager].llUpdate * 0.001f));
			sBuffers[0] = sBuffers[0].substr(0, sBuffers[0].find('.') + 3);
			sBuffers[1] = std::to_string((double)(Timers[eGraphicManager].llDraw * 0.001f));
			sBuffers[1] = sBuffers[1].substr(0, sBuffers[1].find('.') + 3);
			m_TextArray[eCONSOLE_DEV_TEXT::eGraphicConsoleText].setString("Graphic:     \tUpdate: " + sBuffers[0] + "ms\t\t"
																		+ "Draw: " + sBuffers[1] + "ms");

			sBuffers[0] = std::to_string((double)(Timers[eMsgManager].llUpdate * 0.001f));
			sBuffers[0] = sBuffers[0].substr(0, sBuffers[0].find('.') + 3);
			m_TextArray[eCONSOLE_DEV_TEXT::eMsgConsoleText].setString("Msg:         \tUpdate: " + sBuffers[0] + "ms");

			sBuffers[0] = std::to_string((double)(Timers[eDebugManager].llUpdate * 0.001f));
			sBuffers[0] = sBuffers[0].substr(0, sBuffers[0].find('.') + 3);
			sBuffers[1] = std::to_string((double)(Timers[eDebugManager].llDraw * 0.001f));
			sBuffers[1] = sBuffers[1].substr(0, sBuffers[1].find('.') + 3);
			m_TextArray[eCONSOLE_DEV_TEXT::eDebugConsoleText].setString("Debug:       \tUpdate: " + sBuffers[0] + "ms\t\t"
																		+ "Draw: " + sBuffers[1] + "ms");

			//Autres compteurs
			sBuffers[0] = std::to_string(ENTITY_MANAGER->getEntityNumber());
			sBuffers[1] = std::to_string((double)(ENTITY_MANAGER->getSortTime() * 0.001f));
			sBuffers[1] = sBuffers[1].substr(0, sBuffers[1].find('.') + 3);
			m_TextArray[eCONSOLE_DEV_TEXT::eEntityCounter].setString("Entity number:    " + sBuffers[0] + '\n'
																	 + "Entity sort time: " + sBuffers[1] + "ms");

			sBuffers[0] = std::to_string(GRAPHIC_MANAGER->getCurrentParallaxe()->getLastLoadLevelDataAccount());
			sBuffers[1] = std::to_string(GRAPHIC_MANAGER->getCurrentParallaxe()->getDrawAccount());
			m_TextArray[eCONSOLE_DEV_TEXT::eParallaxeCounter].setString("Layer number:     " + sBuffers[0] + '\n'
																	 + "Draw account:     " + sBuffers[1]);
		}


		void ConsoleDev::memoryUsage()
		{
			char cMemory[64] = { 0 };

			PROCESS_MEMORY_COUNTERS pmc;
			GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
			
			unsigned long fTotalRam2(pmc.PeakWorkingSetSize / 1024 / 1024);
			std::string sTotalRam2(std::to_string(fTotalRam2));
			sTotalRam2 = sTotalRam2.substr(0, 3);
			unsigned long fUsedRam2(pmc.WorkingSetSize / 1024 / 1024);
			std::string sUsedRam2(std::to_string(fUsedRam2));
			sUsedRam2 = sUsedRam2.substr(0, 3);

			std::string PhysicRam("RAM:  Current: " + sUsedRam2 + " Mo / Max: " + sTotalRam2 + " Mo");
			m_TextArray[eCONSOLE_DEV_TEXT::eRamConsoleText].setString(PhysicRam);
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
			m_TextArray[eCONSOLE_DEV_TEXT::eCpuConsoleText].setString(cCpuUSage);

			delete[]pBuffer;
			return m_fCpuUsagePercent;
		}


		void ConsoleDev::print(const std::string& _String, const sf::Color& _Color)
		{
			stSTRING_ARRAY* sStrings = cutStringInPart(_String, CONSOLE_STRING_MAX_CHARACTER);
			stCONSOLE_STRINGS* newConsoleString = NULL;

			for( unsigned int i(0U); i < sStrings->uiStringNumber; i++ )
			{
				newConsoleString = new stCONSOLE_STRINGS;
				newConsoleString->Color = _Color;
				newConsoleString->sString = sStrings->sStrings[i];

				m_sConsoleStrings.push_back(newConsoleString);
			}

			delete[] sStrings->sStrings;
			delete sStrings;
		}

		void ConsoleDev::helpCommand()
		{
			auto it = m_Commands.begin();
			
			while( it != m_Commands.end() )
			{
				stCONSOLE_STRINGS* newCommandString = new stCONSOLE_STRINGS;

				for( int i(0); i < CONSOLE_COMMAND_PER_LINE && it != m_Commands.end(); i++, ++it )
				{
					newCommandString->sString += "\t\t" + (*it).first + "\t\t";
					newCommandString->Color = HELP_COMMAND_COLOR;
				}

				m_sConsoleStrings.push_back(newCommandString);
			}

			auto itTwo = m_CommandsWithArgs.begin();

			while( itTwo != m_CommandsWithArgs.end() )
			{
				stCONSOLE_STRINGS* newCommandString = new stCONSOLE_STRINGS;

				for( int i(0); i < CONSOLE_COMMAND_PER_LINE && itTwo != m_CommandsWithArgs.end(); i++, ++itTwo )
				{
					newCommandString->sString += "\t\t" + (*itTwo).first + "\t\t";
					newCommandString->Color = HELP_COMMAND_COLOR;
				}

				m_sConsoleStrings.push_back(newCommandString);
			}
		}

		void ConsoleDev::command(std::string _Seizure)
		{
			std::transform(_Seizure.begin(), _Seizure.end(), _Seizure.begin(), ::tolower);

			//Recherche de la commande entr�e dans la liste de commandes
			auto it = m_Commands.find(_Seizure);
			if( it != m_Commands.end() )
			{
				(*it).second.Foncter->Execute();
				(*it).second.bBeenCalled = !(*it).second.bBeenCalled;

				stCONSOLE_STRINGS* newCommandString = new stCONSOLE_STRINGS;

				if( (*it).second.bBeenCalled )
				{
					newCommandString->sString = (*it).second.sOnCallOutput;
					newCommandString->Color = COMMAND_COLOR_ACTIVE;
				}
				else
				{
					newCommandString->sString = (*it).second.sOnRecallOutput;
					newCommandString->Color = COMMAND_COLOR_DEACTIVE;
				}
				
				m_sConsoleStrings.push_back(newCommandString);
			}
			else if( _Seizure == "/help" )
				helpCommand();

			//Recherche de la commande entr�e dans la liste de commandes avec arguments
			auto itTwo = m_CommandsWithArgs.find(_Seizure);
			if( itTwo != m_CommandsWithArgs.end() )
			{
				(*itTwo).second.Foncter->Execute();

				stCONSOLE_STRINGS* newCommandString = new stCONSOLE_STRINGS;
				newCommandString->sString = (*itTwo).second.sOnCallOutput;
				newCommandString->Color = COMMAND_COLOR_ACTIVE;

				m_sConsoleStrings.push_back(newCommandString);
			}
		}

		void ConsoleDev::registerCommand(const std::string& _commandName, FoncterTemplate* _Foncter, const std::string& _CallOutput, const std::string& _RecallOutput, const bool& _InitialState)
		{
			stCONSOLE_COMMAND NewCommand = { _Foncter,
											 _InitialState,
										     _CallOutput,
											 _RecallOutput };

			m_Commands.insert(std::pair<std::string, stCONSOLE_COMMAND>(_commandName, NewCommand));
		}

		void ConsoleDev::registerCommandWithArgs(const std::string& _commandName, FoncterTemplate* _Foncter, const std::string& _CallOutput, const unsigned int& _StringLength)
		{
			stCONSOLE_COMMAND_WITH_ARGS NewCommand = { _Foncter,
													   _CallOutput,
													   _StringLength };

			m_CommandsWithArgs.insert(std::pair<std::string, stCONSOLE_COMMAND_WITH_ARGS>(_commandName, NewCommand));
		}
	}
}