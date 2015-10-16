/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		25/11/2014
	@brief		Fichier d'entête de Sprite

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_SPRITE_HPP
#define SFGMK_SPRITE_HPP


namespace sfgmk
{
	class Sprite : public sf::Sprite
	{
		public:

			Sprite();
			Sprite(std::string _resName, bool _isAnim);
			~Sprite();

			void setAnimation(sfgmk::Animation& _animation);

			void setRelativeOrigin(float _rox, float _roy);

			sf::Vector2f getCenter();

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