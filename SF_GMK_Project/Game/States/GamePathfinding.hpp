#ifndef STATE_GAME_PATHFINDING_HPP
#define STATE_GAME_PATHFINDING_HPP


class StateGamePathfinding : public sfgmk::engine::State
{
	public:
		StateGamePathfinding();
		~StateGamePathfinding();

	private:
		Pathfinding m_Pathfinding;

		sf::Vector2i m_ArraySize, m_Begin, m_End;
		stPATHFINDING_CASE** m_CaseArray;
		std::vector<sf::Vector2i> m_Path;

		sf::RenderTexture* m_RenderCases;
		sf::Sprite m_RenderCasesSprite;
		sf::RenderTexture* m_RenderCasesState;
		sf::Sprite m_RenderCasesSpriteState;
		sf::Font m_Font;
		sf::Text m_PathfindingText;
		sf::Text m_HudText;

		std::vector<std::string> m_MapFileName;
		std::string m_sAlgosNames[ePATHFINDING_ALGOS_NUMBER];
		unsigned int m_uiAlgoChosen;

	public:
		void init();
		void update();
		void deinit();

		void draw();

		void initArray();
		void deleteArray();

		void loadFile(const std::string& _FileName);

		sf::Vector2i getMouseCase(const sf::Vector2f& _MouseWorldPos, const sf::Vector2f& _MapDecal = sf::Vector2f(0.0f, 0.0f));
		bool isInCases(const sf::Vector2i& _Position);

		void drawCase(const sf::Vector2f& _Position, const sf::Color& _Color);
};


#endif