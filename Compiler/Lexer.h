#pragma once
#include "Symbols_And_Resourses.h"

namespace LEXER
{
	struct Lexem
	{
		Lexem(std::string lexema,
			  unsigned int line,
			  unsigned int index)
		{
			this->index = index;
			this->line = line;
			this->lexema = lexema;
		}
		std::string lexema;
		unsigned int line;
		unsigned int index;
		unsigned int positionInIdTable;
	};

	struct Entry
	{
		Keywords type = None;
		std::string name;
		Keywords valueType = None;
		std::string scope;
		std::string ownScope;
		int line = -1;
		int pos = -1;
		int params = 0;
		std::variant <int, double, bool, std::string, Keywords> value;
		
		std::variant <Keywords, 
			std::string, 
			int, 
			std::variant<int, double, bool, std::string, Keywords>
		> operator[](unsigned index) const;

		double GetHashCode() const;
		std::string GetValue();
	};

	struct IdTable
	{
		void Add(Entry& entry);
		bool Contains(Entry& entry) const;
		bool Contains(double& key) const;
		mutable std::unordered_map<double, Entry> table;
		std::list<double> keys;

		Entry& operator[](double key) const;
	};

	struct Lexer
	{
	public:
		Lexer(std::string& fileName);

		~Lexer();

		std::vector<Lexem> generateLexTable();

		IdTable generateIdTable();
	private:
		std::list<string> preprocessCode(std::string& code);
		std::string readFile(std::string& fileName);
		Keywords GetKeyword(string& token, int& line, int& counter);
		std::string scopeGenerator(string& priviosScope, string& id);
		void createFun(std::list<string>::iterator& currentTocken, IdTable& idTable, std::list<string>& tokens, int& line, int& counter, bool& hasMain);
		void createVar(std::list<string>::iterator& currentToken, IdTable& idTable, std::list<string>& tokens, int& line, int& counter, Keywords type);
		void createAuto(std::list<string>::iterator& currentToken, IdTable& idTable, std::list<string>& tokens, int& line, int& counter);
		inline std::string GetType(string& token, int& line, int& counter);

		std::ifstream inFile;
		std::list<string> preprocesseredStr;
		std::string sourseCode;

		std::stack<string> scopes;
		int currentScopeNumber;
		bool hasErrors = false;
	};
}