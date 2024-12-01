#include "pch.h"
#include "InputValidator.h"
#include "Lexer.h"
#include "Magazine_Automate.h"
#include "Polish.h"
#include "Syntax_Analiser.h"

using namespace INPUTVAL;
using namespace LEXER;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru");
 	setlocale(LC_NUMERIC, "C");
	try
	{
		std::unordered_map<Keywords, string> kl
		{
			{Int, "int"},
			{String, "string"},
			{Double, "double"},
			{Bool, "bool"},
			{Fun, "fn"},
			{Return, "return"},
			{Main, "main"},
			{If, "if"},
			{Else, "else"},
			{For, "for"},
			{While, "while"},
			{ServisSymbol, ";"},
			{ServisSymbol, ","},
			{OpenBracket, "{"},
			{CloseBracket, "}"},
			{ServisSymbol, "("},
			{ServisSymbol, ")"},
			{ServisSymbol, "+"},
			{ServisSymbol, "-"},
			{ServisSymbol, "*"},
			{ServisSymbol, "/"},
			{ServisSymbol, "="},
			{Param, "param"},
			{Variable, "var"}
		};
		auto params = validateParams(argc, argv);
		Lexer lexer(params[IN_FILE]);
		auto hjk = lexer.generateLexTable();
		auto h = lexer.generateIdTable();

		for (auto i : h.keys)
		{
			auto q = h.table[i];
			std::cout << kl[q.type] << " " << q.name << " " << kl[q.valueType] << " " 
				<< q.scope << " " << q.ownScope << " " << q.line << " " << q.pos << " " << q.params 
				<< " " << q.GetValue()  << "  " << q.GetHashCode() << std::endl;
		}

		auto log = std::make_shared<std::ofstream>("translationInfo.txt.log");

		/*MFST_TRACE_START(log);
		MFST::Mfst mfst(hjk, h, GRB::getGreibach());
		if (!mfst.start(log))
			throw "Error";
		mfst.savededucation();
		mfst.printrules(log);*/

		ANALISER::Analiser analiser;
		analiser.analise(hjk, h);

		POLISH::changeLexTable(hjk, h);
	}
	catch (...)
	{
		
	}
}