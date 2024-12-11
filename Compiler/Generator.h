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
		Entry currentFunc;
		size_t ifCounter = 0;
		bool realIf = false;
		size_t copyEqalGenerator = 0;

		void GenerateDataSection(size_t& index);
		void GenerateFunction(size_t& index);
		void GenerateMain(size_t& index);
		void GenerateExpression(std::list<std::string>& code, size_t& index, bool isReturn = false, size_t ind = 2, char end = ';', int iterRebase = -1);
		void GenerateLockalVars(LEXER::Entry& fun);
		void GenerateIf(std::list<std::string>& code, size_t& index, size_t localIfCounter, bool rebase = true);

		static inline std::array<std::string, COUNT_OF_TYPES * 2> types
		{
			"SDWORD",
			"BYTE",
			"DWORD",
			"REAL8",
			"DWORD",
			"DWORD",
			"SDWORD",
			"BYTE",
			"DWORD",
			"REAL8",
			"DWORD",
			"DWORD",
		};
	};
}

