/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		25/11/2014
	@brief		Fichier d'entête de Animation

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_ANIMATION_HPP
#define SFGMK_ANIMATION_HPP


namespace sfgmk
{
	class Sprite;

	struct AnimationFrame
	{
		sf::Texture *texture;
		float duration;
	};

	class SFGMK_API Animation
	{
		public:
			Animation();
			Animation(Animation& _copy);
			~Animation();

			void FreeTextures();

			void update(Sprite& _sprite, float _timeDelta);

			bool AddFrame(std::string _filePath, float _duration, sf::IntRect _rectangle = sf::IntRect());

			void SetLoop(bool _loop);
			bool GetLoop();

			void LoadFromFile(std::string _file);

		private:
			bool m_isLoop;

			float m_fTimer;

			int m_iCurrentFrame;

			std::vector<AnimationFrame*> m_Frames;
	};
}


#endif