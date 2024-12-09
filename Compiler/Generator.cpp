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

		this->idTable =idTable;
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

		code.push_back(PROC);
		code.push_back(CODE);

		for (size_t i = index; i < lexTable.size(); i++)
		{
			switch (lexTable[i].lexema[0])
			{
			case 'f':
				functionFlag = true;
				i += 2;
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
					GenerateExpression(i);
				}
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

					if(lexTable[index].positionInIdTable == entry.first)
						checkGlobalVar = true;
						index++;
				}while(!checkGlobalVar);

				auto newVar = MAKE_VAR(MAKE_NAME(entry.second), types[entry.second.valueType], MAKE_VAL(idTable[lexTable[index + 1].positionInIdTable].GetValue()));

				code.push_back(newVar + "\n");
				index = 0;
			}
		}

		while(lexTable[index].lexema != "f") index++;
	}

	void Generator::GenerateFunction(size_t& index)
	{
		int bytes = 0;
		std::string params;

		auto fun = idTable[lexTable[index].positionInIdTable];
		index++;

		while (lexTable[index].lexema != "{")
		{
			if (lexTable[index].lexema == "i")
			{
				auto param = idTable[lexTable[index].positionInIdTable];
				params += MAKE_PARAM(MAKE_NAME(param), types[param.valueType]);
			}
			index++;
		}

		if(!params.empty()) params.erase(params.length() - 2);

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

	void Generator::GenerateExpression(size_t& index)
	{
		auto var = idTable[lexTable[index - 2].positionInIdTable];
		std::list<std::string> expression;

		auto count = [&](auto operators, bool isDouble = false) 
			{
				while (lexTable[index].originalText != ";")
				{
					if (lexTable[index].lexema != "o" && lexTable[index].lexema != "u")
					{
						auto var = idTable[lexTable[index].positionInIdTable];
						expression.push_back(PUSH(MAKE_NAME(var), isDouble));
					}
					else if (lexTable[index].lexema == "o")
					{
						expression.push_back(operators.at(lexTable[index].originalText));
					}
					index++;
				}

				expression.push_back(POP(MAKE_NAME(var), isDouble));
			};

		if (var.valueType != Double)
		{
			count(operatorsInt);
		}
		else
		{
			count(operatorsDouble, IS_DOUBLE);
		}

		auto it = code.end();
		std::advance(it, -1);
		code.insert(it, expression.begin(), expression.end());
	}

	void Generator::GenerateLockalVars(LEXER::Entry& fun)
	{
		std::string vars;


		for (const auto& entry : idTable)
		{
			if (entry.second.scope.find(fun.ownScope) == 0)
			{
				vars += MAKE_PARAM(MAKE_NAME(entry.second), types[entry.second.valueType]);
			}
		}
		if (!vars.empty()) vars.erase(vars.length() - 2);

		auto it = code.end();
		std::advance(it, -1);
		auto newBlock = MAKE_LOCALS(vars);
		code.insert(it, newBlock.begin(), newBlock.end());
	}
}