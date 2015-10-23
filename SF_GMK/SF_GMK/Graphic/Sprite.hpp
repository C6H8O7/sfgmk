/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		25/11/2014
	@brief		Fichier d'entête de Sprite

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_SPRITE_HPP
#define SFGMK_SPRITE_HPP


namespace sfgmk
{
	class SFGMK_API Sprite : public sf::Sprite
	{
		public:
			Sprite();
			Sprite(std::string _resName, bool _isAnim);
			~Sprite();

			void setAnimation(Animation& _animation);

			void setRelativeOrigin(float _rox, float _roy);

			sf::Vector2f getCenter();
			sf::Vector2f getSize();

			virtual Sprite& finalize(float _timeDelta);

			void SetLoop(bool _loop);
			bool GetLoop();

		protected:
			void FinalizeSprite(float _timeDelta);

			bool m_isAnim;
			Animation *m_Animation;
	};
}


#endif