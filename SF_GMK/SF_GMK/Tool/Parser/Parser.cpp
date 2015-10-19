namespace sfgmk
{
	//ParserContent
	ParserContent::ParserContent(std::string _content) : m_Content(_content), m_Pos(0)
	{
	}


	ParserContent ParserContent::getLineAsContent()
	{
		return ParserContent(getLineAsString());
	}

	std::string ParserContent::getLineAsString()
	{
		int nextEndLinePos = m_Content.find("\n", m_Pos);

		if( nextEndLinePos != std::string::npos )
		{
			std::string line = m_Content.substr(m_Pos, nextEndLinePos - m_Pos);
			m_Pos = nextEndLinePos + 1;

			if( line.empty() )
				return getLineAsString();

			return line;
		}
		else
			return "END_OF_CONTENT";
	}


	bool ParserContent::isEndOfContent()
	{
		return m_Content == "END_OF_CONTENT";
	}


	std::string& ParserContent::getContent()
	{
		return m_Content;
	}


	std::string ParserContent::getString()
	{
		std::string string = "END_OF_CONTENT";
		int pos_start = m_Content.find_first_not_of(" \t", m_Pos);

		if( pos_start == std::string::npos )
			return string;

		int pos_end = 0;
		if( m_Content[pos_start] == '\"' )
		{

			pos_end = m_Content.find_first_of('\"', pos_start + 1);
			string = m_Content.substr(pos_start + 1, pos_end - 1 - pos_start);
		}
		else
		{
			pos_end = m_Content.find_first_of(" \t", pos_start + 1);
			string = m_Content.substr(pos_start, pos_end - pos_start);
		}

		m_Pos = pos_end + 1;
		return string;
	}

	int ParserContent::getInt()
	{
		return (int)getDouble();
	}

	float ParserContent::getFloat()
	{
		return (float)getDouble();
	}

	double ParserContent::getDouble()
	{
		std::string str = getString();

		if( str == "END_OF_CONTENT" )
		{
			std::cout << "[PARSER] Error reading value (END_OF_CONTENT)" << std::endl;
			return 0.0f;
		}

		return strtod(str.c_str(), NULL);
	}


	//Parser
	Parser::Parser(std::string _file)
	{
		m_FileContent = Parser::ReadFile(_file);
	}

	Parser::~Parser()
	{
	}


	std::string& Parser::getContent()
	{
		return m_FileContent;
	}

	ParserContent Parser::retrieveContent(std::string _key)
	{
		std::string content = "NULL";

		int str_pos_start = m_FileContent.find("<" + _key + ">");
		int str_pos_end = m_FileContent.find("</" + _key + ">", str_pos_start);

		if( str_pos_start != std::string::npos && str_pos_end != std::string::npos )
		{
			str_pos_start += 1 + _key.size() + 1;
			content = m_FileContent.substr(str_pos_start, str_pos_end - str_pos_start);
		}

		return ParserContent(content);
	}

	std::string Parser::ReadFile(std::string& _file)
	{
		std::streampos fsize = 0;
		std::ifstream input_stream(_file);

		fsize = input_stream.tellg();
		input_stream.seekg(0, std::ios::end);
		fsize = input_stream.tellg() - fsize;
		input_stream.seekg(0, std::ios::beg);

		unsigned int size = (unsigned int)fsize;
		char *szContent = new char[++size];
		memset(szContent, 0, size);
		input_stream.read(szContent, fsize);

		input_stream.close();
		std::string content = szContent;
		delete szContent;

		return content;
	}	
}