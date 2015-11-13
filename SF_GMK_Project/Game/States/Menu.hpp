#ifndef STATE_MENU_HPP
#define STATE_MENU_HPP


class StateMenu : public sfgmk::engine::State
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
