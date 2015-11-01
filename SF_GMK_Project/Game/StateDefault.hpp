#ifndef STATE_DEFAULT_HPP
#define STATE_DEFAULT_HPP


class StateDefault : public sfgmk::engine::State
{
	public:
		StateDefault();
		~StateDefault();

		void init();
		void update();
		void deinit();

		void draw();
};


#endif