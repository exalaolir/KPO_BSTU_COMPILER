#include "pch.h"
#include "Generator.h"
#include "Constructions.h"

namespace GEN
{
	Generator::Generator(std::string path,
						 std::vector<Lexem> lexTable,
						 IdTable idTable)
	{
		assembly.open(path, std::ios::out | std::ios::binary);
		if (!assembly.is_open())
		{
			FATAL_LOG(std::format("Файл {}", path), "Неудаётся открыть файл");
			throw "Ex";
		}

		this->idTable = idTable;
		this->lexTable = lexTable;
	}

	Generator::~Generator()
	{
		if (assembly.is_open())
		{
			assembly.close();
		}
	}

	void Generator::Generate()
	{
		size_t index = 0;

		code.push_back(BEGIN_OF_FILE);

		code.push_back(DATA + "\n");
		code.push_back(MAKE_VAR("real_buff", types[LEXER::Double], "0.0") + "\n");

		for (size_t i = 0; i < lexTable.size(); i++)
		{
			if (lexTable[i].lexema == "l")
			{
				auto entry = idTable[lexTable[i].positionInIdTable];
				auto newVar = MAKE_VAR(MAKE_NAME(entry), types[entry.valueType], MAKE_VAL(entry.GetValue()));

				code.push_back(newVar + "\n");
			}
		}

		GenerateDataSection(index);

		code.push_back(CODE);
		code.push_back(PROC);

		for (size_t i = index; i < lexTable.size(); i++)
		{
			switch (lexTable[i].lexema[0])
			{
			case 'f':
				functionFlag = true;
				i += 2;
				this->currentFunc = idTable[lexTable[i].positionInIdTable];
				if (lexTable[i].lexema == "m")
				{
					GenerateMain(i);
				}
				else GenerateFunction(i);
				break;
			case 'i':
				i++;
				if (lexTable[i].originalText == "=" && functionFlag)
				{
					i++;
					GenerateExpression(code, i);
				}
				break;
			case 'r':
				i++;
				GenerateExpression(code, i, true);
				break;
			case 'q':
				i+=2;
				GenerateIf(code, i, ifCounter);
				break;
			default:
				break;
			}
		}

		for (const auto& block : code)
		{
			assembly << block;
		}
	}

	void Generator::GenerateDataSection(size_t& index)
	{
		for (const auto& entry : idTable)
		{
			if (entry.second.type == Variable && entry.second.scope == "g0")
			{
				bool checkGlobalVar = false;

				do
				{
					while (lexTable[index].lexema != "i")
					{
						if (lexTable[index].lexema == "f")
						{
							FATAL_LOG(std::format("Source code: строка {}, столбец {}", entry.second.line, entry.second.pos), "Внутренняя ошибка. Неудалось сопоставить глобальную переменную");
							throw "Ex";
						}
						index++;
					}

					if (lexTable[index].positionInIdTable == entry.first)
						checkGlobalVar = true;
					index++;
				} while (!checkGlobalVar);

				auto newVar = MAKE_VAR(MAKE_NAME(entry.second), types[entry.second.valueType], MAKE_VAL(idTable[lexTable[index + 1].positionInIdTable].GetValue()));

				code.push_back(newVar + "\n");
				index = 0;
			}
		}

		while (lexTable[index].lexema != "f") index++;
	}

	void Generator::GenerateFunction(size_t& index)
	{
		int bytes = 0;
		std::string params ="";

		auto fun = idTable[lexTable[index].positionInIdTable];
		index++;

		while (lexTable[index].lexema != "{")
		{
			if (lexTable[index].lexema == "i")
			{
				auto param = idTable[lexTable[index].positionInIdTable];
				params.insert(0, MAKE_PARAM(MAKE_NAME(param), types[param.valueType], param.valueType));
			}
			index++;
		}

		if (!params.empty())
			params.erase(params.length() - 2);
		
		auto it = code.end();
		auto newBlock = MAKE_FUN(MAKE_NAME(fun), params);
		code.insert(it, newBlock.begin(), newBlock.end());

		GenerateLockalVars(fun);
	}

	void Generator::GenerateMain(size_t& index)
	{
		int bytes = 0;
		auto fun = idTable[lexTable[index].positionInIdTable];
		index++;

		auto it = code.end();
		auto newBlock = MAKE_MAIN(MAKE_NAME(fun));
		code.insert(it, newBlock.begin(), newBlock.end());
		GenerateLockalVars(fun);
	}

