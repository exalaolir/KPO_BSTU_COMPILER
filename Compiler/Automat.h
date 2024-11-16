#pragma once
#include "Parser.h"

namespace Regex
{
	struct Regex
	{
	public:

		Regex() {}
		bool Match(std::string predicate, std::string expression);
	private:
		struct Automat
		{
			Automat()
			{
				epsilonMap = nullptr;
				nodeMap = nullptr;
				epsilonIndexes = nullptr;
			}
			Automat(std::unordered_map<size_t, std::vector<size_t>>& epsilonMap,
					std::unordered_map<size_t, std::vector<size_t>>& nodeMap,
					std::vector<size_t>& epsilonIndexes)
			{
				this->epsilonMap = std::make_shared<std::unordered_map<size_t, std::vector<size_t>>>(epsilonMap);
				this->nodeMap = std::make_shared<std::unordered_map<size_t, std::vector<size_t>>>(nodeMap);
				this->epsilonIndexes = std::make_shared<std::vector<size_t>>(epsilonIndexes);
			}
			void Dfs(size_t start, std::vector<size_t>& visited, std::unordered_map<size_t, std::vector<size_t>>& memoisResults) const;
			std::shared_ptr<std::unordered_map<size_t, std::vector<size_t>>> epsilonMap;
			std::shared_ptr<std::unordered_map<size_t, std::vector<size_t>>> nodeMap;
			std::shared_ptr<std::vector<size_t>> epsilonIndexes;
		} automat;

		inline static std::unordered_map<std::string, std::unordered_map<size_t, std::vector<size_t>>> expressionsResults;

		static inline unordered_map<string, Parser::Parser::Graph> cheins;
	};
}

