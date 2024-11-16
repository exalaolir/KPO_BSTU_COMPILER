#include "pch.h"
#include "Parser.h"

using namespace Utils;

void Parser::Parser::Graph::Add(pair<size_t, vector<size_t>> item) {
	nodeMap.insert(item);
}

void Parser::Parser::Graph::AddEpsilon(pair<size_t, vector<size_t>> item)
{
	epsilonMap.insert(item);
}

Parser::Parser::Graph Parser::Parser::Parse(string value)
{
	Parser::Parser::Graph result;

	string saveValue = value;
	unordered_map<size_t, vector<size_t>> nodeMap;
	unordered_map<size_t, vector<size_t>> epsilonMap;
	stack<pair<vector<string>, int>> parsedData;
	preprocessor(value);
	auto preprocessedValue = regexDevider(value);

	result.dividingRegex = preprocessedValue;
	parsedData.push({ preprocessedValue, 0 });

	while (!parsedData.empty())
	{
		auto currentParse = Pop(parsedData);
		auto str = currentParse.first;
		auto in = currentParse.second;
		vector<Node> chein = createChain(str, parsedData, in);

		for (size_t i = 0; i < chein.size() - 1; i++)
		{
			switch (chein[i].type)
			{
			case Plus:
				plusRyles(chein[i - 1], chein[i], chein[i + 1], epsilonMap);
				result.epsilonIndexes.push_back(chein[i].index);
				break;
			case OpenBracket:
				openRyles(chein[i], chein[i + 1], epsilonMap);
				result.epsilonIndexes.push_back(chein[i].index);
				break;
			case BracketBlock:
			{
				bracketsRules(chein[i], chein[i + 1], epsilonMap);
				result.epsilonIndexes.push_back(chein[i].index);
				auto end = *chein[i].end;
				result.epsilonIndexes.push_back(end.index);
				break;
			}
			case Star:
				starRyles(chein[i - 1], chein[i], chein[i + 1], epsilonMap);
				result.epsilonIndexes.push_back(chein[i].index);
				break;
			case Or:
				orRyles(chein[0], chein[chein.size() - 1], chein[i], chein[i + 1], epsilonMap);
				result.epsilonIndexes.push_back(chein[i].index);
				break;
			default:
				nodeMap[chein[i].index].push_back(chein[i + 1].index);
				break;
			}
		}
	}

	for (const auto& [key, value] : epsilonMap)
	{
		result.AddEpsilon({ key, value });
	}

	for (const auto& [key, value] : nodeMap)
	{
		result.Add({ key, value });
	}

	std::erase(result.epsilonIndexes, value.length() - 1);

	return result;
}

vector<Parser::Parser::Node> Parser::Parser::createChain(vector<string> currenntString, stack<pair<vector<string>, int>>& parsedData, int started)
{

	vector<Node> result;

	result.push_back(Node("(", OpenBracket, started));
	for (size_t i = 1; i < currenntString.size() - 1; i++)
	{
		int counter = 0;
		auto currentSymbol = currenntString[i];
		NodeTypes currentType;
		if (!TryGetValue(currentSymbol, Parser::epsilonSymbolsMap, currentType)) currentType = Symbol;
		Node currentNode;

		switch (currentType)
		{
		case Plus:
			currentNode = Node(string() + currentSymbol, Plus, i + started);
			break;
		case Star:
			currentNode = Node(string() + currentSymbol, Star, i + started);
			break;
		case Or:
			currentNode = Node(string() + currentSymbol, Or, i + started);
			break;
		case OpenBracket:
			currentNode = createBracketsMask(currenntString, i, counter, parsedData, started);
			i += counter;
			break;
		default:
			currentNode = Node(string() + currentSymbol, Symbol, i + started);
			break;
		}

		result.push_back(currentNode);
	}
	result.push_back(Node(")", CloseBracket, currenntString.size() - 1 + started));
	return result;
}



