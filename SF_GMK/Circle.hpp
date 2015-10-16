/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		25/11/2014
	@brief		Fichier d'entête de Circle

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_CIRCLE_HPP
#define SFGMK_CIRCLE_HPP


namespace sfgmk
{
	class Circle : public SpriteComponent
	{
		public:
			Circle(float _radius, float _degreesSeconds);
			virtual ~Circle();

			virtual bool update(Sprite& _sprite, float _timeDelta);

		private:
			float m_fRadius, m_AngularSpeed;
	};
}


#endif