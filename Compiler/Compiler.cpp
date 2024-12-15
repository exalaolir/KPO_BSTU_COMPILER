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
		/*TablePrinter tp(&std::cout);
		tp.set_flush_left();
		tp.AddColumn("Name", 25);
		tp.AddColumn("Age", 25);
		tp.AddColumn("Position", 25);

		tp.PrintHeader();
		tp << "Dat Chu" << 25 << "Research Assistant";
		tp << "John Doe" << 26 << "Professional Anonymity";
		tp << "Jane Doe" << 26 << "Professional Anonymity";
		tp << "Tom Doe" << 7 << "Student";
		tp.PrintFooter();*/

		
		auto params = validateParams(argc, argv);
		Lexer lexer(params[IN_FILE]);

		auto log = std::make_shared<std::ofstream>("translationInfo.txt.log");

		auto lexTable = lexer.generateLexTable();
		lexer.printLexTable(lexTable, *log);
		auto idTable = lexer.generateIdTable(lexTable);
		lexer.printIdTable(idTable, *log);

		/*for (auto i : idTable.keys)
		{
			auto q = idTable.table[i];
			std::cout << kl[q.type] << " " << q.name << " " << kl[q.valueType] << " " 
				<< q.scope << " " << q.ownScope << " " << q.line << " " << q.pos << " " << q.params 
				<< " " << q.GetValue()  << "  " << q.GetHashCode() << std::endl;
		}*/

		int kk = 1;
		for (auto jj : lexTable)
		{
			if (jj.line != kk) std::cout << std::endl;
			std::cout << jj.lexema;
			kk = jj.line;
		}
		std::cout << std::endl;
		std::cout << std::endl;
		for (auto jj : lexTable)
		{
			std::cout << jj.line << "  " << jj.index << "  " << jj.lexema <<
				"  " << jj.positionInIdTable << std::endl;
		}

		
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
		/*for (auto i : idTable)
		{
			auto q = i.second;
			std::cout << kl[q.type] << " " << q.name << " " << kl[q.valueType] << " "
				<< q.scope << " " << q.ownScope << " " << q.line << " " << q.pos << " " << q.params
				<< " " << q.GetValue() << "  " << q.GetHashCode() << std::endl;
		}*/

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