#ifndef STATE_GAME_HPP
#define STATE_GAME_HPP


class StateGame : public sfgmk::engine::State
{
	public:
		StateGame();
		~StateGame();

		void init();
		void update();
		void deinit();

		void draw();

		void drawHud();
};


#endif