#include "pch.h"
#include "Lexer.h"
#include "Automat.h"
#include "Symbols_And_Resourses.h"

namespace LEXER
{

	int counterVec = 0;
	std::variant <Keywords,
		std::string,
		int,
		std::variant<int, double, bool, std::string, Keywords>
	> LEXER::Entry::operator[](unsigned index) const
	{
		switch (index)
		{
		case 0:
			return type;
		case 1:
			return valueType;
		case 2:
			return name;
		case 3:
			return scope;
		case 4:
			return ownScope;
		case 5:
			return line;
		case 6:
			return pos;
		case 7:
			return params;
		case 8:
			return value;
		default:
			throw std::out_of_range("Index out of range in entry of IdTable");
		}
	}

	Entry& LEXER::IdTable::operator[](double key) const
	{
		if(key == -1) 
			throw std::out_of_range(__FILE__ + std::to_string(__LINE__));
		return table[key];
	}

	std::string LEXER::Entry::GetValue()
	{
		auto visitor = [](auto&& arg) -> std::string
			{
				using T = std::decay_t<decltype(arg)>;

				if constexpr (std::is_same_v<T, int>)
				{
					return std::to_string(arg);
				}
				else if constexpr (std::is_same_v<T, std::string>)
				{
					return arg;
				}
				else if constexpr (std::is_same_v<T, Keywords>)
				{
					return std::to_string(arg);
				}
				else if constexpr (std::is_same_v<T, double>)
				{
					return std::to_string(arg);
				}
				else if constexpr (std::is_same_v<T, bool>)
				{
					return std::to_string(arg);
				}
			};

		std::string result = std::visit(visitor, value);
		return result;
	}

	double LEXER::Entry::GetHashCode() const
	{
		return std::hash<string>()(name) +
			std::hash<string>()(scope);
	}

	void IdTable::Add(Entry& entry)
	{
		double key = entry.GetHashCode();
		table.insert(std::make_pair(key, entry));
		keys.push_back(key);
	}

	bool IdTable::Contains(Entry& entry) const
	{
		return table.contains(entry.GetHashCode());
	}

	bool LEXER::IdTable::Contains(double& key) const
	{
		return table.contains(key);
	}

	void IdTable::Delete(Entry& entry) const
	{
		table.erase(entry.GetHashCode());
	}

