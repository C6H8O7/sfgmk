/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		25/11/2014
	@brief		Manager de son, stocke les différents effets/musiques. Utilise la librairie FMOD

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_SOUND_MANAGER_HPP
#define SFGMK_SOUND_MANAGER_HPP


namespace sfgmk
{
	namespace engine
	{
#define SOUND_MANAGER_TOTAL_CHANNEL 128
#define MIN_3D_DISTANCE_DEFAULT 10.0f
#define MAX_3D_DISTANCE_DEFAULT MIN_3D_DISTANCE_DEFAULT * 100.0f
#define FREQUENCY_SAMPLE 128 //[64, 8192]
#define REC_SHAPE_SIZE 25.0f

		class SFGMK_API SoundManager : public SingletonTemplate<SoundManager>
		{
			friend class SingletonTemplate<SoundManager>;

			public:
			void update();

			FMOD::System* getSystem();
			FMOD::Channel** getChannels();
			int getFreeChannel();

			std::string getFileName(char* _FilePath);
			//Charge un son directement par son emplacement
			bool loadMusic(char* _FilePath, char* _MusicName = NULL, int _FmodMode = FMOD_CREATESTREAM);
			bool unloadMusic(char* _MusicName);
			bool loadSound(char* _FilePath, char* _SoundName = NULL, int _FmodMode = FMOD_3D);
			bool unloadSound(char* _SoundName);
			bool loadLevel(const std::string& _Dir, engine::StateLoading* _State = NULL);
			bool unloadLevel(std::string _Dir);
			void unloadAll();

			//Return l'index du canal sur lequel est joué le son, -1 si le son n'a pas pu être joué
			int playMusic(std::string _MusicToplay, bool _Loop = false);
			//Return l'index du canal sur lequel est joué le son, -1 si le son n'a pas pu être joué
			int playSound(std::string _SoundToplay, bool _Loop = false);
			void playAll();
			void pauseAll();
			void stopAll();
			void playAllMusic();
			void pauseAllMusic();
			void stopAllMusic();
			void playAllSounds();
			void pauseAllSounds();
			void stopAllSounds();

			void setMasterVolume(float _Value);
			void setMusicVolume(float _Value);
			void setSoundVolume(float _Value);

			void setMasterPitch(float _Value);
			void setMusicPitch(float _Value);
			void setSoundPitch(float _Value);

			void set3DMinDistance(float _Value);
			void set3DMaxDistance(float _Value);

			float getCpuCharge();

			void setAnalyzeMusic(bool _Boolean, int _Channel = 0);
			float* getSpectrumArray();
			void drawSpectrum();
			bool getBeat();

			private:
			SoundManager();
			~SoundManager();

			void analyzeMusic();

			FMOD::System* m_SoundSystem;
			FMOD::Channel* m_Channel[SOUND_MANAGER_TOTAL_CHANNEL];
			FMOD::ChannelGroup *m_MusicGroup, *m_SoundGroup, *m_MasterGroup;

			std::map<std::string, FMOD::Sound*> m_MusicMap;
			std::map<std::string, FMOD::Sound*> m_SoundMap;

			float m_3DMinDistanceDefault;
			float m_3DMaxDistanceDefault;

			bool m_bAnalyze;
			int m_iChannelToAnalyze;
			float m_fFrequencyArray[FREQUENCY_SAMPLE];
			float m_fBeatDectection[3];
			bool m_bBeat;
			float fBeatTimer;
			sf::RectangleShape m_RecShape;
		};
	}
}


#endif