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
		#define CONSOLE_SIZE_X 564
		#define CONSOLE_SIZE_Y 500
		#define CONSOLE_UPDATE_TIMING 1.0f
		#define CONSOLE_STRING_MAX_LINE 9

		enum eCONSOLE_DEV_TEXT
		{
			eCpu = 0,
			eRam,
			eFmod,
			eFps,
			eState,
			eParallaxe,
			eEntity,
			eSeizure,
			eConsoleText,
			eCONSOLE_DEV_TEXT_NUMBER
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

		class SFGMK_API ConsoleDev : public SingletonTemplate<ConsoleDev>
		{
			friend class SingletonTemplate<ConsoleDev>;

			public:
				bool setActive(bool _Boolean);
				bool setActive();
				void update(float _TimeDelta);
				bool display(sf::RenderTexture* _Render, sf::Vector2f _CameraOrigin);

				int updateFps(float _TimeDelta);
				void updateFpsDraw();
				void updateFpsCurb();
				float updateFmodCharge();
				void updateSeizure();
				void command();
				void updateCounters();

				void memoryUsage();
				void initCpuUsage();
				float cpuUsage();

			private:
				ConsoleDev();
				~ConsoleDev();

				bool m_bOpacity;

				float m_fTimer;
				bool m_bIsActive;
				bool m_bIsSeizureActive;
				std::string m_sSeizureBuffer;
				std::string m_sConsoleString;
				int m_iConsoleStringLine;

				sf::Sprite m_RenderSprite;
				sf::RenderTexture m_ConsoleRender;
				sf::Sprite m_ConsoleSprite;
				sf::Font m_Font[2];
				sf::Texture m_Texture[2];

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
		};
	}
}


#endif