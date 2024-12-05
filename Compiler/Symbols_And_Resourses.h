#pragma once

#define IS_DEPRICATED_SYMBOL(i) code[i] == '\t' || code[i] == '\v'
#define MATH_SIMBOL(i, n) code[i - n] == '+' || code[i - n] == '-' || code[i - n] == '*' || code[i - n] == '/' || code[i - n] == '%' || code[i - n] == '(' || code[i - n] == '=' || code[i - n] == ',' 
#define DEPRICATED_SYMBOL "none"

namespace LEXER
{
	enum Keywords
	{
		Int,
		String,
		Bool,
		Double,

		Fun,
		Return,
		Main,

		IntLiteral,
		StringLiteral,
		BoolLiteral,
		DoubleLiteral,

		If,
		Else,
		For,
		While,

		Param,
		Id,

		OpenBracket,
		CloseBracket,
		ServisSymbol,
		OpenParmBracket,
		CloseParmBracket,
		Comma,

		None,
		Variable,
		Let,
		End,

		Plus,
		Minus,
		Star,
		Delim,
		More,
		Less,
		opDelim,
		Eqal,

		IfEqal,
		IfNoEqal,
		MoreorEqal,
		LessorEqal,

		ShiftRight,
		ShiftLeft,

		Literal
	};

	inline static const std::unordered_map<string, string> Lexems
	{
		{"int", "t"},
		{"string", "t"},
		{"double", "t"},
		{"bool", "t"},
		{"let", "t"},
		{"fn", "f"},
		{"return", "r"},
		{"main", "m"},

		{"if", "q"},
		{"else", "s"},
		{"for", "ñ"},
		{"while", "w"},

		{";", ";"},
		{",", ","},
		{"{", "{"},
		{"}", "}"},
		{"(", "("},
		{")", ")"},
		{"+", "o"},
		{"-", "o"},
		{"*", "o"},
		{"/", "o"},
		{"=", "="},
		{"%", "o"},
		{">>", "o"},
		{"<<", "o"},

		{":", "u"},//==
		{":!", "u"},//!=
		{">=", "u"},//>=
		{"<=", "u"},//<=
		{">", "u"},
		{"<", "u"},
	};

	inline static const unordered_map<std::string, Keywords> TokenTypes
	{
		{"int", Int},
		{"string", String},
		{"double", Double},
		{"bool", Bool},
		{"fn", Fun},
		{"return", Return},
		{"main", Main},
		{"if", If},
		{"else", Else},
		{"for", For},
		{"while", While},
		{"let", Let},
		{";", End},
		{",", Comma},
		{"{", OpenBracket},
		{"}", CloseBracket},
		{"(", OpenParmBracket},
		{")", CloseParmBracket},
		{"+", Plus},
		{"-", Minus},
		{"*", Star},
		{"/", Delim},
		{"%", opDelim},
		{"=", Eqal},

		{":", IfEqal},//==
		{":!", IfNoEqal},//!=
		{">=", MoreorEqal},//>=
		{"=<", LessorEqal},//<=

		{">", More},
		{"<", Less},
		{"<<", ShiftLeft},//<<
		{">>", ShiftRight},//>>
	};

	inline static const std::unordered_map<char, bool> Spaces
	{
		{'/', true},
		{'*', true},
		{'+', true},
		{'%', true},
		{'=', true},
		{'(', true},
		{')', true},
		{'{', true},
		{'}', true},
		{',', true},
		{';', true},
		{'-', true},
		{'|', true},
		{'!', true},
		{'>', true},
		{'<', true}
	};

	inline static const std::unordered_map<std::string, char> SpecialSymbols
	{
		{"\\n", '\n'},
		{"\\t", '\t'},
		{"\\v", '\v'},
	};
}