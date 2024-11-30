#include "pch.h"
#include "Syntax_Analiser.h"

void ANALISER::Analiser::analise(std::vector<Lexem>& lexTable, IdTable& idTable)
{
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

	auto generateThrow = [&]()
	{
			ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", lexTable[index].line, lexTable[index].index),
					  std::format("Ожидался тип {}", types[currentType.valueType]));
			throw "Exception";
	};

	size_t currentParamType = 0;

	while (!brackets.empty() && currentParamType != countOfParams - 1)
	{
		if(lexTable[index].lexema == ")") brackets.pop();
		if(lexTable[index].lexema == ",") index++;

		size_t oldIndex = index;
		switch (returnType(lexTable, idTable, index))
		{
		case Fun:
			if(params[currentParamType] != literalTypes[idTable[lexTable[oldIndex].positionInIdTable].valueType]) generateThrow();
			checkFun(lexTable, idTable, index);
			break;
		case ServisSymbol:
			if (lexTable[index].lexema != ",")
			{
				checkExp(lexTable, idTable, index);
			}
			break;
		case Literal:
			if (params[currentParamType] != literalTypes[idTable[lexTable[oldIndex].positionInIdTable].valueType]) generateThrow();
			if (lexTable[index + 1].lexema != ",")
			{
				checkExp(lexTable, idTable, index);
			}
			break;
		case Param:
		case Variable:
			if (currentFunction.valueType != idTable[lexTable[oldIndex].positionInIdTable].valueType) generateThrow();
			if (lexTable[index + 1].lexema != ",")
			{
				checkExp(lexTable, idTable, index);
			}
			break;
		default:
			break;
		}
		index++;
		currentParamType++;
	}
}

void ANALISER::Analiser::checkExp(std::vector<Lexem>& lexTable, IdTable& idTable, size_t& index)
{

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

	while (lexTable[index].lexema != ";")
	{
		size_t oldIndex = index;
		auto j = idTable[lexTable[oldIndex].positionInIdTable].valueType;
		switch (returnType(lexTable, idTable, index))
		{
		case Fun:
			if (currentFunction.valueType != literalTypes[idTable[lexTable[oldIndex].positionInIdTable].valueType]) generateThrow();
			checkFun(lexTable, idTable, index);
			break;
		case ServisSymbol:
			if (lexTable[index].lexema != ",")
			{
				checkExp(lexTable, idTable, index);
			}
			break;
		case Literal:
			if (currentFunction.valueType != literalTypes[idTable[lexTable[oldIndex].positionInIdTable].valueType]) generateThrow();
			if (lexTable[index + 1].lexema != ",")
			{
				checkExp(lexTable, idTable, index);
			}
			break;
		case Param:
		case Variable:
			if (currentFunction.valueType != idTable[lexTable[oldIndex].positionInIdTable].valueType) generateThrow();
			if (lexTable[index + 1].lexema != ",")
			{
				checkExp(lexTable, idTable, index);
			}
			break;
		default:
			break;
		}
		index++;
	}
}
