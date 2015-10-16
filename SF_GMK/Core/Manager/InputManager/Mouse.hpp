/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		10/12/2014
	@brief		Fichier d'entête de Mouse

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_MOUSE_HPP
#define SFGMK_MOUSE_HPP


namespace sfgmk
{
	class Mouse
	{
		friend class InputManager;

		public:
			void updateMouse();
			void handleEvent(sf::Event _Event);

			sf::Vector2i getWindowPosition();
			sf::Vector2f getWorldPosition();

			int getButtonState(sf::Mouse::Button _Button);
			//0 = pas de roulement, 1 = roulement haut, -1 roulemant bas
			int getWheelState();

		private:
			Mouse();
			~Mouse();

			int m_KeyStates[sf::Mouse::ButtonCount];
			int m_iWheelState;
	};
}


#endif