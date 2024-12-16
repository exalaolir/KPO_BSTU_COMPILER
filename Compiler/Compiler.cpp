#include "pch.h"
#include "InputValidator.h"
#include "Lexer.h"
#include "Magazine_Automate.h"
#include "Polish.h"
#include "Syntax_Analiser.h"
#include "Generator.h"


using namespace bprinter;

using namespace INPUTVAL;
using namespace LEXER;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru");
 	setlocale(LC_NUMERIC, "C");

	try
	{
		auto params = validateParams(argc, argv);
		Lexer lexer(params[IN_FILE]);

		auto log = std::make_shared<std::ofstream>(params[PROTOCOL_FILE]);

		INFO_LOG("Источник: Lexer", "Начато формирование таблицы лексем");
		auto lexTable = lexer.generateLexTable();
		lexer.printLexTable(lexTable, *log);
		SUCCESS_LOG("Источник: Lexer", "Формирование таблицы лексем завершено успешно");

		INFO_LOG("Источник: Lexer", "Начато формирование таблицы индентификаторов");
		auto idTable = lexer.generateIdTable(lexTable);
		*log << "Таблица идентификаторов" << std::endl;
		lexer.printIdTable(idTable, *log);
		SUCCESS_LOG("Источник: Lexer", "Формирование таблицы индентификаторов завершено успешно");

		INFO_LOG("Источник: Parser", "Начата проверка синтаксиса");
		MFST_TRACE_START(log);
		MFST::Mfst mfst(lexTable, idTable, GRB::getGreibach());
		if (!mfst.start(log))
			throw "Error";
		mfst.savededucation();
		mfst.printrules(log);
		SUCCESS_LOG("Источник: Parser", "Проверка синтаксиса завершена успешно");

		INFO_LOG("Источник: Semantic_analiser", "Начата проверка семантики");
		ANALISER::Analiser analiser;
		analiser.analise(lexTable, idTable);
		SUCCESS_LOG("Источник: Semantic_analiser", "Проверка семантики завершена успешно");

		*log << "Изменённые данные" << std::endl;
		INFO_LOG("Источник: Expressions_Analiser", "Начата преобразование таблицы лексем и оптимизация выражений");
		POLISH::changeLexTable(lexTable, idTable);
		lexer.printLexTable(lexTable, *log);
		*log << "Изменённая таблица идентификаторов" << std::endl;
		lexer.printIdTable(idTable, *log);
		SUCCESS_LOG("Источник: Expressions_Analiser", "Преобразование таблицы лексем и оптимизация выражений завершено успешно");

		INFO_LOG("Источник: Generator", "Начата генерация кода");
		GEN::Generator gen(params[OUT_FILE], lexTable, idTable);
		gen.Generate();
		SUCCESS_LOG("Источник: Generator", "Генерация кода завершена успечно");

		system("C:\\labs\\Course\\Compiler\\Release\\CompilationOfGenCode.bat");
	}
	catch (const std::out_of_range& ex)
	{
		std::cout << ex.what();
	}
	catch (...)
	{
		
	}
}

