#include "pch.h"
#include "Lexer.h"
#include "Syntax_Analiser.h"
#include "Polish.h"

using namespace LEXER;
using namespace Utils;

int literalCounter = 0;

void POLISH::changeLexTable(vector<LEXER::Lexem>& lexTable, LEXER::IdTable& idTable)
{
	std::vector<LEXER::Lexem> newTable;
	newTable.reserve(lexTable.size());

	auto exeptionStop = [](vector<LEXER::Lexem>& lexTable, size_t index) -> bool
		{
			return lexTable[index].lexema == ";";
		};

	auto ifStop = [](vector<LEXER::Lexem>& lexTable, size_t index) -> bool
		{
			return lexTable[index].lexema == ")" && lexTable[index + 1].lexema == "{";
		};

	for (size_t i = 0; i < lexTable.size(); i++)
	{
		if (lexTable[i].lexema == "q" || lexTable[i].lexema == "w")
		{
			newTable.push_back(lexTable[i]);
			i++;
			newTable.push_back(lexTable[i]);
			i++;
			auto polishExpression = makePolish(lexTable, idTable, i, ifStop);
			newTable.insert(newTable.end(), polishExpression.begin(), polishExpression.end());

		}
		if (lexTable[i].lexema != "=" && lexTable[i].lexema != "r") newTable.push_back(lexTable[i]);
		else
		{
			newTable.push_back(lexTable[i]);
			i++;
			auto polishExpression = makePolish(lexTable, idTable, i, exeptionStop);
			i--;
			newTable.insert(newTable.end(), polishExpression.begin(), polishExpression.end());
		}
	}

	lexTable = newTable;

	int kk = 1;
	for (auto jj : lexTable)
	{
		if (jj.line != kk) std::cout << std::endl;
		std::cout << jj.originalText;
		kk = jj.line;
	}
}

template<typename T> void POLISH::countPolish(std::list<LEXER::Lexem>& expression, LEXER::IdTable& idTable)
{
	std::stack<T> results;
	std::list<LEXER::Lexem> result;

	auto count = [](T a, T b, auto& el) -> T
		{
			/*std::swap(a, b);*/
			switch (el.originalText[0])
			{
			case '+':
				return a + b;
				break;
			case '-':
				return a - b;
				break;
			case '*':
				return a * b;
				break;
			case '/':
				return a / b;
				break;
			case '%':
			{
				if (std::is_same_v<T, int>)
				{
					return (int)a % (int)b;
				}
				else
				{
					ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", el.line, el.index), "Оператор % недопустим для типа double");
					throw "Exception";
				}
				break;
			}
			}
		};

	auto getType = []() -> Keywords {
		return std::is_same_v<T, int> ? IntLiteral : DoubleLiteral;
		};

	auto createEntry = [&](auto& newEntry, auto& val) {
		newEntry.type = Literal;
		newEntry.name = "Literal_New_" + std::to_string(literalCounter);
		literalCounter++;
		newEntry.valueType = getType();
		newEntry.scope = "none";
		newEntry.value = val;
		};
	for (auto element : expression)
	{
		if (element.lexema == "u")
		{
			auto val = Pop(results);
			Entry newEntry;
			createEntry(newEntry, val);
			idTable.Add(newEntry);
			result.push_back(Lexem("l", std::to_string(val), -1, -1, newEntry.GetHashCode()));

			val = Pop(results);
			Entry newEntry1;
			createEntry(newEntry1, val);
			idTable.Add(newEntry1);
			result.push_back(Lexem("l", std::to_string(val), -1, -1, newEntry1.GetHashCode()));

			result.push_back(element);
		}
		else if (element.lexema == "o")
		{
			results.push(count(Pop(results), Pop(results), element));
		}
		else
		{
			results.push(std::get<T>(idTable[element.positionInIdTable].value));
		}
	}

	if (result.empty())
	{
		auto val = Pop(results);
		Entry newEntry;
		createEntry(newEntry, val);
		idTable.Add(newEntry);
		result.push_back(Lexem("l", std::to_string(val), -1, -1, newEntry.GetHashCode()));
	}

	expression = result;
}

