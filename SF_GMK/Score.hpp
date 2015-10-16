/*--------------------------------------------------------------------------------------------------

	@author		KCC
	@date		04/07/2015
	@brief		Etat d'affichage des scores

--------------------------------------------------------------------------------------------------*/

#ifndef STATE_SCORE_HPP
#define STATE_SCORE_HPP


class StateScore : public sfgmk::State
{
	public:
		StateScore();
		~StateScore();

		void init();
		void update();
		void deinit();

		void draw();
};


#endif