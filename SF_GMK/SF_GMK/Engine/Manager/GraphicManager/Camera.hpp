/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		16/12/2014
	@brief		Basé sur les sf::view, cette classe facilite la gestion des affichages

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_CAMERA_HPP
#define SFGMK_CAMERA_HPP


namespace sfgmk
{
	namespace engine
	{
#define FREE_CAMERA_DEFAULT_SPEED 250.0f

		class SFGMK_API Camera : public sf::View
		{
			private:
			void freeMove(float _TimeDelta);
			void vibrate(float _TimeDelta);

			float m_fZoomFactor;

			bool m_bLinked;
			Entity* m_LinkedEntity;
			sf::Vector2f m_LinkOffset;
			bool m_bFreeMove;
			sf::Vector2f m_Speed;

			bool m_bVibrate;
			sf::Vector2f m_VibrationRange;
			sf::Vector2f m_VibrationAttenuation;
			float m_fVibrationTimer;

			sf::Vector2f m_RelativOrigin;

			sf::Rect<float> m_Rect;

			public:
			Camera();
			~Camera();

			void update(float _TimeDelta);

			sf::Vector2f getRelativOrigin();
			sf::Vector2f getRelativOriginWithZoomCompensation();
			float getZoomFactor();
			void zoom(float factor);

			void linkToEntity(Entity* _Entity, const sf::Vector2f& _Offset = sf::Vector2f(0.0f, 0.0f));
			Entity* getLinkedEntity();
			void unlink(bool _DestroyEntity = false);
			bool getFreeMove();
			bool setFreeMove();

			void setVibration(float _Time, const sf::Vector2f& _Range, const sf::Vector2f& _Attenuation = sf::Vector2f(0.0f, 0.0f));
			void stopVibration();

			const sf::Rect<float>& getRect();
		};
	}
}


#endif