std::list<LEXER::Lexem> POLISH::makePolish(vector<LEXER::Lexem>& lexTable, LEXER::IdTable& idTable, size_t& index, bool (*stopped)(vector<LEXER::Lexem>&, size_t))
{
	bool optymiseFlag = true;
	auto currentType = idTable[lexTable[index].positionInIdTable].valueType;

	auto GetType = [&idTable, &optymiseFlag, &lexTable, &index](LEXER::Lexem& lexem)->LEXER::Keywords
		{
			auto type = idTable[lexem.positionInIdTable];

			if (type.type == Fun || type.type == Variable || type.type == Param) optymiseFlag = false;

			if (lexem.positionInIdTable == -1)
			{
				switch (lexem.lexema[0])
				{
				case 'o':
					if (lexem.originalText == "%" && (type.valueType == Double || type.valueType == DoubleLiteral))
					{
						ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", lexem.line, lexem.index), "Оператор % недопустим для типа double");
						throw "Exception";
					}
					if (lexTable[index - 1].lexema == "i" || lexTable[index - 1].lexema == "l")
					{
						/*auto right = ANALISER::Analiser::types[idTable[lexTable[index + 1].positionInIdTable].valueType];*/
						auto left = ANALISER::Analiser::types[idTable[lexTable[index - 1].positionInIdTable].valueType];

						if ((left != "int" && left != "double") /*|| (right != "int" || right != "double")*/)
						{
							ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", lexem.line, lexem.index), std::format("Арифметические выражения неподдерживаются для типа {}", ANALISER::Analiser::types[type.valueType]));
							throw "Exception";
						}
					}
					if (lexTable[index + 1].lexema == "i" || lexTable[index + 1].lexema == "l")
					{
						auto right = ANALISER::Analiser::types[idTable[lexTable[index + 1].positionInIdTable].valueType];

						if ((right != "int" && right != "double"))
						{
							ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", lexem.line, lexem.index), std::format("Арифметические выражения неподдерживаются для типа {}", ANALISER::Analiser::types[type.valueType]));
							throw "Exception";
						}
					}
					return ServisSymbol;
					break;
				case 'u':
					if (lexem.originalText == "%" && (type.valueType == Double || type.valueType == DoubleLiteral))
					{
						ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", lexem.line, lexem.index), "Оператор % недопустим для типа double");
						throw "Exception";
					}
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

			/*if (type.valueType != Int &&
				type.valueType != IntLiteral &&
				type.valueType != Double &&
				type.valueType != DoubleLiteral &&
				type.type != Param &&
				lexTable[index + 1].lexema != ";" &&
				lexTable[index + 1].lexema != "u" &&
				lexTable[index + 1].lexema != ")")
			{
				ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", lexem.line, lexem.index), std::format("Арифметические выражения неподдерживаются для типа {}", ANALISER::Analiser::types[type.valueType]));
				throw "Exception";
			}*/
			return type.type;
		};

	std::list<LEXER::Lexem> newPolishExpression;
	std::stack< LEXER::Lexem> operators;

	while (!stopped(lexTable, index))
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
					   operators.top().prioryty <= lexTable[index].prioryty)
				{
					newPolishExpression.push_back(Pop(operators));
					if (operators.empty()) break;
				}
			}
			operators.push(lexTable[index]);
			break;
		}
		case CloseParmBracket:
		{
			if (operators.empty())
			{
				auto lexem = lexTable[index];
				ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", lexem.line, lexem.index), "Неверный синтаксис выражения");
				throw "Exception";
			}
			while (GetType(operators.top()) != OpenParmBracket)
			{
				newPolishExpression.push_back(Pop(operators));
				if (operators.empty())
				{
					auto lexem = lexTable[index];
					ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", lexem.line, lexem.index), "Неверный синтаксис выражения");
					throw "Exception";
				}
			}

			operators.pop();

			if (!operators.empty() && GetType(operators.top()) == Fun)
				newPolishExpression.push_back(Pop(operators));
			break;
		}
		case Comma:
		{
			if (operators.empty())
			{

				auto lexem = lexTable[index];
				ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", lexem.line, lexem.index), "Неверный синтаксис выражения");
				throw "Exception";
			}
			while (GetType(operators.top()) != OpenParmBracket)
			{
				newPolishExpression.push_back(Pop(operators));
				if (operators.empty())
				{
					auto lexem = lexTable[index];
					ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", lexem.line, lexem.index), "Неверный синтаксис выражения");
					throw "Exception";
				}
			}
			break;
		}
		default:
			break;
		}
		index++;
	}

	while (!operators.empty())
	{
		if (GetType(operators.top()) == OpenParmBracket)
		{
			auto lexem = lexTable[index];
			ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", lexem.line, lexem.index), "Неверный синтаксис выражения");
			throw "Exception";
		}
		else newPolishExpression.push_back(Utils::Pop(operators));
	}

	for (auto j : newPolishExpression) std::cout << j.originalText;

	std::cout << std::endl;

	if (optymiseFlag)
	{
		if (currentType == Int || currentType == IntLiteral)
			POLISH::countPolish<int>(newPolishExpression, idTable);
		else if (currentType == Double || currentType == DoubleLiteral)
			POLISH::countPolish<double>(newPolishExpression, idTable);
	}

	return newPolishExpression;
}
