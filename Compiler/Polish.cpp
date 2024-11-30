#include "pch.h"
#include "Lexer.h"
#include "Polish.h"

using namespace LEXER;


void POLISH::changeLexTable(vector<LEXER::Lexem>& lexTable, LEXER::IdTable& idTable)
{	
	std::vector<LEXER::Lexem> newTable;
	newTable.reserve(lexTable.size());

	for (size_t i = 0; i < lexTable.size(); i++)
	{
		if(lexTable[i].lexema != "=") newTable.push_back(lexTable[i]);
		else
		{
			newTable.push_back(lexTable[i]);
			i++;
			auto polishExpression = makePolish(lexTable, idTable, i);
			newTable.insert(newTable.end(), polishExpression.begin(), polishExpression.end());
		}
	}

	lexTable = newTable;



	int kk = 1;
	for (auto jj : lexTable)
	{
		if (jj.line != kk) std::cout << std::endl;
		std::cout << jj.lexema;
		kk = jj.line;
	}
}

std::list<LEXER::Lexem> POLISH::makePolish(vector<LEXER::Lexem>& lexTable, LEXER::IdTable& idTable, size_t& index)
{
	auto GetType = [&idTable](LEXER::Lexem& lexem)->LEXER::Keywords
	{
			if (lexem.positionInIdTable == -1)
			{
				switch (lexem.lexema[0])
				{
				case 'o':
					return ServisSymbol;
					break;
				case ',':
					return Comma;
					break;
				case '(':
					return OpenParmBracket;
					break;
				case ')':
					return CloseParmBracket;
					break;
				default:
					break;
				}
			}
			return idTable[lexem.positionInIdTable].type;
	};

	std::list<LEXER::Lexem> newPolishExpression;
	std::stack< LEXER::Lexem> operators;

	while (lexTable[index].lexema != ";")
	{
		switch (GetType(lexTable[index]))
		{
		case Literal:
		case Param:
		case Variable:
			newPolishExpression.push_back(lexTable[index]);
			break;
		case Fun:
		case OpenParmBracket:
			operators.push(lexTable[index]);
			break;
		case ServisSymbol:
		{
			if (!operators.empty())
			{
				while (GetType(operators.top()) == ServisSymbol &&
					   operators.top().prioryty >= lexTable[index].prioryty)
				{
					newPolishExpression.push_back(Utils::Pop(operators));
					if (operators.empty()) break;
				}
			}
			operators.push(lexTable[index]);
			break;
		}
		case CloseParmBracket:
		{
			if (operators.empty()) throw;
			while (GetType(operators.top()) != OpenParmBracket)
			{
				newPolishExpression.push_back(Utils::Pop(operators));
				if (operators.empty()) throw;
			}

			operators.pop();

			if (!operators.empty() && GetType(operators.top()) == Fun)
				newPolishExpression.push_back(Utils::Pop(operators));
			break;
		}
		case Comma:
		{
			if (operators.empty()) throw;
			while (GetType(operators.top()) != OpenParmBracket)
			{
				newPolishExpression.push_back(Utils::Pop(operators));
				if (operators.empty()) throw;
			}
			break;
		}
		default:
			break;
		}
		index++;
	}

	index--;

	while (!operators.empty())
	{
		if (GetType(operators.top()) == OpenParmBracket)
		{
			throw;
		}
		else newPolishExpression.push_back(Utils::Pop(operators));
	}

	return newPolishExpression;
}
