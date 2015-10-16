/*--------------------------------------------------------------------------------------------------

	@author		KCC
	@date		04/07/2015
	@brief		Etat des options

--------------------------------------------------------------------------------------------------*/

#ifndef STATE_OPTION_HPP
#define STATE_OPTION_HPP


class StateOption : public sfgmk::State
{
	public:
		StateOption();
		~StateOption();

		void init();
		void update();
		void deinit();

		void draw();
};


#endif