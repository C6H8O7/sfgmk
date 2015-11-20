/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		29/11/2014
	@brief		Console développeur faite maison, donne des informations sur le programme et ses 
				performances

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_CONSOLE_DEV_HPP
#define SFGMK_CONSOLE_DEV_HPP


namespace sfgmk
{
	namespace engine
	{
		#define CONSOLE_SEIZURE_SIZE 32
		#define CONSOLE_FPS_SAMPLING 60
		#define CONSOLE_SIZE_X 576
		#define CONSOLE_SIZE_Y 720
		#define CONSOLE_UPDATE_TIMING 1.0f
		#define CONSOLE_DISPLAY_TIMING 0.1f
		#define CONSOLE_STRING_MAX_LINE 14
		#define CONSOLE_STRING_MAX_CHARACTER 34
		#define CONSOLE_COMMAND_PER_LINE 4
		#define COMMAND_COLOR_ACTIVE sf::Color(100, 255, 200, 255)
		#define COMMAND_COLOR_DEACTIVE sf::Color(205, 51, 51, 255)
		#define HELP_COMMAND_COLOR sf::Color(255, 165, 0, 255)
		#define OUTPUT_COLOR sf::Color::Yellow
		#define CURSOR_MAX_Y 595.0f
		#define CURSOR_MIN_Y 321.0f

		enum eCONSOLE_DEV_TEXT
		{
			eCpuConsoleText = 0,
			eRamConsoleText,
			eFmodConsoleText,

			eInputConsoleText,
			eSoundConsoleText,
			eStateConsoleText,
			eAIConsoleText,
			eEntityConsoleText,
			ePhysicConsoleText,
			eGraphicConsoleText,
			eMsgConsoleText,
			eDebugConsoleText,

			eTotalTimeUpdate,
			eTotalTimeDraw,

			eEntityCounter,
			eParallaxeCounter,

			eFpsConsoleText,

			eSeizureConsoleText,
			eConsoleText,
			eCONSOLE_DEV_TEXT_NUMBER
		};

		struct stCONSOLE_COMMAND
		{
			FoncterTemplate* Foncter;
			bool bBeenCalled;
			std::string sOnCallOutput;
			std::string sOnRecallOutput;
		};

		struct stCONSOLE_COMMAND_WITH_ARGS
		{
			FoncterTemplate* Foncter;
			std::string sOnCallOutput;
			unsigned int uiStringSize;
		};

		typedef struct _PROCESSOR_POWER_INFORMATION
		{
			ULONG  Number;
			ULONG  MaxMhz;
			ULONG  CurrentMhz;
			ULONG  MhzLimit;
			ULONG  MaxIdleState;
			ULONG  CurrentIdleState;
		} PROCESSOR_POWER_INFORMATION, *PPROCESSOR_POWER_INFORMATION;

		struct stCONSOLE_STRINGS
		{
			std::string sString;
			sf::Color Color;
		};

		class DebugManager;

		class SFGMK_API ConsoleDev
		{
			friend class DebugManager;

			private:
				ConsoleDev();
				~ConsoleDev();

				bool m_bOpacity;

				float m_fTimer;
				float m_fDisplayTimer;
				bool m_bIsActive;
				bool m_bIsSeizureActive;
				std::string m_sSeizureBuffer;
				std::vector<stCONSOLE_STRINGS*> m_sConsoleStrings;

				sf::Sprite m_RenderSprite;
				sf::RenderTexture m_ConsoleRender;
				sf::Sprite m_ConsoleSprite;
				sf::Sprite m_CursorSprite;
				sf::Font m_Font[2];
				sf::Texture m_Texture[2];

				struct sFPS_ON_A_SECOND 
				{
					float m_fTime;
					unsigned int uiFrames;
				}m_LastSecondFps;

				int m_iMinFps, m_iMaxFps;
				MEMORYSTATUSEX m_MemInfo;
				ULARGE_INTEGER m_LastCPU;
				ULARGE_INTEGER m_LastSysCPU;
				ULARGE_INTEGER m_LastUserCPU;
				HANDLE m_CurrentProcess;
				DWORD m_NumProcessors;
				float m_fCpuUsagePercent;

				int m_iFpsArray[CONSOLE_FPS_SAMPLING];
				sf::Image m_FpsCurbImageOriginal;
				sf::Image m_FpsCurbImage;
				sf::Texture m_FpsCurbTexture;
				sf::Sprite m_FpsCurbSprite;
				sf::Text m_TextArray[eCONSOLE_DEV_TEXT::eCONSOLE_DEV_TEXT_NUMBER];
				sf::Sprite m_CameraSprite;
				sf::Text m_CameraText;

				std::map<std::string, stCONSOLE_COMMAND> m_Commands;
				std::map<std::string, stCONSOLE_COMMAND_WITH_ARGS> m_CommandsWithArgs;
				std::vector<std::string> m_EnteredCommands;
				int m_iEnteredCommandsIndex;
				unsigned int m_uiConsoleDefillindex;

			public:
				bool setActive();
				void update(float _TimeDelta);
				void display();
				void draw(sf::RenderTexture* _Render);
				void setDisplayTimer(const float& _Value = 0.0f);

				int updateFps(float _TimeDelta);
				void updateFpsDraw();
				void updateFpsCurb();
				float updateFmodCharge();
				void updateSeizure();
				void updateCounters();

				void memoryUsage();
				void initCpuUsage();
				float cpuUsage();

				void print(const std::string& _String, const sf::Color& _Color = OUTPUT_COLOR);
				void helpCommand();
				void command(std::string _Seizure);
				void registerCommand(const std::string& _commandName, FoncterTemplate* _Foncter, const std::string& _CallOutput, const std::string& _RecallOutput, const bool& _InitialState = false);
				void registerCommandWithArgs(const std::string& _commandName, FoncterTemplate* _Foncter, const std::string& _CallOutput, const unsigned int& _StringLength);
		};
	}
}


#endif