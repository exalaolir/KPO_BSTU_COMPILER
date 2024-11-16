#include "pch.h"
#include "Lexer.h"
#include "Automat.h"
#include "Symbols_And_Resourses.h"

namespace LEXER
{
	std::list<Lexem> Lexer::generateLexTable()
	{
		std::list<Lexem> result;
		int line = 1;
		int counter = 0;

		for (auto& token : this->preprocesseredStr)
		{
			auto newLex = GetType(token, line, counter);
			if (newLex != DEPRICATED_SYMBOL)
			{
				Lexem lex(newLex, line, counter);
				result.push_back(lex);
			}
		}

		int kk = 1;
		for (auto jj : result)
		{
			if (jj.line != kk) std::cout << std::endl;
			std::cout << jj.lexema;
			kk = jj.line;
		}
		std::cout << std::endl;
		std::cout << std::endl;
		for (auto jj : result)
		{
			std::cout << jj.line << "  " << jj.index << "  "  << jj.lexema << std::endl;
		}

		return result;
	}

	inline std::string LEXER::Lexer::GetType(string& token, int& line, int& counter)
	{
		std::string result;
		counter += 1;

		if (Lexems.contains(token))
		{
			result = Lexems.at(token);
		}
		else
		{
			Regex::Regex regex;

			if (regex.Match(token, "(\"[\n-€]*\")"))
			{
				result = "l";
			}
			else if (token == "|")
			{
				line += 1;
				counter = 0;
				return DEPRICATED_SYMBOL;
			}
			else if (regex.Match(token, "(([1-9]+[0-9]*|[0-9]+.[0-9]+|true|false))"))
			{
				result = "l";
			}
			else if (regex.Match(token, "((-[1-9]+[0-9]*|-[0-9]+.[0-9]+))"))
			{
				result = "l";
			}
			else if (regex.Match(token, "(0b(1|0)+)"))
			{
				result = "l";
			}
			else if (regex.Match(token, "((0[1-7]+[0-9]*|-0[1-7]+[0-9]*))"))
			{
				result = "l";
			}
			else if (regex.Match(token, "((0x([A-F]+|[1-9])+[0-9]*[A-F]*|-0x([A-F]+|[1-9])+[0-9]*[A-F]*))"))
			{
				result = "l";
			}
			else if (regex.Match(token, "([A-z]([A-z]|[0-9])*)")
					 && !TokenTypes.contains(token))
			{
				result = "i";
			}
			else
			{
				ERROR_LOG(std::format("Sourse code: строка {}, индекс лексемы {}", line, counter),
						  std::format("Ќераспознана€ лексема {}", token));
				throw "Esception";
			}
		}
		return result;
	}
}