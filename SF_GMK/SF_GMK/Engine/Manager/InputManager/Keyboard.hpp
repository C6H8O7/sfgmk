/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		29/11/2014
	@brief		Fichier d'entête de Keyboard

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_KEYBOARD_HPP
#define SFGMK_KEYBOARD_HPP


namespace sfgmk
{
	namespace engine
	{
		class SFGMK_API Keyboard
		{
			friend class InputManager;

			public:
			void updateKeyboard();
			void handleEvent(sf::Event _event);

			char getLastChar();
			int getKeyState(sf::Keyboard::Key _key);

			private:
			Keyboard();
			~Keyboard();

			char m_LastChar;
			int m_KeyStates[sf::Keyboard::KeyCount];
		};
	}
}


#endif