namespace sfgmk
{
	namespace engine
	{
		DebugManager::DebugManager() : m_Selector(NULL)
		{
			//Commandes console
			m_Console.registerCommand("/freecam", new sfgmk::FoncterTemplateInstance<Camera, void>(GraphicManager::getSingleton()->getCurrentCamera(), &Camera::setFreeMove), "Camera libre activee\n", "Camera libre desactivee\n");
			m_Console.registerCommand("/physic", new sfgmk::FoncterTemplateInstance<PhysicManager, void>(PHYSIC_MANAGER, &PhysicManager::setDraw), "Affichage physique activee\n", "Affichage physique desactivee\n");
			m_Console.registerCommand("/life", new sfgmk::FoncterTemplateInstance<CLASS_NULL, void>(&EntityWithPv::setLifeBarDraw), "Affichage vie activee\n", "Affichage vie desactivee\n");
			m_Console.registerCommand("/selector", new sfgmk::FoncterTemplateInstance<DebugManager, void>(this, &DebugManager::SetSelector), "Selector active\n", "Selector desactive\n");
			m_Console.registerCommand("/ai", new sfgmk::FoncterTemplateInstance<AIManager, void>(AI_MANAGER, &AIManager::setDrawAiState), "Affichage AI active\n", "Affichage AI desactive\n");
			m_Console.registerCommand("/id", new sfgmk::FoncterTemplateInstance<EntityManager, void>(ENTITY_MANAGER, &EntityManager::setDrawId), "Affichage ID active\n", "Affichage ID desactive\n");
			m_Console.registerCommand("/hud", new sfgmk::FoncterTemplateInstance<GraphicManager, void>(GRAPHIC_MANAGER, &GraphicManager::setDrawHud), "Affichage HUD active\n", "Affichage HUD desactive\n");
			m_Console.registerCommand("/warden", new sfgmk::FoncterTemplateInstance<DebugManager, void>(this, &DebugManager::ShowWarden), "", "");
			m_Console.registerCommand("/wardenwin", new sfgmk::FoncterTemplateInstance<DebugManager, void>(this, &DebugManager::ShowWardenWindows), "", "");

			//Commandes avec arguments
			//m_Console.registerCommand("/kill", new sfgmk::FoncterTemplateInstance<DebugManager, void>(this, &DebugManager::ShowWarden), "", "");

			//Destroy goomba
			/*else if( _Seizure.substr(0, 5) == "/kill" )
			{
				_Seizure = _Seizure.substr(6, _Seizure.length() - 6);

				unsigned int uiId;
				stringstream(_Seizure) >> uiId;

				if( MESSAGE_MANAGER->SendMsgToEntity(uiId, NULL, 0, MSG_TAG::TAG_BOOL) )
				{
					stCONSOLE_STRINGS* newCommandString = new stCONSOLE_STRINGS;
					newCommandString->sString = "Destruction entité " + _Seizure;
					newCommandString->Color = sf::Color::Cyan;;
					m_sConsoleStrings.push_back(newCommandString);
				}*/

				//Pop goomba
				/* if( _Seizure.substr(0, 4) == "/pop" )
				{
				_Seizure = _Seizure.substr(5, _Seizure.length() - 5);

				sf::Vector2f Position;
				stringstream(_Seizure) >> Position.x;
				_Seizure = _Seizure.substr(_Seizure.find_first_of(' ') + 1, _Seizure.length());
				stringstream(_Seizure) >> Position.y;

				Goomba* NewCleverGoomba = new Goomba();
				int iscale = RAND(1, 4);
				NewCleverGoomba->setScale(iscale * 0.5f, iscale * 0.5f);
				NewCleverGoomba->setPosition(Position);
				ADD_ENTITY(NewCleverGoomba);
				}*/
		}

		DebugManager::~DebugManager()
		{
		}


		void DebugManager::update(const float& _TimeDelta)
		{
			//Console
			if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::Quote) == KEY_PRESSED )
				m_Console.setActive();

			m_Console.update(_TimeDelta);
		}


		void DebugManager::SetSelector()
		{
			if( m_Selector )
			{
				m_Selector->setIsAlive(false);
				m_Selector = NULL;
			}
			else
			{
				m_Selector = new Selector(true);
				ADD_ENTITY(m_Selector);
			}
		}

		void DebugManager::DeleteSelector()
		{
			m_Selector = NULL;
		}


		void DebugManager::ShowWarden()
		{
			sfgmk::WardenReg *reg = sfgmk::WardenReg::getSingleton();
			DynamicArray<sfgmk::WardenReg::WardenRegEntry*>* entries = reg->getEntriesPtr();
			sfgmk::WardenReg::WardenRegEntry* entry;
			std::string str;
			
			for( unsigned int i = 0U; i < entries->getElementNumber(); i++ )
			{
				entry = (*entries)[i];
				str = entry->name + "\t\t" + std::to_string(entry->count) + "\n";
				CONSOLE.print(str, sf::Color(255, 255, 255, 255));
			}
		}

		void DebugManager::ShowWardenWindows()
		{
			sfgmk::WardenReg *reg = sfgmk::WardenReg::getSingleton();
			DynamicArray<sfgmk::WardenReg::WardenRegEntry*>* entries = reg->getEntriesPtr();
			sfgmk::WardenReg::WardenRegEntry* entry;
			std::string str;

			for( unsigned int i = 0U; i < entries->getElementNumber(); i++ )
			{
				entry = (*entries)[i];
				str = entry->name + "\t\t" + std::to_string(entry->count) + "\n";
				std::cout << str << std::endl;
			}
		}
	}
}