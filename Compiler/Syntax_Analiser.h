#pragma once
#include "Lexer.h"

using namespace LEXER;

namespace ANALISER
{
	struct Analiser
	{
	public:
		void analise(std::vector<Lexem>& lexTable, IdTable& idTable);
	private:
		Entry currentFunction;
		void setCurrentFunction(std::vector<Lexem>& lexTable, IdTable& idTable, size_t& index);
		Keywords returnType(std::vector<Lexem>& lexTable, IdTable& idTable, size_t& index);
		void checkFun(std::vector<Lexem>& lexTable, IdTable& idTable, size_t& index);
		void checkExp(std::vector<Lexem>& lexTable, IdTable& idTable, size_t& index);
		void checkReturnType(std::vector<Lexem>& lexTable, IdTable& idTable, size_t& index);

		static inline std::unordered_map<Keywords, string> types 
		{
			{Int, "int"},
			{String, "string"},
			{Bool, "bool"},
			{Double, "double"},
		};

		static inline std::unordered_map<Keywords, Keywords> literalTypes
		{
			{IntLiteral, Int},
			{StringLiteral, String},
			{BoolLiteral, Bool},
			{DoubleLiteral, Double},
		};
	};
}

