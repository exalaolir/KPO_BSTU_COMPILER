#include "pch.h"
#include "InputValidator.h"
#include "Lexer.h"
#include "Magazine_Automate.h"
#include "Polish.h"
#include "Syntax_Analiser.h"
#include "Generator.h"

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
		auto h = lexer.generateIdTable(hjk);

		for (auto i : h.keys)
		{
			auto q = h.table[i];
			std::cout << kl[q.type] << " " << q.name << " " << kl[q.valueType] << " " 
				<< q.scope << " " << q.ownScope << " " << q.line << " " << q.pos << " " << q.params 
				<< " " << q.GetValue()  << "  " << q.GetHashCode() << std::endl;
		}

		int kk = 1;
		for (auto jj : hjk)
		{
			if (jj.line != kk) std::cout << std::endl;
			std::cout << jj.lexema;
			kk = jj.line;
		}
		std::cout << std::endl;
		std::cout << std::endl;
		for (auto jj : hjk)
		{
			std::cout << jj.line << "  " << jj.index << "  " << jj.lexema <<
				"  " << jj.positionInIdTable << std::endl;
		}

		auto log = std::make_shared<std::ofstream>("translationInfo.txt.log");
		std::cout << "---------------------------начало синтаксического анализатора-------------------------" << std::endl;
		MFST_TRACE_START(log);
		MFST::Mfst mfst(hjk, h, GRB::getGreibach());
		if (!mfst.start(log))
			throw "Error";
		mfst.savededucation();
		mfst.printrules(log);
		
		ANALISER::Analiser analiser;
		analiser.analise(hjk, h);

		POLISH::changeLexTable(hjk, h);

		std::cout << std::endl;
		std::cout << std::endl;
		for (auto i : h)
		{
			auto q = i.second;
			std::cout << kl[q.type] << " " << q.name << " " << kl[q.valueType] << " "
				<< q.scope << " " << q.ownScope << " " << q.line << " " << q.pos << " " << q.params
				<< " " << q.GetValue() << "  " << q.GetHashCode() << std::endl;
		}

		GEN::Generator gen(params[OUT_FILE], hjk, h);
		gen.Generate();
	}
	catch (const std::out_of_range& ex)
	{
		std::cout << ex.what();
	}
	catch (...)
	{
		
	}
}