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

		auto log = std::make_shared<std::ofstream>("translationInfo.txt.log");
		auto lexTable = lexer.generateLexTable();
		lexer.printLexTable(lexTable, *log);
		auto idTable = lexer.generateIdTable(lexTable);
		lexer.printIdTable(idTable, *log);
		
		std::cout << "---------------------------начало синтаксического анализатора-------------------------" << std::endl;
		MFST_TRACE_START(log);
		MFST::Mfst mfst(lexTable, idTable, GRB::getGreibach());
		if (!mfst.start(log))
			throw "Error";
		mfst.savededucation();
		mfst.printrules(log);
		std::cout << "---------------------------конец синтаксического анализатора-------------------------" << std::endl;
		ANALISER::Analiser analiser;
		analiser.analise(lexTable, idTable);

		POLISH::changeLexTable(lexTable, idTable);

		std::cout << std::endl;
		std::cout << std::endl;

		GEN::Generator gen(params[OUT_FILE], lexTable, idTable);
		gen.Generate();

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