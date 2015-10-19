/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		10/12/2014
	@brief		Singleton regroupant l'ensemble des accés clavier, souris et joystick

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_INPUT_MANAGER_HPP
#define SFGMK_INPUT_MANAGER_HPP


namespace sfgmk
{
	namespace engine
	{
		enum KeyStates
		{
			KEY_UP = 0,
			KEY_PRESSED = 1 << 0,
			KEY_DOWN = KEY_PRESSED | 1 << 1,
			KEY_RELEASED = 1 << 2,
		};

		enum eMouseWheelStates
		{
			WHEEL_BOT = -1,
			WHEEL_CENTER = 0,
			WHEEL_TOP = 1
		};

		class InputManager : public SingletonTemplate<InputManager>
		{
			friend class SingletonTemplate<InputManager>;

			public:
			void update();
			void handleEvent(sf::Event _Event);

			Keyboard& getKeyboard();
			Mouse& getMouse();
			Joystick& getJoystick(int _Index);

			sf::Vector2f getJoystickUnitVector(int _Id, sf::Joystick::Axis _AxisVertical, sf::Joystick::Axis _AxisHorizontal);
			float getJoystickAngle(int _Id, sf::Joystick::Axis _AxisVertical, sf::Joystick::Axis _AxisHorizontal);

			void setGeneralVibration(float _Time, sf::Vector2i _Vibration);

			private:
			InputManager();
			~InputManager();

			Keyboard m_Keyboard;
			Mouse m_Mouse;
			Joystick m_Joystick[sf::Joystick::Count];
		};
	}
}


#endif