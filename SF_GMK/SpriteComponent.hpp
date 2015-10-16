/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		25/11/2014
	@brief		Fichier d'entête de SpriteComponent

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_SPRITE_COMPONENT_HPP
#define SFGMK_SPRITE_COMPONENT_HPP


namespace sfgmk
{
	class SpriteComponent
	{
		public:
			SpriteComponent();
			virtual ~SpriteComponent();

			virtual bool update(Sprite& _sprite, float _timeDelta) = 0;

		protected:
			float m_fTimer;
	};
}


#endif