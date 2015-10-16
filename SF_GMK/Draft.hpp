/*--------------------------------------------------------------------------------------------------

	@author		KCC
	@date		04/07/2015
	@brief		Etat de création d'une nouvelle partie

--------------------------------------------------------------------------------------------------*/

#ifndef STATE_DRAFT_HPP
#define STATE_DRAFT_HPP


class StateDraft : public sfgmk::State
{
	private:
		sfgmk::DynamicArray<sfgmk::Grid*> m_Grids;

		bool m_bPlayerHasJoined[PLAYER_MAX_NUMBER];
		bool m_bPlayerHasChosen[PLAYER_MAX_NUMBER];

	public:
		StateDraft();
		~StateDraft();

		void init();
		void update();
		void deinit();

		void draw();

		void addGrid(const int& _Number);
		void removeGrid(const int& _Number);
};


#endif