/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		27/11/2015
	@brief		Classe premettant de calculer un chemin en utilisant différents algos

--------------------------------------------------------------------------------------------------*/

#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP


namespace sfgmk
{
	enum ePATHFINDING_ALGOS
	{
		eZpath = 0,
		eDijkstra,
		eAStar,
		ePATHFINDING_ALGOS_NUMBER
	};

	struct stPATHFINDING_CASE
	{
		bool bTested;
		bool bIswall;
		unsigned int uiStep;
	};

	class SFGMK_API Pathfinding
	{
		public:
			Pathfinding();
			~Pathfinding();

		private:
			enum eNEXT_CASES
			{
				eTop = 0,
				eRight,
				eBot,
				eLeft,
				eNEXT_CASES_NUMBER_4 = 4,

				eTopRight = 4,
				eBotRight,
				eBotLeft,
				eTopLeft,
				eNEXT_CASES_NUMBER_8
			};

			struct stPATHFINDING_NODE
			{
				sf::Vector2i GridCoords;

				stPATHFINDING_NODE* ParentPtr;

				float fCostSoFar;
				float fHeuristic;
				float fEstimatedTotalCost;

				stPATHFINDING_NODE(sf::Vector2i _GridCoord, stPATHFINDING_NODE* _Parent = NULL, float _CostSoFar = 0.0f, float _Heuristic = 0.0f, float _EstimatedTotalCost = 0.0f)
				{
					GridCoords = _GridCoord;
					ParentPtr = _Parent;
					fCostSoFar = _CostSoFar;
					fHeuristic = _Heuristic;
					fEstimatedTotalCost = _EstimatedTotalCost;
				}
			};

			stPATHFINDING_CASE** m_CaseArray;
			sf::Vector2i m_Begin, m_End, m_GridSize;
			std::vector<sf::Vector2i>* m_Path;
			sf::Vector2i m_NextCases[eNEXT_CASES_NUMBER_8];
			sf::Vector2i m_CasesToTest[2];

			sfgmk::FoncterTemplateArray m_Algorithms;
			std::string m_sAlgosNames[ePATHFINDING_ALGOS_NUMBER];

			bool m_bStartFound;
			unsigned int m_uiStep;
			sf::Clock m_Clock;
			sf::Int64 m_ElapsedTime;

			std::queue<sf::Vector2i> m_ZPathList;

		public:
			inline void computeNextCases4(const sf::Vector2i& _CurrentCase, sf::Vector2i _Array[eNEXT_CASES_NUMBER_4]);
			inline void computeNextCases8(const sf::Vector2i& _CurrentCase, sf::Vector2i _Array[eNEXT_CASES_NUMBER_8]);
			inline bool isInCases(const sf::Vector2i& _Position);
			inline bool checkDiagonalWall(const sf::Vector2i& _CaseOne, const sf::Vector2i& _CaseTwo);

			void computePathfinding(std::vector<sf::Vector2i>* _Path, const ePATHFINDING_ALGOS& _Algo, stPATHFINDING_CASE** _Grid, const sf::Vector2i& _GridSize, const sf::Vector2i& _Begin, const sf::Vector2i& _End);

			void zPath();
			void dijkstra();
			void aStar();

			void sortDijkstra();

			inline int astar_search_in_list(const sf::Vector2i& _node, std::vector<stPATHFINDING_NODE*>& _list);
			inline void astar_remove_from_list(stPATHFINDING_NODE* _node, std::vector<stPATHFINDING_NODE*>& _list, bool _delete);
			inline stPATHFINDING_NODE* astar_find_smallest(std::vector<stPATHFINDING_NODE*>& _list);
			inline float astar_heuristic(const sf::Vector2i& _node);
	};
}


#endif