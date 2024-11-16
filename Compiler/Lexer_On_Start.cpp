#include "pch.h"
#include "Lexer.h"
#include "Symbols_And_Resourses.h"

LEXER::Lexer::Lexer(std::string& fileName)
{
	currentScopeNumber = 0;
	scopes.push("g0");

	if (!inFile.is_open())
	{
		sourseCode = readFile(fileName);
		preprocesseredStr = preprocessCode(sourseCode);
	}
}

LEXER::Lexer::~Lexer()
{
	if (inFile.is_open())
	{
		inFile.close();
	}
}

std::list<string> LEXER::Lexer::preprocessCode(std::string& code)
{
	std::list<string> result;
	std::list<char> buffer;
	std::string newStr;
	std::stack<std::tuple<int, int, char>> literalBrackets;
	std::stack<std::tuple<int, int, char>> commentBrackets;
	size_t i = 0;
	int lineCounter = 1;
	int simbolCounter = 0;
	bool shouldChange;

	auto findSpecial = [&](bool predicate, size_t& i)
		{
			if (predicate)
			{
				if ((int)i - 1 >= 0 && (int)i + 1 < code.length() && code[i - 1] == '/' && code[i + 1] == '/')
				{
					buffer.pop_back();
					buffer.push_back(code[i]);
					i += 1;
					shouldChange = false;
				}
				else
				{
					auto bracket = std::make_tuple(lineCounter, simbolCounter, code[i]);
					if (literalBrackets.empty())
					{
						literalBrackets.push(bracket);
					}
					else if (std::get<2>(literalBrackets.top()) == std::get<2>(bracket))
					{
						literalBrackets.pop();
						if (literalBrackets.empty())
						{
							shouldChange = false;
							buffer.push_back(code[i]);
							buffer.push_back('$');
						}
					}
				}
			}
			if (!literalBrackets.empty())
			{
				if ((int)i - 1 >= 0 && 
					(int)i + 1 < code.length() && 
					code[i - 1] == '/' && 
					code[i + 2] == '/' &&
					code[i] == '\\' && 
					(code[i + 1] == 'v' || code[i + 1] == 'n' || code[i + 1] == 't'))
				{
					buffer.pop_back();
					std::string simbol = string() + code[i] + code[i+1];
					
					buffer.push_back(SpecialSymbols.at(simbol));
					i += 2;
					shouldChange = false;
				}
			}
		};

	auto findComment = [&](bool predicate)
		{
			if (predicate)
			{
				if ((int)i + 1 < code.length() && (int)i - 1 >= 0 && code[i - 1] == '/' && code[i + 1] == '/')
				{
					i += 1;
				}
				else
				{
					auto comment = std::make_tuple(lineCounter, simbolCounter, code[i]);
					if (commentBrackets.empty())
					{
						commentBrackets.push(comment);
					}
					else if (std::get<2>(commentBrackets.top()) == std::get<2>(comment))
					{
						commentBrackets.pop();
					}
				}
			}
		};

	auto generateError = [](auto& stack)
		{

			if (!stack.empty())
			{
				std::tuple<int, int, char> errorSymbol;
				while (!stack.empty())
				{
					errorSymbol = Utils::Pop(stack);
					std::string mes = std::format("Source code, line {}, simbol {}",
												  std::get<0>(errorSymbol),
												  std::get<1>(errorSymbol));
					ERROR_LOG(mes, std::format("Лишний символ {}", std::get<2>(errorSymbol)));
				}
				throw "Exception";
			}
		};

	for (i; i < code.length(); i++)
	{
		shouldChange = true;

		if(code[i] != '\t' && code[i] != '\v' && code[i] != ' ') simbolCounter += 1;

		if (code[i] == '|')
		{
			simbolCounter = 0;
			lineCounter += 1;
		}

		findComment(code[i] == '@');
		if (commentBrackets.empty())
		{
			findSpecial(code[i] == '\"' || code[i] == '\'', i);

			if (literalBrackets.empty())
			{
				if (IS_DEPRICATED_SYMBOL(i)) continue;

				if (code[i] == ' ')
				{
					buffer.push_back('$');
					continue;
				}

				if (i - 1 > 0 && i + 1 < code.length() && code[i] == '-')
				{
					int counter = 1;

					while (code[i - counter] == ' ')
					{
						counter++;
					}
					
					bool isSymbol = (MATH_SIMBOL(i, counter));
					if (i + 2 < code.length() && isSymbol && code[i + 1] == 0 && code[i + 2] == 'x' && counter > 1)
					{
						buffer.push_back('$');
						buffer.push_back(code[i]);
						code.erase(i + 2, 1);
						continue;
					}
					else if (i + 2 < code.length() && isSymbol && code[i + 1] == 0 && code[i + 2] == 'o' && counter > 1)
					{
						buffer.push_back('$');
						buffer.push_back(code[i]);
						continue;
					}
					else if (isSymbol && isdigit(code[i + 1]) && counter > 1)
					{
						buffer.push_back('$');
						buffer.push_back(code[i]);
						continue;
					}
				}

				if (Spaces.contains(code[i]))
				{
					buffer.push_back('$');
					buffer.push_back(code[i]);
					buffer.push_back('$');
					continue;
				}
			}
			if (shouldChange) buffer.push_back(code[i]);
		}
		else if (code[i] == '|')
		{
			buffer.push_back('$');
			buffer.push_back(code[i]);
			buffer.push_back('$');
		}
	}

	generateError(literalBrackets);
	generateError(commentBrackets);

	newStr.reserve(buffer.size());
	std::copy(buffer.begin(), buffer.end(), std::back_inserter(newStr));

	result = Utils::Split(newStr, '$');

	for (auto h : result)
	{
		std::cout << h << std::endl;
	}
	return result;
}

std::string LEXER::Lexer::readFile(std::string& fileName)
{
	std::string result;
	inFile.open(fileName);

	if (inFile.is_open())
	{
		string res;

		while (getline(inFile, res))
		{
			result += res + "|";
		}
	}
	inFile.close();

	//std::cout << result;

	return result;
}
