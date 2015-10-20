/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		23/12/2014
	@brief		Fichier d'entête du Parser

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_PARSER_HPP
#define SFGMK_PARSER_HPP


namespace sfgmk
{
	class SFGMK_API ParserContent
	{
		public:
			ParserContent(std::string _content);

			ParserContent getLineAsContent();
			std::string getLineAsString();

			bool isEndOfContent();

			std::string& getContent();

			std::string getString();
			int getInt();
			float getFloat();
			double getDouble();

		private:
			int m_Pos;
			std::string m_Content;
	};

	class SFGMK_API Parser
	{
		public:
			Parser(std::string _file);
			~Parser();

			std::string& getContent();
			ParserContent retrieveContent(std::string _key);
			static std::string ReadFile(std::string& _file);

		private:
			std::string m_FileContent;
	};
}


#endif