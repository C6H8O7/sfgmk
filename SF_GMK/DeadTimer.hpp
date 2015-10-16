/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		29/11/2014
	@brief		Fichier d'entête de DeadTimer

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_DEADTIMER_HPP
#define SFGMK_DEADTIMER_HPP


namespace sfgmk
{
	class DeadTimer : public SpriteComponent
	{
		public:
			DeadTimer(float _timeInSeconds, float _differedStart = 0.0f);
			virtual ~DeadTimer();

			virtual bool update(Sprite& _sprite, float _timeDelta);

		private:
			float m_fLimit;
	};
}


#endif