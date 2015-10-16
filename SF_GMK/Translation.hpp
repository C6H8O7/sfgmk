/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		25/11/2014
	@brief		Fichier d'entête de Translation

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_TRANSLATION_HPP
#define SFGMK_TRANSLATION_HPP


namespace sfgmk
{
	class Translation : public SpriteComponent
	{
		public:

			Translation(float _degrees, float _speed);
			virtual ~Translation();

			virtual bool update(Sprite& _sprite, float _timeDelta);

		private:
			float m_Vx, m_Vy;

			float m_Speed;
	};
}


#endif