	IdTable LEXER::Lexer::generateIdTable(std::vector<Lexem>& lexTable)
	{
		std::stack<Keywords> brackets;
		int line = 1;
		int counter = 0;
		bool hasMain = false;
		auto reservedStr = preprocesseredStr; 

		for (auto token = preprocesseredStr.begin(); token != preprocesseredStr.end(); token++, counterVec++)
		{
			switch (GetKeyword(*token, line, counter))
			{
			case Fun:
				createFun(token, lexTable, idTable,  preprocesseredStr, line, counter, hasMain);
				break;
			case Int:
				createVar(token, lexTable, idTable, preprocesseredStr, line, counter, Int);
				break;
			case String:
				createVar(token, lexTable, idTable, preprocesseredStr, line, counter, String);
				break;
			case Double:
				createVar(token, lexTable, idTable, preprocesseredStr, line, counter, Double);
				break;
			case Bool:
				createVar(token, lexTable, idTable, preprocesseredStr, line, counter, Bool);
				break;
			case Let:
				createAuto(token, lexTable, idTable, preprocesseredStr, line, counter);
				break;
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
			case Id:
			{
				auto allScopes = scopes;
				bool check = false;
				while (!allScopes.empty())
				{
					auto scope = Utils::Pop(allScopes);
					double key = std::hash<string>()(*token) +
						std::hash<string>()(scope);
					if (idTable.Contains(key))
					{
						int hhh = std::distance(preprocesseredStr.begin(), token) - line + 1;
						std::cout << hhh << std::endl;
						lexTable[hhh].positionInIdTable = key;
						check = true;
						break;
					}
				}
				if (!check)
				{
					ERROR_LOG(std::format("Sourse code: строка {}, индекс лексемы {}", line, counter),
							  std::format("Необъявлееный идентификатор {}", *token));
					throw "Esception";
					break;
				}
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
			case CloseBracket:
			{
				if (brackets.empty())
				{
					ERROR_LOG(std::format("Sourse code: строка {}, индекс лексемы {}", line, counter),
							  "Нарушен уровень вложенности");
					throw "Esception";
				}
				brackets.pop();
				if (scopes.size() != 1) scopes.pop();
				break;
			}
			default:
				break;
			}
		}

		if (!hasMain)
		{
			ERROR_LOG("Sourse code",
					  "Отсутствует функция main");
			throw "Esception";
		}

		preprocesseredStr = reservedStr;
		return idTable;
	}

	Keywords LEXER::Lexer::GetKeyword(string& token, int& line, int& counter, bool isLexTable)
	{
		Keywords type;
		counter += 1;

		auto isValidLiteral = [=](Keywords type, std::string& literal, int code = 10) {
			try
			{
				double val;
				int intVal;
				switch (type)
				{

				case LEXER::IntLiteral:
					intVal = std::stoi(literal, nullptr, code);
					break;
				case LEXER::DoubleLiteral:
					val = std::stod(literal);
					break;
				default:
					break;
				}
			}
			catch (...)
			{
				ERROR_LOG(std::format("Sourse code: строка {}, индекс лексемы {}", line, counter),
						  std::format("Выход за пределы допустимого диапазона типа {}", token));
				throw "Esception";
			}
			};

		if (TokenTypes.contains(token))
		{
			type = TokenTypes.at(token);
		}
		else
		{
			Regex::Regex regex;

			if (regex.Match(token, "(\"[\t-я]*\")"))
			{
				type = StringLiteral;
				
				if (!isLexTable)
				{
					Entry literal;

					literal.type = Literal;
					literal.name = "Literal_" + std::to_string(literalNumber);
					literalNumber++;
					literal.valueType = StringLiteral;
					literal.scope = "none";
					literal.line = line;
					literal.pos = counter;
					literal.value = token;

					idTable.Add(literal);
				}
			}
			else if (token == "|")
			{
				line += 1;
				counter = 0;
				return None;
			}
			else if (regex.Match(token, "(([1-9]+[0-9]*|-[1-9]+[0-9]*|0))"))
			{
				isValidLiteral(IntLiteral, token);
				type = IntLiteral;

				if (!isLexTable)
				{
					Entry literal;

					literal.type = Literal;
					literal.name = "Literal_" + std::to_string(literalNumber);
					literalNumber++;
					literal.valueType = IntLiteral;
					literal.scope = "none";
					literal.line = line;
					literal.pos = counter;
					literal.value = std::stoi(token, nullptr, 10);

					idTable.Add(literal);
				}
			}
			else if (regex.Match(token, "(0b(1|0)+)"))
			{
				auto newTocken = token.erase(0, 2);
				isValidLiteral(IntLiteral, newTocken, 2);
				type = IntLiteral;

				if (!isLexTable)
				{
					Entry literal;

					literal.type = Literal;
					literal.name = "Literal_" + std::to_string(literalNumber);
					literalNumber++;
					literal.valueType = IntLiteral;
					literal.scope = "none";
					literal.line = line;
					literal.pos = counter;
					literal.value = std::stoi(token, nullptr, 8);

					idTable.Add(literal);
				}
			}
			else if (regex.Match(token, "((0[1-7]+[0-9]*|-0[1-7]+[0-9]*))"))
			{
				std::string newTocken;
				if (token[0] == '-')
				{
					newTocken = token.erase(1, 1);
				}
				else
				{
					newTocken = token.erase(0, 1);
				}
				isValidLiteral(IntLiteral, newTocken, 8);
				type = IntLiteral;

				if (!isLexTable)
				{
					Entry literal;

					literal.type = Literal;
					literal.name = "Literal_" + std::to_string(literalNumber);
					literalNumber++;
					literal.valueType = IntLiteral;
					literal.scope = "none";
					literal.line = line;
					literal.pos = counter;
					literal.value = std::stoi(token, nullptr, 8);

					idTable.Add(literal);
				}
			}
			else if (regex.Match(token, "((0h([A-F]+|[1-9])+[0-9]*[A-F]*|-0h([A-F]+|[1-9])+[0-9]*[A-F]*))"))
			{
				std::string newTocken;
				if (token[0] == '-')
				{
					newTocken = token.erase(1, 2);
				}
				else
				{
					newTocken = token.erase(0, 2);
				}
				isValidLiteral(IntLiteral, newTocken, 16);
				type = IntLiteral;

				if (!isLexTable)
				{
					Entry literal;

					literal.type = Literal;
					literal.name = "Literal_" + std::to_string(literalNumber);
					literalNumber++;
					literal.valueType = IntLiteral;
					literal.scope = "none";
					literal.line = line;
					literal.pos = counter;
					literal.value = std::stoi(token, nullptr, 16);

					idTable.Add(literal);
				}
			}
			else if (regex.Match(token, "(([0-9]+.[0-9]+|-[0-9]+.[0-9]+))"))
			{
				isValidLiteral(DoubleLiteral, token);
				type = DoubleLiteral;

				if (!isLexTable)
				{
					Entry literal;

					literal.type = Literal;
					literal.name = "Literal_" + std::to_string(literalNumber);
					literalNumber++;
					literal.valueType = DoubleLiteral;
					literal.scope = "none";
					literal.line = line;
					literal.pos = counter;
					literal.value = std::stod(token);

					idTable.Add(literal);
				}
			}
			else if (regex.Match(token, "((true|false))"))
			{
				type = BoolLiteral;
				if (!isLexTable)
				{
					Entry literal;

					literal.type = Literal;
					literal.name = "Literal_" + std::to_string(literalNumber);
					literalNumber++;
					literal.valueType = BoolLiteral;
					literal.scope = "none";
					literal.line = line;
					literal.pos = counter;
					
					if (token == "true")
					{
						literal.value = true;
					}
					else
					{
						literal.value = false;
					}

					idTable.Add(literal);
				}
			}
			else if (regex.Match(token, "(([A-Z]|[a-z])(([A-Z]|[a-z])|[0-9])*)")
					 && !TokenTypes.contains(token))
			{
				type = Id;
			}
			else
			{
				ERROR_LOG(std::format("Sourse code: строка {}, индекс лексемы {}", line, counter),
						  std::format("Нераспознаная лексема {}", token));
				throw "Esception";
			}
		}
		return type;
	}

	std::string LEXER::Lexer::scopeGenerator(string& priviosScope, string& id)
	{
		std::string newScope = priviosScope;
		currentScopeNumber += 1;
		std::string uHash = std::to_string(currentScopeNumber);

		int levelNumber = stoi(string() + priviosScope[priviosScope.length() - 1]);

		std::string level = std::to_string(levelNumber + 1);
		newScope += '_' + string() + id[0] + uHash + level;

		scopes.push(newScope);
		return newScope;
	}

	void LEXER::Lexer::createFun(std::list<string>::iterator& currentToken, std::vector<Lexem>& lexTable, IdTable& idTable, std::list<string>& tokens, int& line, int& counter, bool& hasMain)
	{
		Entry function;
		Regex::Regex regex;
		currentToken++;
		counterVec++;

		const std::array<string, 2> expressions
		{
			"(int|string|bool|double)",
			"(([A-Z]|[a-z])(([A-Z]|[a-z])|[0-9])*)"
		};

		const std::array<std::string, 2> errorMessages
		{
			"Неверно указан тип",
			"Неверно задан идентификатор"
		};

		auto isExist = [&](Entry& entry)
			{
				if (idTable.Contains(entry))
				{
					if (entry.type == Variable)
					{

						int hhh = std::distance(preprocesseredStr.begin(), currentToken) - line + 1;
						std::cout << hhh << std::endl;
						lexTable[hhh].positionInIdTable = entry.GetHashCode();
					}
					ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", line, counter), "Множественная инициализация");
					throw "Exception";
				}
			};

		auto getTyoeAndId = [&](Entry& entry, size_t index)
			{
				for (size_t i = index, j = 0; j < 2; i++, j++)
				{
					do
					{
						if (*currentToken == "|")
						{
							line++;
							counter = 0;
							currentToken++;
							counterVec++;
						}
					} while (*currentToken == "|");

					if (regex.Match(*currentToken, expressions[j]))
					{
						if (i == 2)
						{
							if (GetKeyword(*currentToken, line, counter) != Id)
							{
								if (*currentToken == "main" && !hasMain)
								{
									hasMain = true;
								}
								else
								{
									ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", line, counter),
											  std::format("{} {}", errorMessages[j], *currentToken));
									throw "Exception";
								}
							}
							entry.name = *currentToken;
						}
						else
						{
							entry.valueType = GetKeyword(*currentToken, line, counter);
							switch (entry.valueType)
							{
							case Int:
								entry.value = 0;
								break;
							case Bool:
								entry.value = false;
								break;
							case Double:
								entry.value = 0.0;
								break;
							case String:
								entry.value = "";
								break;
							default:
								break;
							}
						}
						currentToken++;
						counterVec++;
					}
					else
					{
						ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", line, counter),
								  std::format("{} {}", errorMessages[j], *currentToken));
						throw "Exception";
					}
				}
			};


