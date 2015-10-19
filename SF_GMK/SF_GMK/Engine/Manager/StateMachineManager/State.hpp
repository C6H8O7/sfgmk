/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		16/12/2014
	@brief		Classe abstraire State

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_STATE_H
#define SFGMK_STATE_H


namespace sfgmk
{
	namespace engine
	{
		struct StateOptions
		{
			bool deinitAfter;
			bool getNewChild;
			bool goToParent;
			bool isContinuing;
		};

		class State
		{
			protected:
			State *m_Parent;
			State *m_Child;

			int m_ID;
			std::string m_sRessourcesPath;

			struct StateOptions m_Options;

			public:
			State(State *_parent = 0);
			virtual ~State();

			virtual void init();
			virtual void update();
			virtual void deinit();

			virtual void draw();

			const int& getID();
			void setID(const int& _Id);
			const std::string& getRessourcePath();
			void setRessourcePath(const std::string& _Path);

			StateOptions& getOptions();
			State*& getParent();
			State*& getChild();
		};
	}
};

#endif