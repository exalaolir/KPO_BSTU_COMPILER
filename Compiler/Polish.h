#pragma once

namespace POLISH
{
	void changeLexTable(vector<LEXER::Lexem>& lexTable, LEXER::IdTable& idTable);

	template<typename T> void countPolish(std::list<LEXER::Lexem>& expression, LEXER::IdTable& idTable);
	std::list<LEXER::Lexem> makePolish(vector<LEXER::Lexem>& lexTable, LEXER::IdTable& idTable, size_t& index, bool (*stopped)(vector<LEXER::Lexem>&, size_t));
}

