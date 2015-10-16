/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		25/11/2014
	@brief		Fichier d'entête de Rotation

--------------------------------------------------------------------------------------------------*/

#ifndef _SFGMK_ROTATION_H_
#define _SFGMK_ROTATION_H_


namespace sfgmk
{
	class Rotation : public SpriteComponent
	{
		public:
			Rotation(float _degreesSeconds);
			virtual ~Rotation();

			virtual bool update(Sprite& _sprite, float _timeDelta);

		private:
			float m_AngularSpeed;
	};
}


#endif