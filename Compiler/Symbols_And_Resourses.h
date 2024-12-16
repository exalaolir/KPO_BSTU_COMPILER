#pragma once

#define IS_DEPRICATED_SYMBOL(i) code[i] == '\t' || code[i] == '\v'
#define MATH_SIMBOL(i, n) code[i - n] == '+' || code[i - n] == '-' || code[i - n] == '*' || code[i - n] == '/' || code[i - n] == '%' || code[i - n] == '(' || code[i - n] == '=' || code[i - n] == ',' || code[i - n] == ':' || code[i - n] == '!'  || code[i - n] == '>' || code[i - n] == '<' 
#define DEPRICATED_SYMBOL "none"

namespace LEXER
{
	enum Keywords
	{
		Int,
		String,
		Bool,
		Double,
		Char,
		Byte,
		UInt,

		IntLiteral,
		StringLiteral,
		BoolLiteral,
		DoubleLiteral,
		CharLiteral,
		ByteLiteral,
		UIntLiteral,

		Fun,
		Return,
		Main,

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
		{"byte", "t"},
		{"uint", "t"},
		{"char", "t"},
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
		{"uint", UInt},
		{"char", Char},
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
		{"<=", LessorEqal},//<=

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
		{':', true},
		{'<', true}
	};

	inline static const std::unordered_map<std::string, char> SpecialSymbols
	{
		{"\\n", '\n'},
		{"\\t", '\t'},
		{"\\v", '\v'},
	};

	inline static const std::unordered_map<std::string, std::string> Functions
	{
		{"Print", "Print"},
		{"Println", "Println"},
		{"IToString", "IToString"},
		{"BoolToString", "BoolToString"},
		{"FToString", "FToString"},
		{"Concat", "Concat"},
		{"Absb", "Absb"},
		{"UIToString", "UIToString"},
		{"CharToString", "CharToString"},
	};

	inline static const std::unordered_map<Keywords, string> stringInfo
	{
		{Int, "int"},
		{String, "string"},
		{Double, "double"},
		{Bool, "bool"},
		{Char, "char"},
		{UInt, "uint"},
		{Byte, "byte"},
		{IntLiteral, "int"},
		{StringLiteral, "string"},
		{DoubleLiteral, "double"},
		{BoolLiteral, "bool"},
		{CharLiteral, "char"},
		{UIntLiteral, "uint"},
		{ByteLiteral, "byte"},
		{Literal, "literal"},
		{Fun, "fn"},
		{Return, "return"},
		{Main, "main"},
		{If, "if"},
		{Else, "else"},
		{For, "for"},
		{While, "while"},
		{ServisSymbol, ";"},
		{ServisSymbol, ","},
		{OpenBracket, "{"},
		{CloseBracket, "}"},
		{Param, "param"},
		{Variable, "var"},
		{None, "none"}
	};
}