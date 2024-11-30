#pragma once

namespace POLISH
{
	void changeLexTable(vector<LEXER::Lexem>& lexTable, LEXER::IdTable& idTable);
	std::list<LEXER::Lexem> makePolish(vector<LEXER::Lexem>& lexTable, LEXER::IdTable& idTable, size_t& index);
}

