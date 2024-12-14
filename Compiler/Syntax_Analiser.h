#pragma once
#define END_BRACKET ')'
#include "Lexer.h"

using namespace LEXER;

namespace ANALISER
{
	struct Analiser
	{
	public:
		void analise(std::vector<Lexem>& lexTable, IdTable& idTable);
		static inline std::unordered_map<Keywords, string> types
		{
			{Int, "int"},
			{String, "string"},
			{Bool, "bool"},
			{Double, "double"},
			{Byte, "byte"},
			{Char, "char"},
			{UInt, "uint"},
			{IntLiteral, "int"},
			{StringLiteral, "string"},
			{BoolLiteral, "bool"},
			{DoubleLiteral, "double"},
			{ByteLiteral, "byte"},
			{CharLiteral, "char"},
			{UIntLiteral, "uint"},
		};
	private:
		Entry currentFunction;
		void setCurrentFunction(std::vector<Lexem>& lexTable, IdTable& idTable, size_t& index);
		Keywords returnType(std::vector<Lexem>& lexTable, IdTable& idTable, size_t& index);
		void checkFun(std::vector<Lexem>& lexTable, IdTable& idTable, size_t& index);
		void checkExp(std::vector<Lexem>& lexTable, IdTable& idTable, size_t& index, Entry& currentType, char endSymbol = ';');
		void checkReturnType(std::vector<Lexem>& lexTable, IdTable& idTable, size_t& index);
		void checkBlockRange(std::vector<Lexem>& lexTable, IdTable& idTable, size_t index, size_t& leftStart, size_t& leftEnd, size_t& rightStart, size_t& rightEnd);


		static inline std::unordered_map<Keywords, Keywords> literalTypes
		{
			{IntLiteral, Int},
			{StringLiteral, String},
			{BoolLiteral, Bool},
			{DoubleLiteral, Double},
			{CharLiteral, Char},
			{UIntLiteral, UInt},
			{ByteLiteral, Byte},
			{Int, Int},
			{String, String},
			{Bool, Bool},
			{Double, Double},
			{Char, Char},
			{UInt, UInt},
			{Byte, Byte},
		};
	};
}

