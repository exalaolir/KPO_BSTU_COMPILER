#include "pch.h"
#include "Generator.h"
#include "Constructions.h"

namespace GEN
{
	Generator::Generator(std::string path, 
						 std::vector<Lexem> lexTable,
						 IdTable idTable)
	{
		assembly.open(path, std::ios::out);
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

		assembly << BEGIN_OF_FILE;

		GenerateDataSection(index);

		for (size_t i = index; i < lexTable.size(); i++)
		{

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
					}

					//if()
				}while(!checkGlobalVar);

			}
		}
	}
}