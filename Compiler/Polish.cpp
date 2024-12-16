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

	for (auto& lexem : lexTable)
	{
		if (lexem.positionInIdTable != -1 && idTable.Contains(lexem.positionInIdTable))
		{
			idTable[lexem.positionInIdTable].isUse = true;
		}
	}

	std::list<Entry> deleteEntrys;
	for (auto key_val : idTable)
	{
		auto entry = key_val.second;
		if (!entry.isUse) 
			deleteEntrys.push_back(entry);
	}

	for(auto item : deleteEntrys) idTable.Delete(item);
}

template<typename T> void POLISH::countPolish(std::list<LEXER::Lexem>& expression, LEXER::IdTable& idTable, Keywords type)
{
	std::stack<T> results;
	std::list<LEXER::Lexem> result;

	auto count = [](T a, T b, auto& el) -> T
		{
			switch (TokenTypes.at(el.originalText))
			{
			case Plus:
				return a + b;
				break;
			case Minus:
				return a - b;
				break;
			case Star:
				return a * b;
				break;
			case Delim:
				return a / b;
				break;
			case ShiftLeft:
			{
				if (std::is_same_v<T, int>)
				{
					return (INT64)a << (int)b;
				}
				else
				{
					ERROR_LOG(std::format("Expressions_Analiser: строка {}, лексема {}.", el.line, el.index), "Оператор << недопустим для типа double");
					throw "Exception";
				}
				break;
			}
			case ShiftRight:
			{
				if (std::is_same_v<T, int>)
				{
					return (int)a >> (int)b;
				}
				else
				{
					ERROR_LOG(std::format("Expressions_Analiser: строка {}, лексема {}.", el.line, el.index), "Оператор >> недопустим для типа double");
					throw "Exception";
				}
				break;
			}
			case opDelim:
			{
				if (std::is_same_v<T, int>)
				{
					return (int)a % (int)b;
				}
				else
				{
					ERROR_LOG(std::format("Expressions_Analiser: строка {}, лексема {}.", el.line, el.index), "Оператор % недопустим для типа double");
					throw "Exception";
				}
				break;
			}
			}
		};

	auto getType = [=]() -> Keywords {
		return type;
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
			if (std::is_same_v<T, double>)
			{
				if (std::isinf((double)val) && val > 0) return;
				else if (std::isinf((double)val) && val < 0) return;
				else if (std::isnan((double)val)) return;
			}
			Entry newEntry;
			createEntry(newEntry, val);
			idTable.Add(newEntry);

			val = Pop(results);
			if (std::is_same_v<T, double>)
			{
				if (std::isinf((double)val) && val > 0) return;
				else if (std::isinf((double)val) && val < 0) return;
				else if (std::isnan((double)val)) return;
			}
			Entry newEntry1;
			createEntry(newEntry1, val);
			idTable.Add(newEntry1);
			result.push_back(Lexem("l", std::to_string(val), -1, -1, newEntry1.GetHashCode()));
			result.push_back(Lexem("l", std::to_string(val), -1, -1, newEntry.GetHashCode()));

			result.push_back(element);
		}
		else if (element.lexema == "o")
		{
			try
			{
				if (element.originalText == ">>" || element.originalText == "<<")
				{
					auto testResult{ results };
					auto num1 = Pop(testResult);
					auto num2 = Pop(testResult);
					//if()
					if (num1 >= num2 || num1 < 0)
					{
						WARN_LOG(std::format("Источник: Expressions_Analiser. Строка: {}, Столбец: {}", element.line, element.index), "Значениие сдвига слишком большое или отрицательное. Неопределённое поведение");
					}
				}
				results.push(count(Pop(results), Pop(results), element));
			}
			catch (...)
			{
				ERROR_LOG("Expressions_Analiser",
						  "Недопустимая математическая операция");
				throw "Esception";
			}
		}
		else
		{
			results.push(std::get<T>(idTable[element.positionInIdTable].value));
		}
	}

	if (result.empty())
	{
		auto val = Pop(results);
		if (std::is_same_v<T, double>)
		{
			if (std::isinf((double)val) && val > 0) return;
			else if (std::isinf((double)val) && val < 0) return;
			else if (std::isnan((double)val)) return;
		}
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
	size_t typeIndex = index;
	while (lexTable[typeIndex].positionInIdTable == -1) typeIndex++;
	auto currentType = idTable[lexTable[typeIndex].positionInIdTable];

	auto GetType = [&idTable, &optymiseFlag, &lexTable, &index, &currentType](LEXER::Lexem& lexem)->LEXER::Keywords
		{

			if (lexem.positionInIdTable == -1)
			{
				switch (lexem.lexema[0])
				{
				case 'o':
					if (lexem.originalText == "%" && (currentType.valueType == Double || currentType.valueType == DoubleLiteral))
					{
						ERROR_LOG(std::format("Expressions_Analiser: строка {}, лексема {}.", lexem.line, lexem.index), "Оператор % недопустим для типа double");
						throw "Exception";
					}
					if (lexem.originalText == "<<" && (currentType.valueType == Double || currentType.valueType == DoubleLiteral))
					{
						ERROR_LOG(std::format("Expressions_Analiser: строка {}, лексема {}.", lexem.line, lexem.index), "Оператор << недопустим для типа double");
						throw "Exception";
					}
					if (lexem.originalText == ">>" && (currentType.valueType == Double || currentType.valueType == DoubleLiteral))
					{
						ERROR_LOG(std::format("Expressions_Analiser: строка {}, лексема {}.", lexem.line, lexem.index), "Оператор << недопустим для типа double");
						throw "Exception";
					}
					if (lexTable[index - 1].lexema == "i" || lexTable[index - 1].lexema == "l")
					{

						auto left = ANALISER::Analiser::types[idTable[lexTable[index - 1].positionInIdTable].valueType];

						if ((left != "int" && left != "double" && left != "uint"))
						{
							ERROR_LOG(std::format("Expressions_Analiser: строка {}, лексема {}.", lexem.line, lexem.index), std::format("Арифметические выражения неподдерживаются для типа {}", ANALISER::Analiser::types[idTable[lexTable[index + 1].positionInIdTable].valueType]));
							throw "Exception";
						}
					}
					if (lexTable[index + 1].lexema == "i" || lexTable[index + 1].lexema == "l")
					{
						auto right = ANALISER::Analiser::types[idTable[lexTable[index + 1].positionInIdTable].valueType];

						if ((right != "int" && right != "double" && right != "uint"))
						{
							ERROR_LOG(std::format("Expressions_Analiser: строка {}, лексема {}.", lexem.line, lexem.index), std::format("Арифметические выражения неподдерживаются для типа {}", ANALISER::Analiser::types[idTable[lexTable[index + 1].positionInIdTable].valueType]));
							throw "Exception";
						}
					}
					return ServisSymbol;
					break;
				case 'u':
					if (lexem.originalText == ">" && (currentType.valueType == String || currentType.valueType == StringLiteral))
					{
						ERROR_LOG(std::format("Expressions_Analiser: строка {}, лексема {}.", lexem.line, lexem.index), "Оператор > недопустим для типа string");
						throw "Exception";
					}
					if (lexem.originalText == "<" && (currentType.valueType == String || currentType.valueType == StringLiteral))
					{
						ERROR_LOG(std::format("Expressions_Analiser: строка {}, лексема {}.", lexem.line, lexem.index), "Оператор < недопустим для типа string");
						throw "Exception";
					}
					if (lexem.originalText == ">=" && (currentType.valueType == String || currentType.valueType == StringLiteral))
					{
						ERROR_LOG(std::format("Expressions_Analiser: строка {}, лексема {}.", lexem.line, lexem.index), "Оператор >= недопустим для типа string");
						throw "Exception";
					}
					if (lexem.originalText == "<=" && (currentType.valueType == Char || currentType.valueType == CharLiteral))
					{
						ERROR_LOG(std::format("Expressions_Analiser: строка {}, лексема {}.", lexem.line, lexem.index), "Оператор <= недопустим для типа char");
						throw "Exception";
					}
					if (lexem.originalText == ">" && (currentType.valueType == Char || currentType.valueType == CharLiteral))
					{
						ERROR_LOG(std::format("Expressions_Analiser: строка {}, лексема {}.", lexem.line, lexem.index), "Оператор > недопустим для типа char");
						throw "Exception";
					}
					if (lexem.originalText == "<" && (currentType.valueType == Char || currentType.valueType == CharLiteral))
					{
						ERROR_LOG(std::format("Expressions_Analiser: строка {}, лексема {}.", lexem.line, lexem.index), "Оператор < недопустим для типа char");
						throw "Exception";
					}
					if (lexem.originalText == ">=" && (currentType.valueType == Char || currentType.valueType == CharLiteral))
					{
						ERROR_LOG(std::format("Expressions_Analiser: строка {}, лексема {}.", lexem.line, lexem.index), "Оператор >= недопустим для типа char");
						throw "Exception";
					}
					if (lexem.originalText == "<=" && (currentType.valueType == Char || currentType.valueType == CharLiteral))
					{
						ERROR_LOG(std::format("Expressions_Analiser: строка {}, лексема {}.", lexem.line, lexem.index), "Оператор <= недопустим для типа char");
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

			auto type = idTable[lexem.positionInIdTable];

			if (type.type == Fun || type.type == Variable || type.type == Param) optymiseFlag = false;
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
			operators.push(lexTable[index]);
			break;
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
				ERROR_LOG(std::format("Expressions_Analiser: строка {}, лексема {}.", lexem.line, lexem.index), "Неверный синтаксис выражения");
				throw "Exception";
			}
			while (GetType(operators.top()) != OpenParmBracket)
			{
				newPolishExpression.push_back(Pop(operators));
				if (operators.empty())
				{
					auto lexem = lexTable[index];
					ERROR_LOG(std::format("Expressions_Analiser: строка {}, лексема {}.", lexem.line, lexem.index), "Неверный синтаксис выражения");
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
				ERROR_LOG(std::format("Expressions_Analiser: строка {}, лексема {}.", lexem.line, lexem.index), "Неверный синтаксис выражения");
				throw "Exception";
			}
			while (GetType(operators.top()) != OpenParmBracket)
			{
				newPolishExpression.push_back(Pop(operators));
				if (operators.empty())
				{
					auto lexem = lexTable[index];
					ERROR_LOG(std::format("Expressions_Analiser: строка {}, лексема {}.", lexem.line, lexem.index), "Неверный синтаксис выражения");
					throw "Exception";
				}
			}
			newPolishExpression.push_back(Lexem("$", "$"));
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
			ERROR_LOG(std::format("Expressions_Analiser: строка {}, лексема {}.", lexem.line, lexem.index), "Неверный синтаксис выражения");
			throw "Exception";
		}
		else newPolishExpression.push_back(Utils::Pop(operators));
	}

	if (optymiseFlag)
	{
		if (currentType.valueType == Int || currentType.valueType == IntLiteral)
			POLISH::countPolish<int>(newPolishExpression, idTable, IntLiteral);
		else if (currentType.valueType == Double || currentType.valueType == DoubleLiteral)
			POLISH::countPolish<double>(newPolishExpression, idTable, DoubleLiteral);
		else if (currentType.valueType == UInt || currentType.valueType == UIntLiteral)
			POLISH::countPolish<unsigned int>(newPolishExpression, idTable, UIntLiteral);
	}

	return newPolishExpression;
}
