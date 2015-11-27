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
			sf::Color FillColor;

			int iStep; //Z-Path

			float fWeight;	//Dijkstra
			sf::Vector2i* Predecessor; //Dijkstra
		};

		enum ePATHFINDING_ALGOS
		{
			eZpath = 0,
			eDijkstra,
			eAStar,
			ePATHFINDING_ALGOS_NUMBER
		};

		std::string m_AlgoStringArray[ePATHFINDING_ALGOS_NUMBER];

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

		FoncterTemplateArray m_PathfindingAlgos;
		FoncterTemplate* m_LaunchPathfinding;
		unsigned int m_uiCurrentAlgo;
		bool m_bPathFindingComputing;
		signed long long m_llExecutionTime;
		ThreadTemplate<> m_PathFindingThread;

		sf::Text m_StepText;
		sf::Font m_Font;

		sf::Text m_InstructionText;
		sf::Text m_AlgoText;

		sf::Vector2i m_Begin;
		sf::Vector2i m_End;
		std::vector<sf::Vector2i> m_Path;
		stCASE** m_CaseArray;

		//Z Path
		std::queue<sf::Vector2i> m_List;
		
		//Dijkstra
		struct NodeDijkstra
		{
			sf::Vector2i grid_node;

			NodeDijkstra* parent;
			float cost_so_far;
			bool open;
		};
		
		//AStar
		struct NodeAStar
		{
			sf::Vector2i grid_node;

			NodeAStar* parent;
			float cost_so_far;
			float heuristic;
			float estimated_total_cost;
			bool open;

			NodeAStar(sf::Vector2i _gridNode, NodeAStar* _parent = 0, float _costSoFar = 0.0f, float _heuristic = 0.0f, float _estimatedTotalCost = 0.0f, bool _open = true)
			{
				grid_node = _gridNode;
				parent = _parent;
				cost_so_far = _costSoFar;
				heuristic = _heuristic;
				estimated_total_cost = _estimatedTotalCost;
				open = _open;
			}
		};

	public:
		void init();
		void update();
		void deinit();

		void draw();

		void initArray();
		void clearList();

		sf::Vector2i getMouseCase(const sf::Vector2f& _MapDecal = sf::Vector2f(0.0f, 0.0f));
		bool isInCases(const sf::Vector2i& _Position);
		bool checkDiagonalWall(const sf::Vector2i& _CaseOne, const sf::Vector2i& _CaseTwo);

		void computePathfinding();
		void computeNextCases4(sf::Vector2i _CurrentCase, sf::Vector2i _Array[eNEXT_CASES_NUMBER_4]);
		void computeNextCases8(sf::Vector2i _CurrentCase, sf::Vector2i _Array[eNEXT_CASES_NUMBER_8]);

		//Algos pathfinding
		void zPath();
		void zPathComputeFoundPath(unsigned int _Step);

		void dijkstra();

		// A*
		int astar_search_in_list(sf::Vector2i _node, std::vector<NodeAStar*>& _list);
		void astar_remove_from_list(NodeAStar* _node, std::vector<NodeAStar*>& _list, bool _delete);
		NodeAStar* astar_find_smallest(std::vector<NodeAStar*>& _list);
		float astar_heuristic(sf::Vector2i& _node);
		void astar();
};


#endif