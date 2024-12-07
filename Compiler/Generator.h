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
		std::ofstream assembly;
		std::vector<Lexem> lexTable;
		IdTable idTable;

		void GenerateDataSection(size_t& index);


		static inline std::array<std::string, COUNT_OF_TYPES> types
		{
			"sdword",
			"byte",
			"byte",
			"real8",
			"byte",
			"byte",
		};
	};
}

