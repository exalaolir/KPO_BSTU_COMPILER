#include "pch.h"
#include "Lexer.h"
#include "Automat.h"
#include "Symbols_And_Resourses.h"

namespace LEXER
{
	std::vector<Lexem> Lexer::generateLexTable()
	{
		std::vector<Lexem> result;
		std::stack<Keywords> brackets;

		result.reserve(preprocesseredStr.size());
		int line = 1;
		int counter = 0;
		int timeToScopeGen = 1;

		for (auto iter = this->preprocesseredStr.begin(); iter != this->preprocesseredStr.end(); iter++)
		{
			double hash = -1.0;
			short prioryty = -1;
			auto token = *iter;
			auto newLex = GetType(token, line, counter, hash, timeToScopeGen, brackets, iter);
			if (newLex != DEPRICATED_SYMBOL)
			{
				if (newLex == "o")
				{
					switch (newLex[0])
					{
					case '(':
						prioryty = 0;
						break;
					case ')':
						prioryty = 0;
						break;
					case ',':
						prioryty = 1;
						break;
					case '+':
						prioryty = 2;
						break;
					case '-':
						prioryty = 2;
						break;
					case '*':
						prioryty = 3;
						break;
					case '/':
						prioryty = 3;
						break;
					case '%':
						prioryty = 3;
						break;
					default:
						break;
					}
				}
				Lexem lex(newLex, line, counter, hash, prioryty);
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
			std::cout << jj.line << "  " << jj.index << "  "  << jj.lexema <<
				"  " << jj.positionInIdTable << std::endl;
		}

		scopes = {};
		scopes.push("g0");
		currentScopeNumber = 0;
		literalNumber = 0;
		return result;
	}

	inline std::string LEXER::Lexer::GetType(string& token, int& line, int& counter, double& hash, int& timeToScopeGen, std::stack<Keywords>& brackets, auto& iter)
	{
		auto hasher = [](string& name, string& scope)->double {
			return std::hash<string>()(name) +
				std::hash<string>()(scope);
		};

		std::string result;
		counter += 1;

		int notLine = line; 
		int notcounter = line;
		
		if(timeToScopeGen > 1) timeToScopeGen++;

		Keywords tokenType = GetKeyword(token, notLine, notcounter, true);

		switch (tokenType)
		{
		case Fun:
		{
			timeToScopeGen++;
			break;
		}
		case OpenBracket:
		{
			brackets.push(OpenBracket);
			break;
		}
		case If:
		{
			string id = "i";
			scopeGenerator(scopes.top(), id);
			break;
		}
		case Else:
		{
			string id = "e";
			scopeGenerator(scopes.top(), id);
			break;
		}
		case For:
		{
			string id = "f";
			scopeGenerator(scopes.top(), id);
			break;
		}
		case While:
		{
			string id = "w";
			scopeGenerator(scopes.top(), id);
			break;
		}
		case BoolLiteral:
		{
			std::string id = "none";
			std:: string name = "Literal-" + std::to_string(literalNumber);
			hash = hasher(name, id);
			literalNumber++;
			break;
		}
		case DoubleLiteral:
		{
			std::string name = "Literal-" + std::to_string(literalNumber);
			std::string id = "none";
			hash = hasher(name, id);
			literalNumber++;
			break;
		}
		case StringLiteral:
		{
			std::string name = "Literal-" + std::to_string(literalNumber);
			std::string id = "none";
			hash = hasher(name, id);
			literalNumber++;
			break;
		}
		case IntLiteral:
		{
			std::string name = "Literal-" + std::to_string(literalNumber);
			std::string id = "none";
			hash = hasher(name, id);
			literalNumber++;
			break;
		}
		case Main:
		{
			hash = hasher(token, scopes.top());
			if (timeToScopeGen == 4)
			{
				string id = string() + token[0];
				scopeGenerator(scopes.top(), id);
				timeToScopeGen = 1;
			}
			break;
		}
		case Id:
		{
			iter++;
			if (*iter == "(" && scopes.top() != "g0")
			{
				string id = "g0";
				hash = hasher(token, id);
			}
			else
			{
				hash = hasher(token, scopes.top());
				if (timeToScopeGen == 4)
				{
					string id = string() + token[0];
					scopeGenerator(scopes.top(), id);
					timeToScopeGen = 1;
				}
			}
			iter--;
			break;
		}
		case CloseBracket:
		{
			if (brackets.empty())
			{
				ERROR_LOG(std::format("Sourse code: строка {}, индекс лексемы {}", line, counter),
						  "Ќарушен уровень вложенности");
				throw "Esception";
			}
			brackets.pop();
			if (scopes.size() != 1) scopes.pop();
			break;
		}
		default:
			break;
		}
		
		if (Lexems.contains(token))
		{
			result = Lexems.at(token);
		}
		else
		{
			Regex::Regex regex;

			if (regex.Match(token, "(\"[\t-€]*\")"))
			{
				result = "l";
			}
			else if (token == "|")
			{
				line += 1;
				counter = 0;
				return DEPRICATED_SYMBOL;
			}
			else if (regex.Match(token, "(([1-9]+[0-9]*|[0-9]+.[0-9]+|true|false|0))"))
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
			else if (regex.Match(token, "((0h([A-F]+|[1-9])+[0-9]*[A-F]*|-0h([A-F]+|[1-9])+[0-9]*[A-F]*))"))
			{
				result = "l";
			}
			else if (regex.Match(token, "(([A-Z]|[a-z])(([A-Z]|[a-z])|[0-9])*)")
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