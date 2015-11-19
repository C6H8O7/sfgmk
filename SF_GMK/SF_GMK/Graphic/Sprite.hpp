/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		25/11/2014
	@brief		Fichier d'entête de Sprite

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_SPRITE_HPP
#define SFGMK_SPRITE_HPP


namespace sfgmk
{
	class SFGMK_API Sprite : public sf::Sprite, WARDEN(Sprite)
	{
		public:
			Sprite();
			Sprite(std::string _resName, bool _isAnim);
			~Sprite();

			Animation* getAnimation();
			void setAnimation(Animation& _animation, const bool& _DeletePrevious = true);
			void deleteAnimation();
			bool getIsAnimation();

			void setRelativOrigin(float _rox, float _roy);

			sf::Vector2f getCenter();
			sf::Vector2f getSize();

			virtual Sprite& finalize(float _timeDelta);

			void SetLoop(bool _loop);
			bool GetLoop();

			void Pause();
			void Stop();
			void Play();

		protected:
			void FinalizeSprite(float _timeDelta);

			bool m_isAnim;
			Animation *m_Animation;
	};
}


#endif