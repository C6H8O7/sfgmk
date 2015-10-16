/*--------------------------------------------------------------------------------------------------

	@author		KCC
	@date		04/07/2015
	@brief		Etat d'affichage des credits

--------------------------------------------------------------------------------------------------*/

#ifndef STATE_CREDIT_HPP
#define STATE_CREDIT_HPP


class StateCredit : public sfgmk::State
{
	public:
		StateCredit();
		~StateCredit();

		void init();
		void update();
		void deinit();

		void draw();
};


#endif