	void Generator::GenerateExpression(std::list<std::string>& code, size_t& index, bool isReturn, size_t ind, char end, int iterRebase)
	{
		LEXER::Entry var;
		if (isReturn)
		{
			var = Entry();
			var.valueType = currentFunc.valueType;
		}
		else var = idTable[lexTable[index - ind].positionInIdTable];

		std::list<std::string> expression;
		bool doubleFlag = false;
		bool strFlag = false;
		LEXER::Entry currentFun = var;

		auto count = [&](auto operators, bool isDouble = false, bool isReturn = false, string end = ";", bool isString = false)
			{
				while (lexTable[index].originalText != end)
				{
					if (lexTable[index].positionInIdTable != -1)
						currentFun = idTable[lexTable[index].positionInIdTable];
					else currentFun = var;

					if (lexTable[index].originalText == "$")
					{
						index++;
						continue;
					}

					if (currentFun.type == LEXER::Fun)
					{
						expression.push_back(CALL(MAKE_NAME(currentFun), isDouble));
					}
					else if (lexTable[index].lexema != "o" && lexTable[index].lexema != "u")
					{
						auto var = idTable[lexTable[index].positionInIdTable];

						if (var.valueType == LEXER::String || var.valueType == LEXER::StringLiteral || isDouble)
						{
							strFlag = true;
						}
						else strFlag = false;

						if (var.valueType == LEXER::Double || var.valueType == LEXER::DoubleLiteral || isDouble)
						{
							doubleFlag = true;
						}
						else doubleFlag = false;

						if(currentFun.valueType != LEXER::Double && currentFun.valueType != LEXER::DoubleLiteral) doubleFlag = false;

						expression.push_back(PUSH(MAKE_NAME(var), doubleFlag, strFlag));

						if ((lexTable[index + 1].originalText == "$" || (lexTable[index + 1].lexema == "i" && currentFun.type == LEXER::Fun)) && doubleFlag)
						{
							expression.push_back(PUSH_REAL_PARAM);
						}
					}
					else if (lexTable[index].lexema == "o")
					{
						if (doubleFlag)
						{
							expression.push_back(operatorsDouble.at(lexTable[index].originalText));
						}
						else expression.push_back(operators.at(lexTable[index].originalText));

						if ((lexTable[index + 1].originalText == "$" || (lexTable[index + 1].lexema == "i" && currentFun.type == LEXER::Fun)) && doubleFlag)
						{
							expression.push_back(PUSH_REAL_PARAM);
						}
					}
					else if (lexTable[index].lexema == "u")
					{
						if(var.valueType == Double || var.valueType == DoubleLiteral) realIf = true;
						return;
					}
					index++;
				}

				expression.push_back(POP(MAKE_NAME(var), isDouble, isReturn, isString, copyEqalGenerator));
				if(isString) copyEqalGenerator++;
			};

		if (var.valueType == String || var.valueType == StringLiteral)
		{
			count(operatorsInt, false, isReturn, string() + end, true);
		}
		else if (var.valueType != Double)
		{
			count(operatorsInt, false, isReturn, string() + end);
		}
		else
		{
			count(operatorsDouble, IS_DOUBLE, isReturn, string() + end);
		}

		auto it = code.end();
		std::advance(it, iterRebase);
		code.insert(it, expression.begin(), expression.end());
	}

	void Generator::GenerateLockalVars(LEXER::Entry& fun)
	{
		std::string vars;
		std::list<std::string> copyes;

		for (const auto& entry : idTable)
		{
			bool isLocalStr = (entry.second.valueType == String || entry.second.valueType == StringLiteral);
			if (entry.second.scope.find(fun.ownScope) == 0 && entry.second.type == Variable)
			{
				vars += MAKE_PARAM(MAKE_NAME(entry.second), types[entry.second.valueType], None, isLocalStr);
			}
			if (entry.second.scope.find(fun.ownScope) == 0 && entry.second.type == Param && entry.second.valueType == String)
			{
				vars += MAKE_PARAM(MAKE_NAME(entry.second), types[entry.second.valueType], None, true);
				copyes.push_back(MAKE_COPY(MAKE_NAME(entry.second) + "_S", MAKE_NAME(entry.second), this->copyEqalGenerator, true));
				copyEqalGenerator++;
			}
		}
		if (!vars.empty())
		{
			vars.erase(vars.length() - 2);
			auto it = code.end();
			std::advance(it, -1);
			auto newBlock = MAKE_LOCALS(vars);
			code.insert(it, newBlock.begin(), newBlock.end());
			code.insert(it, copyes.begin(), copyes.end());
		}
	}

	void Generator::GenerateIf(std::list<std::string>& code, size_t& index, size_t localIfCounter, bool rebase)
	{
		ifCounter++;
		std::list<std::string> block;
		GenerateExpression(block, index, false, 0, ')', 0);

		block.push_back(MAKE_IF(lexTable[index].originalText,
					   MAKE_BOOKMARK(If, localIfCounter), MAKE_BOOKMARK(Else, localIfCounter), realIf));
		index += 2;
		bool isWork = true;
		bool elseExist = false;
		realIf = false;
		while (isWork)
		{
			switch (lexTable[index].lexema[0])
			{
			case 'i':
				index++;
				if (lexTable[index].originalText == "=" && functionFlag)
				{
					index++;
					GenerateExpression(block, index, false, 2, ';', 0);
				}
				break;
			case 'r':
				index++;
				GenerateExpression(block, index, true, 2, ';', 0);
				break;
			case 'q':
				index += 2;
				GenerateIf(block, index, ifCounter, false);
				index--;
				break;
			case '}':
				if (!elseExist)
				{
					block.push_back(MAKE_END_MARK(If, localIfCounter));
					elseExist = true;
				}
				if (lexTable[index + 1].lexema != "s")
				{
					isWork = false;
					block.push_back(MAKE_ENDIF(If, localIfCounter));
				}
				break;
			default:
				break;
			}
			index++;
		}

		auto it = code.end();
		if(rebase) std::advance(it, -1);
		code.insert(it, block.begin(), block.end());
	}
}