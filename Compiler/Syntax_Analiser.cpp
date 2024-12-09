#include "pch.h"
#include "Syntax_Analiser.h"

void ANALISER::Analiser::analise(std::vector<Lexem>& lexTable, IdTable& idTable)
{
	size_t leftStart = 0;
	size_t leftEnd = 0;
	size_t rightStart = 0;
	size_t rightEnd = 0;

	for (size_t i = 0; i < lexTable.size(); i++)
	{
		auto currentLexem = lexTable[i];
		switch (currentLexem.lexema[0])
		{
		case 'f':
			setCurrentFunction(lexTable, idTable, i);
			break;
		case 'r':
			checkReturnType(lexTable, idTable, i);
			break;
		case 'i':
		case 'l':
			checkExp(lexTable, idTable, i, idTable[lexTable[i].positionInIdTable]);
			break;
		case 'w':
		case 'q':
		{
			i++;
			checkBlockRange(lexTable, idTable, i, leftStart, leftEnd, rightStart, rightEnd);
			size_t currentType = leftStart;
			rightStart--;
			checkExp(lexTable, idTable, i, idTable[lexTable[currentType].positionInIdTable], lexTable[leftEnd + 1].lexema[0]);
			checkExp(lexTable, idTable, rightStart, idTable[lexTable[currentType].positionInIdTable], lexTable[rightEnd].lexema[0]);
			i = rightStart;

			leftStart = 0;
			leftEnd = 0;
			rightStart = 0;
			rightEnd = 0;
		}
		break;
		default:
			break;
		}
	}
}

void ANALISER::Analiser::setCurrentFunction(std::vector<Lexem>& lexTable, IdTable& idTable, size_t& index)
{
	index += 2;
	this->currentFunction = idTable[lexTable[index].positionInIdTable];
	if (currentFunction.name == "main" && currentFunction.valueType != Int)
	{
		ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", currentFunction.line, currentFunction.pos),
				  "Недопустимая тип функции main");
		throw "Exception";
	}
	while(lexTable[index].lexema != "{") index++;
}

Keywords ANALISER::Analiser::returnType(std::vector<Lexem>& lexTable, IdTable& idTable, size_t& index)
{
	if(lexTable[index].positionInIdTable == -1) return ServisSymbol;
	auto currentLex = idTable[lexTable[index].positionInIdTable];

	switch (currentLex.type)
	{
	case Variable:
	case Param:
	case Literal:
		return currentLex.type;
		break;
	case Fun:
		checkFun(lexTable, idTable, index);
		return currentLex.type;
		break;
	default:
		return ServisSymbol;
		break;
	}
}

void ANALISER::Analiser::checkFun(std::vector<Lexem>& lexTable, IdTable& idTable, size_t& index)
{
	auto currentType = idTable[lexTable[index].positionInIdTable];
	index++;

	auto params = currentType.paramTypes;
	size_t countOfParams = currentType.params;

	std::stack<char> brackets;
	brackets.push(lexTable[index].lexema[0]);
	index++;

	size_t currentParamType = 0;

	auto generateThrow = [&]()
	{
			ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", lexTable[index].line, lexTable[index].index),
					  std::format("Ожидался тип {}", types[params[currentParamType]]));
			throw "Exception";
	};


	while (!brackets.empty())
	{
		if (lexTable[index].lexema == ")")
		{
			brackets.pop();
			if (brackets.empty() && currentParamType != countOfParams - 1 && currentParamType != 0 && countOfParams != 0)
			{
				ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", lexTable[index].line, lexTable[index].index),
						  std::format("Неверное кол-во параметров функции {}, ожидается {}", currentType.name, currentType.params));
				throw "Exception";
			}
			continue;
		}
		if (lexTable[index].lexema == ";")
		{
			if (currentParamType != countOfParams - 1)
			{
				ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", lexTable[index].line, lexTable[index].index),
						  std::format("Неверное кол-во параметров функции {}, ожидается {}", currentType.name, currentType.params));
				throw "Exception";
			}
			break;
		}
		if (lexTable[index].lexema == ",")
		{
			index++;
			currentParamType++;
			if (currentParamType >= countOfParams)
			{
				ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", lexTable[index].line, lexTable[index].index),
						  std::format("Неверное кол-во параметров функции {}, ожидается {}", currentType.name, currentType.params));
				throw "Exception";
			}
		}

		size_t oldIndex = index;
		switch (returnType(lexTable, idTable, index))
		{
		case Fun:
			if(params[currentParamType] != literalTypes[idTable[lexTable[oldIndex].positionInIdTable].valueType]) generateThrow();
			break;
		case ServisSymbol:
			if (lexTable[index].lexema == "," || lexTable[index].lexema == ")" )
			{
				currentParamType++;
			}
			else if (lexTable[index].lexema == "(" && lexTable[index - 1].lexema == "(")
			{
				while(lexTable[index].lexema == "(") index++;
				checkExp(lexTable, idTable, index, idTable[lexTable[index].positionInIdTable], END_BRACKET);
			}
			break;
		case Literal:
			if (params[currentParamType] != literalTypes[idTable[lexTable[oldIndex].positionInIdTable].valueType]) generateThrow();
			if (lexTable[index + 1].lexema != "," && lexTable[index + 1].lexema != ")")
			{
				char endSymbol;
				if (countOfParams > 0 && currentParamType != countOfParams - 1)
				{
					endSymbol = ',';
				}
				else
				{
					endSymbol = ')';
				}
				checkExp(lexTable, idTable, index, idTable[lexTable[index].positionInIdTable], endSymbol);
				if (endSymbol == ',') currentParamType++;
			}
			break;
		case Param:
		case Variable:
			//std::cout << types[currentFunction.valueType] << " " << types[idTable[lexTable[oldIndex].positionInIdTable].valueType] << std::endl;
			if (params[currentParamType] != idTable[lexTable[oldIndex].positionInIdTable].valueType) generateThrow();
			if (lexTable[index + 1].lexema != "," && lexTable[index + 1].lexema != ")")
			{
				char endSymbol;
				if (countOfParams > 0 && currentParamType != countOfParams - 1)
				{
					endSymbol = ',';
				}
				else
				{
					endSymbol = ')';
				}
				checkExp(lexTable, idTable, index, idTable[lexTable[index].positionInIdTable], endSymbol);
				if(endSymbol == ',') currentParamType++;
			}
			break;
		default:
			break;
		}
		index++;
	}
}

