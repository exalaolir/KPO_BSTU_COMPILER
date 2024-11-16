#pragma once

using uChar = unsigned char;
using nodePair = std::pair<uChar, uChar>;
using std::vector;
using std::pair;
using std::unordered_map;
using std::stack;
using std::string;

namespace Parser
{
	static struct Parser
	{
	public:
		Parser() {}

		struct Graph
		{
			void Add(std::pair<size_t, std::vector<size_t>>);
			void AddEpsilon(std::pair<size_t, std::vector<size_t>>);

			std::unordered_map<size_t, std::vector<size_t>> nodeMap;
			std::unordered_map<size_t, std::vector<size_t>> epsilonMap;
			vector<size_t> epsilonIndexes;
			vector<string> dividingRegex;
		};
		static Graph Parse(string value);
	private:

		static const enum NodeTypes
		{
			Symbol,
			Plus,
			Star,
			OpenBracket,
			CloseBracket,
			BracketBlock,
			Or,
			None
		};

		struct Node
		{
			std::string value;
			NodeTypes type;
			std::shared_ptr<Node> start;
			std::shared_ptr<Node> end;
			size_t index;

			Node()
			{
				value = "";
				type = None;
				index = -1;
				start = nullptr;
				end = nullptr;
			}

			Node(std::string value, NodeTypes type, size_t index, Node start = Node(), Node end = Node())
			{
				this->value = value;
				this->type = type;
				this->index = index;
				start.type == None ?
					this->start = nullptr :
					this->start = std::make_shared<Node>(start);
				end.type == None ?
					this->end = nullptr :
					this->end = std::make_shared<Node>(end);
			}
		};

		static vector<Node> createChain(vector<string> currenntString, stack<pair<vector<string>, int>>& parsedData, int started);
		static Node createBracketsMask(vector<string> val, int index, int& counter, stack<pair<vector<string>, int>>& parsedData, int started);
		static void plusRyles(Node privios, Node current, Node next, unordered_map<size_t, vector<size_t>>& epsilonMap);
		static void openRyles(Node current, Node next, unordered_map<size_t, vector<size_t>>& epsilonMap);
		static void bracketsRules(Node current, Node next, unordered_map<size_t, vector<size_t>>& epsilonMap);
		static void starRyles(Node privios, Node current, Node next, unordered_map<size_t, vector<size_t>>& epsilonMap);
		static void orRyles(Node openBracket, Node closeBracket, Node current, Node next, unordered_map<size_t, vector<size_t>>& epsilonMap);
		static vector<string> regexDevider(string regex);
		static void preprocessor(string& regex);

		static inline const std::map<string, NodeTypes>  epsilonSymbolsMap
		{
			{"(", OpenBracket},
			{")", CloseBracket},
			{"*", Star},
			{"+", Plus},
			{"|", Or}
		};
	};
}