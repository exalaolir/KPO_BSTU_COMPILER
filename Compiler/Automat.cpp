#include "pch.h"
#include "automat.h"
#include "Parser.h"

using namespace Utils;

namespace Regex
{
	void Regex::Automat::Dfs(size_t start, std::vector<size_t>& visited, std::unordered_map<size_t, std::vector<size_t>>& memoisResults) const
	{
		std::stack<size_t> nodeStack;

		if (memoisResults.contains(start))
		{
			visited = memoisResults[start];
		}
		else
		{
			nodeStack.push(start);

			while (!nodeStack.empty())
			{
				auto current = Pop(nodeStack);
				auto& epsilons = *epsilonMap;
				for (size_t edje : epsilons[current])
				{
					if (!Contains(edje, nodeStack) && std::find(visited.begin(), visited.end(), edje) == visited.end())
					{
						nodeStack.push(edje);
					}
				}
				visited.push_back(current);
			}

			for (auto epsilonIndex : *epsilonIndexes)
			{
				visited.erase(std::remove(visited.begin(), visited.end(), epsilonIndex), visited.end());
			}

			memoisResults[start] = visited;
		}
	}

	bool Regex::Match(std::string predicate, std::string expression)
	{
		if (!cheins.contains(expression))
		{
			cheins[expression] = Parser::Parser::Parse(expression);
		}

		auto& parse = cheins[expression];
		this->automat = Automat(parse.epsilonMap, parse.nodeMap, parse.epsilonIndexes);
		vector<std::string> finalExpression = parse.dividingRegex;

		if (!expressionsResults.contains(expression))
		{
			expressionsResults[expression] = {};
		}

		std::unordered_map<size_t, std::vector<size_t>>& memoisResults = expressionsResults[expression];

		std::vector<size_t> candidateStates;
		candidateStates.reserve(expression.size());

		this->automat.Dfs(0, candidateStates, memoisResults);

		std::vector<size_t> newStates;
		std::vector<size_t> newCandidates;

		newCandidates.reserve(expression.size());

		for (size_t i = 0; i < predicate.size(); i++)
		{
			char symbol = predicate[i];

			//std::sort(begin(candidateStates), end(candidateStates));

			for (const auto& state : candidateStates)
			{
				if (finalExpression[state].length() == 1 && symbol == finalExpression[state][0])
				{
					auto& nodeMap = *this->automat.nodeMap;
					Concat(newStates, nodeMap[state]);
				}
				else if (finalExpression[state].length() == 3 && symbol == finalExpression[state][1])
				{
					auto& nodeMap = *this->automat.nodeMap;
					Concat(newStates, nodeMap[state]);
				}
			}

			if (newStates.empty()) return false;

			candidateStates.clear();

			for (auto newTransitionPoint : newStates)
			{
				this->automat.Dfs(newTransitionPoint, newCandidates, memoisResults);
				Concat(candidateStates, newCandidates);
			}

			if (i == predicate.size() - 1 && Contains(finalExpression.size() - 1, candidateStates))
			{
				return true;
			}

			newStates.clear();
			newCandidates.clear();
		}

		return false;
	}
}
