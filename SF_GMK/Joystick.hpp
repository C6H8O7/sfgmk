/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		13/12/2014
	@brief		Fichier d'entête de Joystick

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_JOYSTICK_HPP
#define SFGMK_JOYSTICK_HPP


namespace sfgmk
{
	#define VIBRATION_FULL_POW 65535 //65535 = maximum, (65535 / 16) = à peine perceptible
	#define JOYSTICK_TOLERANCE 25 //Valeur qui semble correcte pour le contrôleur XBOX-360
	#define TRIGGER_TOLERANCE 1 //Valeur qui semble correcte pour le contrôleur XBOX-360
	#define JOYSTICK_MAX_VALUE 100

	class Joystick
	{
		friend class InputManager;

		public:
			void updateJoystick();
			void handleEvent(sf::Event _Event);

			int getButtonState(int _Button);
			float getAxis(sf::Joystick::Axis _Axis);

			int getJoystickTolerance();
			void setJoystickTolerance(int _Value);
			int getTriggerTolerance();
			void Joystick::setTriggerTolerance(int _Value);

			void setVibration(float _Time, sf::Vector2i _Vibration);
			void stopVibration();

		private:
			Joystick();
			~Joystick();

			void setId(int _Id);
			int m_iId;

			int m_KeyStates[sf::Joystick::ButtonCount];

			int m_iJoystickTolerance;
			int m_iTriggerTolerance;

			XINPUT_VIBRATION m_Vibration;
			float m_fVibrationTimer;
	};
}


#endif