void ANALISER::Analiser::checkExp(std::vector<Lexem>& lexTable, IdTable& idTable, size_t& index, Entry& currentType, char endSymbol)
{
	auto generateThrow = [&]()
	{
			ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", lexTable[index].line, lexTable[index].index),
					  std::format("Ожидался тип {}", types[currentType.valueType]));
			throw "Exception";
	}; 
	//index++;

	while (lexTable[index].lexema[0] != endSymbol)
	{
		size_t oldIndex = index;
		
		switch (returnType(lexTable, idTable, index))
		{
		case Fun:
			if (literalTypes[currentType.valueType] != idTable[lexTable[oldIndex].positionInIdTable].valueType) generateThrow();
			break;
		case ServisSymbol:
			break;
		case Literal:
			if (literalTypes[currentType.valueType] != literalTypes[idTable[lexTable[oldIndex].positionInIdTable].valueType]) generateThrow();
			break;
		case Param:
		case Variable:
			if (literalTypes[currentType.valueType] != idTable[lexTable[oldIndex].positionInIdTable].valueType) generateThrow();
			break;
		default:
			break;
		}
		if (lexTable[index].lexema[0] == endSymbol) break;
		index++;
	}
}

void ANALISER::Analiser::checkReturnType(std::vector<Lexem>& lexTable, IdTable& idTable, size_t& index)
{
	auto generateThrow = [&]()
	{
			ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", lexTable[index].line, lexTable[index].index),
					  std::format("Ожидался тип {}", types[currentFunction.valueType]));
			throw "Exception";
	};

	index++;
	if (currentFunction.name == "main" && 
		(idTable[lexTable[index].positionInIdTable].valueType != IntLiteral ||
		std::get<int>(idTable[lexTable[index].positionInIdTable].value) != 0))
	{
		ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", lexTable[index].line, lexTable[index].index),
				  std::format("Функция main должна возвращать 0"));
		throw "Exception";
	}
	else
	{
		while (lexTable[index].lexema != ";")
		{
			size_t oldIndex = index;
			
			switch (returnType(lexTable, idTable, index))
			{
			case Fun:
				if (currentFunction.valueType != literalTypes[idTable[lexTable[oldIndex].positionInIdTable].valueType]) generateThrow();
				//checkFun(lexTable, idTable, index);
				break;
			case ServisSymbol:
				if (lexTable[index].lexema != ";")
				{
					checkExp(lexTable, idTable, index, currentFunction);
				}
				break;
			case Literal:
				if (currentFunction.valueType != literalTypes[idTable[lexTable[oldIndex].positionInIdTable].valueType]) generateThrow();
				if (lexTable[index + 1].lexema != ";")
				{
					checkExp(lexTable, idTable, index, currentFunction);
				}
				break;
			case Param:
			case Variable:
				if (currentFunction.valueType != idTable[lexTable[oldIndex].positionInIdTable].valueType) generateThrow();
				if (lexTable[index + 1].lexema != ";")
				{
					checkExp(lexTable, idTable, index, currentFunction);
				}
				break;
			default:
				break;
			}
			if(lexTable[index].lexema == ";") break;
			index++;
		}
	}
}

void ANALISER::Analiser::checkBlockRange(std::vector<Lexem>& lexTable, IdTable& idTable, size_t index, size_t& leftStart,  size_t& leftEnd, size_t& rightStart, size_t& rightEnd)
{
	bool rightLeftFlag = false;
	bool leftFlag = false;

	while (lexTable[index].lexema != "{")
	{
		if (!leftFlag)
		{
			if (lexTable[index].lexema == "l" || lexTable[index].lexema == "i")
			{
				leftStart = index;
				leftFlag = true;
			}
		}
		if (lexTable[index].lexema == "u")
		{
			rightLeftFlag = true;
			rightStart = index + 1;
			index++;
			continue;
		}
		if (!rightLeftFlag)
		{
			leftEnd = index;
		}
		else
		{
			rightEnd = index;
		}
		index++;
	}
}
