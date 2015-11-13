#ifndef STATE_GAME_HPP
#define STATE_GAME_HPP


class StateGame : public sfgmk::engine::State
{
	TiledMap m_map;

	public:
		StateGame();
		~StateGame();

		void init();
		void update();
		void deinit();

		void draw();
};


#endif