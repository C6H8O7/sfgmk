#ifndef STATE_GAME_PATHFINDING_HPP
#define STATE_GAME_PATHFINDING_HPP

#include "../../IncludesProjet.hpp"


class StateGamePathfinding : public sfgmk::engine::State
{
	public:
		StateGamePathfinding();
		~StateGamePathfinding();

	private:
		struct stCASE
		{
			bool bTested;
			bool bIswall;
			unsigned int uiStep;
			sf::Color FillColor;
		};

		enum ePATHFINDING_ALGOS
		{
			eZpath = 0,
			ePATHFINDING_ALGOS_NUMBER
		};

		enum eNEXT_CASES
		{
			eTop = 0,
			eRight,
			eBot,
			eLeft,
			eNEXT_CASES_NUMBER_4 = 4,
			eTopLeft = 4,
			eTopRight,
			eBotRight,
			eBotLeft,
			eNEXT_CASES_NUMBER_8
		};

		std::queue<sf::Vector2i> m_List;

		stCASE** m_CaseArray;
		sf::Vector2i m_Begin;
		sf::Vector2i m_End;
		std::vector<sf::Vector2i> m_Path;

		FoncterTemplateArray m_PathfindingAlgos;
		FoncterTemplate* m_LaunchPathfinding;
		unsigned int m_uiCurrentAlgo;
		bool m_bPathFindingComputing;
		signed long long m_llExecutionTime;
		ThreadTemplate<> m_PathFindingThread;

		sf::Text m_StepText;
		sf::Font m_Font;

		sf::Text m_InstructionText;

	public:
		void init();
		void update();
		void deinit();

		void draw();

		void initArray();
		void clearList();

		sf::Vector2i getMouseCase(const sf::Vector2f& _MapDecal = sf::Vector2f(0.0f, 0.0f));
		bool isInCases(const sf::Vector2i& _Position);

		void computePathfinding();
		void computeNextCases4(sf::Vector2i _CurrentCase, sf::Vector2i _Array[eNEXT_CASES_NUMBER_4]);
		void computeNextCases8(sf::Vector2i _CurrentCase, sf::Vector2i _Array[eNEXT_CASES_NUMBER_8]);

		//Algos pathfinding
		void zPath();
		void zPathComputeFoundPath(unsigned int _Step);
};


#endif