namespace sfgmk
{
	namespace engine
	{
		State::State(State *_parent) : m_Parent(_parent), m_Child(NULL), m_sRessourcesPath("")
		{
			m_ID = STATE_MACHINE->getStateCreatedId();
			m_sRessourcesPath = STATE_MACHINE_MANAGER->m_States[m_ID].second;

			m_Options.deinitAfter = false;
			m_Options.getNewChild = false;
			m_Options.goToParent = false;
			m_Options.isContinuing = false;
		}

		State::~State()
		{
		}


		void State::init()
		{
		}

		void State::update()
		{
		}

		void State::deinit()
		{
		}


		void State::draw()
		{
		}


		const int& State::getID()
		{
			return m_ID;
		}

		void State::setID(const int& _Id)
		{
			m_ID = _Id;
		}

		const std::string& State::getRessourcePath()
		{
			return m_sRessourcesPath;
		}

		void State::setRessourcePath(const std::string& _Path)
		{
			m_sRessourcesPath = _Path;
		}


		StateOptions& State::getOptions()
		{
			return m_Options;
		}

		State*& State::getParent()
		{
			return m_Parent;
		}

		State*& State::getChild()
		{
			return m_Child;
		}
	}
};