/*--------------------------------------------------------------------------------------------------

	@author		KCC
	@date		04/07/2015
	@brief		Etat de l'intro du jeu

--------------------------------------------------------------------------------------------------*/

#ifndef STATE_INTRO_HPP
#define STATE_INTRO_HPP


class StateIntro : public sfgmk::State
{
	private:
		sfgmk::LightningText* m_Title;

	public:
		StateIntro();
		~StateIntro();

		void init();
		void update();
		void deinit();

		void draw();
};


#endif