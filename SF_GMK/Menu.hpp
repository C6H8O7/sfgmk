/*--------------------------------------------------------------------------------------------------

	@author		KCC
	@date		09/06/2015
	@brief		Etat du menu

--------------------------------------------------------------------------------------------------*/

#ifndef STATE_MENU_HPP
#define STATE_MENU_HPP


class StateMenu : public sfgmk::State
{
	public:
		StateMenu();
		~StateMenu();

		void init();
		void update();
		void deinit();

		void draw();
};


#endif