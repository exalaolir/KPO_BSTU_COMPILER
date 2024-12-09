#pragma once
#define COUNT_OF_TYPES 6
#include "Lexer.h"

using namespace LEXER;

namespace GEN
{
	struct Generator
	{
		Generator(std::string path, std::vector<Lexem> lexTable,
		IdTable idTable);
		~Generator();

		void Generate();

	private:
		bool functionFlag = false;
		std::ofstream assembly;
		std::vector<Lexem> lexTable;
		std::list<std::string> code;
		IdTable idTable;

		void GenerateDataSection(size_t& index);
		void GenerateFunction(size_t& index);
		void GenerateMain(size_t& index);
		void GenerateExpression(size_t& index);
		void GenerateLockalVars(LEXER::Entry& fun);

		static inline std::array<std::string, COUNT_OF_TYPES * 2> types
		{
			"SDWORD",
			"BYTE",
			"BYTE",
			"REAL8",
			"BYTE",
			"BYTE",
			"SDWORD",
			"BYTE",
			"BYTE",
			"REAL8",
			"BYTE",
			"BYTE",
		};
	};
}