		function.type = Fun;

		getTyoeAndId(function, 1);

		function.line = line;
		function.pos = counter;

		if (scopes.top() != "g0")
		{
			ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", line, counter),
					  "Недопустимая вложенная функция");
			throw "Exception";
		}
		function.scope = scopes.top();
		function.ownScope = scopeGenerator(function.scope, function.name);
		function.value = None;

		isExist(function);
		idTable.Add(function);

		int countOfParams = 0;
		if (GetKeyword(*currentToken, line, counter) == OpenParmBracket)
		{
			Entry param;
			currentToken++;
			counterVec++;

			while (*currentToken != ")")
			{
				param.type = Param;

				getTyoeAndId(param, 1);
				idTable[function.GetHashCode()].paramTypes.push_back(param.valueType);

				param.line = line;
				param.pos = counter;

				param.scope = scopes.top();

				do
				{
					if (*currentToken == "|")
					{
						line++;
						counter = 0;
						currentToken++;
						counterVec++;
					}
				} while (*currentToken == "|");

				if (*currentToken != "," && *currentToken != ")")
				{
					ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", line, counter),
							  std::format("Неверная форма записи параметров {}", *currentToken));
					throw "Exception";
				}
				else
				{
					counter += 1;
				}

				isExist(param);

				idTable.Add(param);
				if (*currentToken == ")")
				{
					countOfParams++;
					break;
				}

				currentToken++;
				counterVec++;
				countOfParams++;
			}
		}
		else
		{
			ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", line, counter),
					  std::format("Неверная форма записи параметров {}", *currentToken));
			throw "Exception";
		}

		idTable[function.GetHashCode()].params = countOfParams;
	}

	inline void LEXER::Lexer::createVar(std::list<string>::iterator& currentToken, std::vector<Lexem>& lexTable, IdTable& idTable, std::list<string>& tokens, int& line, int& counter, Keywords type)
	{
		Entry var;
		Regex::Regex regex;
		counter--;

		const std::array<string, 2> expressions
		{
			"(int|string|bool|double)",
			"(([A-Z]|[a-z])(([A-Z]|[a-z])|[0-9])*)"
		};

		const std::array<std::string, 2> errorMessages
		{
			"Неверно указан тип",
			"Неверно задан идентификатор"
		};

		auto isExist = [&](Entry& entry)
			{
				if (idTable.Contains(entry))
				{
					if (entry.type == Variable)
					{
						int hhh = std::distance(preprocesseredStr.begin(), currentToken) - line + 1;
						std::cout << hhh << std::endl;
						lexTable[hhh].positionInIdTable = entry.GetHashCode();
					}
					ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", line, counter), "Множественная инициализация");
					throw "Exception";
				}
			};

		auto getTyoeAndId = [&](Entry& entry, size_t index)
			{
				for (size_t i = index, j = 0; j < 2; i++, j++)
				{
					do
					{
						if (*currentToken == "|")
						{
							line++;
							counter = 0;
							currentToken++;
							counterVec++;
						}
					} while (*currentToken == "|");

					if (regex.Match(*currentToken, expressions[j]))
					{
						if (i == 2)
						{
							if (GetKeyword(*currentToken, line, counter) != Id)
							{
								ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", line, counter),
										  std::format("{} {}", errorMessages[j], *currentToken));
								throw "Exception";
							}
							entry.name = *currentToken;
						}
						else
						{
							entry.valueType = GetKeyword(*currentToken, line, counter);
							switch (entry.valueType)
							{
							case Int:
								entry.value = 0;
								break;
							case Bool:
								entry.value = false;
								break;
							case Double:
								entry.value = 0.0;
								break;
							case String:
								entry.value = "";
								break;
							default:
								break;
							}
						}
						currentToken++;
						counterVec++;
					}
					else
					{
						ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", line, counter),
								  std::format("{} {}", errorMessages[j], *currentToken));
						throw "Exception";
					}
				}
			};

		var.type = Variable;
		getTyoeAndId(var, 1);

		var.line = line;
		var.pos = counter;
		var.scope = scopes.top();

		isExist(var);

		idTable.Add(var);
	}

	void LEXER::Lexer::createAuto(std::list<string>::iterator& currentToken, std::vector<Lexem>& lexTable, IdTable& idTable, std::list<string>& tokens, int& line, int& counter)
	{
		currentToken++;
		Entry autoType;

		auto isExist = [&](Entry& entry)
			{
				if (idTable.Contains(entry))
				{
					if (entry.type == Variable)
					{
						int hhh = std::distance(preprocesseredStr.begin(), currentToken) - line + 1;
						std::cout << hhh << std::endl;
						lexTable[hhh].positionInIdTable = entry.GetHashCode();
					}
					ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", line, counter), "Множественная инициализация");
					throw "Exception";
				}
			};

		auto idExists = [&](double& resultKey) -> bool {
			auto allScopes = scopes;
			bool check = false;
			while (!allScopes.empty())
			{
				auto scope = Utils::Pop(allScopes);
				double key = std::hash<string>()(*currentToken) +
					std::hash<string>()(scope);

				resultKey = key;

				if (idTable.Contains(resultKey))
				{
					if (idTable[resultKey].type == Variable)
					{
						int hhh = std::distance(preprocesseredStr.begin(), currentToken) - line + 1;
						std::cout << hhh << std::endl;
						lexTable[hhh].positionInIdTable = resultKey;
					}
					check = true;
					break;
				}
			}
			if (!check)
			{
				ERROR_LOG(std::format("Sourse code: строка {}, индекс лексемы {}", line, counter),
						  std::format("Необъявлееный идентификатор {}", *currentToken));
				throw "Esception";
			}
			return check;
			};

		auto setType = [&]() {
			switch (autoType.valueType)
			{
			case Int:
				autoType.value = 0;
				break;
			case Bool:
				autoType.value = false;
				break;
			case Double:
				autoType.value = 0.0;
				break;
			case String:
				autoType.value = "";
				break;
			default:
				break;
			}
			};
		Regex::Regex regex;
		autoType.scope = scopes.top();
		autoType.type = Variable;

		do
		{
			if (*currentToken == "|")
			{
				line++;
				counter = 0;
				currentToken++;
				counterVec++;
			}
		} while (*currentToken == "|");

		if (regex.Match(*currentToken, "(([A-Z]|[a-z])(([A-Z]|[a-z])|[0-9])*)"))
		{
			if (GetKeyword(*currentToken, line, counter) != Id)
			{
				ERROR_LOG(std::format("Sourse code: строка {}, лексема {}.", line, counter),
						  std::format("Неверно задан идентификатор {}", *currentToken));
				throw "Exception";
			}

			autoType.name = *currentToken;
			autoType.line = line;
			autoType.pos = counter;

			currentToken++;
			counterVec++;

			do
			{
				if (*currentToken == "|")
				{
					line++;
					counter = 0;
					currentToken++;
					counterVec++;
				}
			} while (*currentToken == "|");

			if (GetKeyword(*currentToken, line, counter) != Eqal)
			{
				ERROR_LOG(std::format("Sourse code: строка {}, индекс лексемы {}", line, counter),
						  "Для авто-типа необходимо использовать оператор присваивания");
				throw "Esception";
			}


		}
		else
		{
			ERROR_LOG(std::format("Sourse code: строка {}, индекс лексемы {}", line, counter),
					  std::format("Недопустимое именование идентификатора {}", *currentToken));
			throw "Esception";
		}

		Keywords currentType;
		stack<Keywords> brackets;
		do
		{
			currentToken++;
			counterVec++;
			do
			{
				if (*currentToken == "|")
				{
					line++;
					counter = 0;
					currentToken++;
					counterVec++;
				}
			} while (*currentToken == "|");

			currentType = GetKeyword(*currentToken, line, counter);

			switch (currentType)
			{
			case Id:
			{
				double key = 0;
				if (brackets.empty())
				{
					auto allScopes = scopes;
					if (idExists(key))
					{
						auto lexem = idTable[key];
						autoType.valueType = lexem.valueType;
						setType();
						break;
					}
					else
					{
						ERROR_LOG(std::format("Sourse code: строка {}, индекс лексемы {}", line, counter),
								  std::format("Необъявлееный идентификатор {}", *currentToken));
						throw "Esception";
					}
				}
				else
				{
					if (!idExists(key))
					{
						ERROR_LOG(std::format("Sourse code: строка {}, индекс лексемы {}", line, counter),
								  std::format("Необъявлееный идентификатор {}", *currentToken));
						throw "Esception";
					}
				}
				break;
			}
			case Plus:
				break;
			case Minus:
				break;
			case Star:
				break;
			case Delim:
				break;
			case More:
				break;
			case Less:
				break;
			case Comma:
				break;
			case IntLiteral:
				if (autoType.valueType == None)
				{
					autoType.valueType = Int;
					setType();
				}
				break;
			case BoolLiteral:
				if (autoType.valueType == None)
				{
					autoType.valueType = Bool;
					setType();
				}

				break;
			case DoubleLiteral:
				if (autoType.valueType == None)
				{
					autoType.valueType = Double;
					setType();
				}
				break;
			case StringLiteral:
				if (autoType.valueType == None)
				{
					autoType.valueType = String;
					setType();
				}
				break;
			case OpenParmBracket:
				brackets.push(OpenParmBracket);
				break;
			case CloseParmBracket:
				brackets.pop();
				break;
			case End:
				break;
			default:
				break;
			}

		} while (currentType != End);

		isExist(autoType);

		idTable.Add(autoType);
	}
}