Parser::Parser::Node Parser::Parser::createBracketsMask(vector<string> val, int index, int& counter, stack<pair<vector<string>, int>>& parsedData, int started)
{
	stack<string> brackets;
	brackets.push(val[index]);
	vector<string> bracketsRange;
	bracketsRange.push_back("(");

	for (size_t i = (size_t)index + 1; i < val.size(); i++)
	{
		if (val[i] == "(") brackets.push(val[i]);
		if (val[i] == ")" && !brackets.empty()) brackets.pop();
		bracketsRange.push_back(val[i]);
		counter++;
		if (brackets.empty()) break;
	}

	parsedData.push({ bracketsRange, index + started });
	return Node("", BracketBlock, index + started, Node("(", OpenBracket, index), Node(")", CloseBracket, index + counter + started));
}

void Parser::Parser::plusRyles(Node privios, Node current, Node next, unordered_map<size_t, vector<size_t>>& epsilonMap)
{
	epsilonMap[current.index].push_back(privios.index);
	epsilonMap[current.index].push_back(next.index);
}

void Parser::Parser::openRyles(Node current, Node next, unordered_map<size_t, vector<size_t>>& epsilonMap)
{
	epsilonMap[current.index].push_back(next.index);
}

void Parser::Parser::bracketsRules(Node current, Node next, unordered_map<size_t, vector<size_t>>& epsilonMap)
{
	Node currentEnd = *current.end;
	epsilonMap[currentEnd.index].push_back(next.index);
}

void Parser::Parser::starRyles(Node privios, Node current, Node next, unordered_map<size_t, vector<size_t>>& epsilonMap)
{
	epsilonMap[current.index].push_back(privios.index);
	epsilonMap[current.index].push_back(next.index);
	if (privios.type != BracketBlock)
	{
		epsilonMap[privios.index].push_back(current.index);
	}
	else
	{
		auto end = *privios.end;
		auto start = *privios.start;
		epsilonMap[end.index].push_back(current.index);
		epsilonMap[start.index].push_back(current.index);
	}
}

void Parser::Parser::orRyles(Node openBracket, Node closeBracket, Node current, Node next, unordered_map<size_t, vector<size_t>>& epsilonMap)
{
	epsilonMap[current.index].push_back(closeBracket.index);
	epsilonMap[openBracket.index].push_back(next.index);
}

vector<string> Parser::Parser::regexDevider(string regex)
{
	vector<string> result;
	const auto strLength = regex.length();
	const size_t ErrorRange = 2;

	for (size_t i = 0; i < strLength; i++)
	{
		if (regex[i] == '/' &&
			i < strLength - ErrorRange &&
			Parser::epsilonSymbolsMap.contains(string() + regex[i + 1]) &&
			regex[i + 2] == '/')
		{
			result.push_back(string() + regex[i] + regex[i + 1] + regex[i + 2]);
			i += 2;
		}
		else
		{
			result.push_back(string() + regex[i]);
		}
	}
	return result;
}

void Parser::Parser::preprocessor(string& regex)
{
	const size_t ErrorRange = 5;

	for (size_t i = 0; i < regex.length(); i++)
	{
		if ((uChar)regex[i] == '[' && (uChar)regex[i + 4] == ']')
		{
			string input = "(";
			int start = (unsigned char)regex[i + 1];
			int end = (unsigned char)regex[i + 3];

			if (start > end) std::swap(start, end);
			for (int code = start; code <= end; code++)
			{
				if (epsilonSymbolsMap.contains(string() + (char)code))
				{
					if (input[input.length() - 1] != '|')
					{
						input += string() + "|" + "/" + (char)code + "/" + "|";
					}
					else
					{
						input += string() + "/" + (char)code + "/" + "|";
					}
				}
				else
				{
					input += string() + (char)code + "|";
				}
			}
			input.erase(input.length() - 1, 1);
			input += ")";

			regex.erase(i, ErrorRange);
			regex.insert(i, input);
			i +=  input.length() - 1;
		}
	}
}
