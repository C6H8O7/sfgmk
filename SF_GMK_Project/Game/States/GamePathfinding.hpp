#ifndef STATE_GAME_PATHFINDING_HPP
#define STATE_GAME_PATHFINDING_HPP


class StateGamePathfinding : public sfgmk::engine::State
{
	public:
		StateGamePathfinding();
		~StateGamePathfinding();

	private:
		PathfindingMap m_Map;

		sf::Font m_Font;
		sf::Text m_PathfindingText;
		sf::Text m_HudText;

		std::vector<std::string> m_MapFileName;
		std::string m_sAlgosNames[ePATHFINDING_ALGOS_NUMBER];
		unsigned int m_uiAlgoChosen;

		sf::Vector2i m_Begin, m_End;
		PathfindingPathCntr m_Path;

		Pathfinding m_Pathfinding;

	public:
		void init();
		void update();
		void deinit();

		void draw();

		sf::Vector2i getMouseCase(const sf::Vector2f& _MouseWorldPos, const sf::Vector2f& _MapDecal = sf::Vector2f(0.0f, 0.0f));
		bool isInCases(const sf::Vector2i& _Position);

		void drawCase(sf::RenderTexture* _Render, const sf::Vector2f& _Position, const sf::Color& _Color);
		void drawExploration();
};


#endif