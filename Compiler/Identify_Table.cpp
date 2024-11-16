#include "pch.h"
#include "Lexer.h"
#include "Automat.h"
#include "Symbols_And_Resourses.h"

namespace LEXER
{
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

	IdTable LEXER::Lexer::generateIdTable()
	{
		IdTable idTable;
		std::stack<Keywords> brackets;
		int line = 1;
		int counter = 0;
		bool hasMain = false;

		for (auto token = preprocesseredStr.begin(); token != preprocesseredStr.end(); token++)
		{
			switch (GetKeyword(*token, line, counter))
			{
			case Fun:
				createFun(token, idTable, preprocesseredStr, line, counter, hasMain);
				break;
			case Int:
				createVar(token, idTable, preprocesseredStr, line, counter, Int);
				break;
			case String:
				createVar(token, idTable, preprocesseredStr, line, counter, String);
				break;
			case Double:
				createVar(token, idTable, preprocesseredStr, line, counter, Double);
				break;
			case Bool:
				createVar(token, idTable, preprocesseredStr, line, counter, Bool);
				break;
			case Let:
				createAuto(token, idTable, preprocesseredStr, line, counter);
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
		return idTable;
	}

	Keywords LEXER::Lexer::GetKeyword(string& token, int& line, int& counter)
	{
		Keywords type;
		counter += 1;

		auto isValidLiteral = [=](Keywords type, std::string& literal) {
			try
			{
				double val;
				switch (type)
				{

				case LEXER::IntLiteral:
					std::stoi(literal);
					break;
				case LEXER::DoubleLiteral:
					std::stod(literal);
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
			}
			else if (token == "|")
			{
				line += 1;
				counter = 0;
				return None;
			}
			else if (regex.Match(token, "(([1-9]+[0-9]*|-[1-9]+[0-9]*))"))
			{
				isValidLiteral(IntLiteral, token);
				type = IntLiteral;
			}
			else if (regex.Match(token, "(0b(1|0)+)"))
			{
				isValidLiteral(IntLiteral, token);
				type = IntLiteral;
			}
			else if (regex.Match(token, "((0[1-7]+[0-9]*|-0[1-7]+[0-9]*))"))
			{
				isValidLiteral(IntLiteral, token);
				type = IntLiteral;
			}
			else if (regex.Match(token, "((0x([A-F]+|[1-9])+[0-9]*[A-F]*|-0x([A-F]+|[1-9])+[0-9]*[A-F]*))"))
			{
				isValidLiteral(IntLiteral, token);
				type = IntLiteral;
			}
			else if (regex.Match(token, "(([0-9]+.[0-9]+|-[0-9]+.[0-9]+))"))
			{
				isValidLiteral(DoubleLiteral, token);
				type = DoubleLiteral;
			}
			else if (regex.Match(token, "((true|false))"))
			{
				type = BoolLiteral;
			}
			else if (regex.Match(token, "([A-z]([A-z]|[0-9])*)")
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
		newScope += '-' + string() + id[0] + uHash + level;

		scopes.push(newScope);
		return newScope;
	}

	void LEXER::Lexer::createFun(std::list<string>::iterator& currentToken, IdTable& idTable, std::list<string>& tokens, int& line, int& counter, bool& hasMain)
	{
		Entry function;
		Regex::Regex regex;
		currentToken++;

		const std::array<string, 2> expressions
		{
			"(int|string|bool|double)",
			"([A-z]([A-z]|[0-9])*)"
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

			while (*currentToken != ")")
			{
				param.type = Param;

				getTyoeAndId(param, 1);

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
				if (*currentToken == ")") break;

				currentToken++;
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

	inline void LEXER::Lexer::createVar(std::list<string>::iterator& currentToken, IdTable& idTable, std::list<string>& tokens, int& line, int& counter, Keywords type)
	{
		Entry var;
		Regex::Regex regex;
		counter--;

		const std::array<string, 2> expressions
		{
			"(int|string|bool|double)",
			"([A-z]([A-z]|[0-9])*)"
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

	void LEXER::Lexer::createAuto(std::list<string>::iterator& currentToken, IdTable& idTable, std::list<string>& tokens, int& line, int& counter)
	{
		currentToken++;
		Entry autoType;

		auto isExist = [&](Entry& entry)
			{
				if (idTable.Contains(entry))
				{
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
			}
		} while (*currentToken == "|");

		if (regex.Match(*currentToken, "([A-z]([A-z]|[0-9])*)"))
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

			do
			{
				if (*currentToken == "|")
				{
					line++;
					counter = 0;
					currentToken++;
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
			do
			{
				if (*currentToken == "|")
				{
					line++;
					counter = 0;
					currentToken++;
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
						if (autoType.valueType == None)
						{
							autoType.valueType = lexem.valueType;
							setType();
						}
						else if (autoType.valueType != lexem.valueType)
						{
							ERROR_LOG(std::format("Sourse code: строка {}, индекс лексемы {}", line, counter),
									  std::format("Невозможно вывести авто-тип, разные типы: {}", *currentToken));
							throw "Esception";
						}
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
			case IntLiteral:
				if (autoType.valueType == None)
				{
					autoType.valueType = Int;
					setType();
				}
				else if (autoType.valueType != Int)
				{
					ERROR_LOG(std::format("Sourse code: строка {}, индекс лексемы {}", line, counter),
							  std::format("Невозможно вывести авто-тип, разные типы: {}", *currentToken));
					throw "Esception";
				}
				break;
			case BoolLiteral:
				if (autoType.valueType == None)
				{
					autoType.valueType = Bool;
					setType();
				}
				else if (autoType.valueType != Bool)
				{
					ERROR_LOG(std::format("Sourse code: строка {}, индекс лексемы {}", line, counter),
							  std::format("Невозможно вывести авто-тип, разные типы: {}", *currentToken));
					throw "Esception";
				}
				break;
			case DoubleLiteral:
				if (autoType.valueType == None)
				{
					autoType.valueType = Double;
					setType();
				}
				else if (autoType.valueType != Double)
				{
					ERROR_LOG(std::format("Sourse code: строка {}, индекс лексемы {}", line, counter),
							  std::format("Невозможно вывести авто-тип, разные типы: {}", *currentToken));
					throw "Esception";
				}
				break;
			case StringLiteral:
				if (autoType.valueType == None)
				{
					autoType.valueType = String;
					setType();
				}
				else if (autoType.valueType != String)
				{
					ERROR_LOG(std::format("Sourse code: строка {}, индекс лексемы {}", line, counter),
							  std::format("Невозможно вывести авто-тип, разные типы: {}", *currentToken));
					throw "Esception";
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
				ERROR_LOG(std::format("Sourse code: строка {}, индекс лексемы {}", line, counter),
						  std::format("Невозможно вывести авто-тип, неопознанная лексема: {}", *currentToken));
				throw "Esception";
				break;
			}

		} while (currentType != End);

		isExist(autoType);

		idTable.Add(autoType);
	}
}