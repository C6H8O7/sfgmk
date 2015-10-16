/*--------------------------------------------------------------------------------------------------

	@author		KCC
	@date		09/06/2015
	@brief		Etat du jeu

--------------------------------------------------------------------------------------------------*/

#ifndef STATE_GAME_HPP
#define STATE_GAME_HPP


class StateGame : public sfgmk::State
{
	private:
		sfgmk::Grid* m_Grid;
		sfgmk::DynamicArray<Player*> m_PlayerArray;

		sSHIP_STAT m_ShipsStats[ePLAYER_SHIP_TYPE_NUMBER];

	public:
		StateGame();
		~StateGame();

		void init();
		void update();
		void deinit();

		void draw();

		sfgmk::Grid* getGrid();

		sfgmk::DynamicArray<Player*>& getPlayers();
		void AddPlayer(const ePLAYER_SHIP_TYPE& _ShipType);

		void LoadShipsStats();
		const sSHIP_STAT& Get_ShipsStats(const ePLAYER_SHIP_TYPE& _Type);
};


#endif