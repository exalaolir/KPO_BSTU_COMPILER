#pragma once
#include "pch.h"
#include "Lexer.h"

namespace GEN
{
	static const std::string BEGIN_OF_FILE =
		".586P\n\
		 .MODEL FLAT, STDCALL \n\
		 includelib kernel32.lib\n\
		 includelib masm32.lib\n\
	     ExitProcess PROTO:DWORD\n\
		 .STACK 4096\n";

	static const bool IS_DOUBLE = true;

	static const std::string DATA = ".DATA";
	static const std::string CODE = ".CODE\n";
	static const std::string LOCAL = "local";
	static const std::string END_MAIN = "end main";
	static const std::string PROC = "option prologue:PrologueDef\noption epilogue:EpilogueDef\n";

	static const std::string DIV_WITH_PERCENTS = "xor EDX,EDX\npop EBX\npop EAX\ndiv EBX\npush EDX\n";
	static const std::string DIV = "xor EDX,EDX\npop EBX\npop EAX\ndiv EBX\npush EAX\n";
	static const std::string ADD = "pop EBX\npop EAX\nadd EAX, EBX\npush EAX\n";
	static const std::string SUB = "pop EBX\npop EAX\nsub EAX, EBX\npush EAX\n";
	static const std::string IMUL = "pop EBX\npop EAX\nimul EAX, EBX\npush EAX\n";
	static const std::string SHIFT_LEFT = "pop eax\npop ecx\nshl eax, cl\npush eax";
	static const std::string SHIFT_RIGHT = "pop eax\npop ecx\nshr eax, cl\npush eax";

	static const std::string F_ADD = "fadd\n";
	static const std::string F_SUB = "fsub\n";
	static const std::string F_MUL = "fmul\n";
	static const std::string F_DIV = "fdiv\n";

	const std::unordered_map<std::string, std::string> operatorsInt
	{
		{"+", ADD},
		{"-", SUB},
		{"*", IMUL},
		{"/", DIV},
		{"%", DIV_WITH_PERCENTS},
		{"<<", SHIFT_LEFT},
		{">>", SHIFT_RIGHT},
	};

	const std::unordered_map<std::string, std::string> operatorsDouble
	{
		{"+", F_ADD},
		{"-", F_SUB},
		{"*", F_MUL},
		{"/", F_DIV}
	};

	const auto MAKE_FUN = [](std::string name, std::string params) -> std::list<std::string>
		{
			std::string prolog = std::format(
				"{} proc {}\n", name, params);
			auto end = std::format("ret\n{} endp\n", name);
			std::list<std::string> result{ prolog, end };
			return result;
		};

	const auto MAKE_MAIN = [](std::string name) -> std::list<std::string>
		{
			std::string prolog = std::format(
				"{} proc\n", name);
			auto end = std::format("\nINVOKE ExitProcess, 0\n{} endp\nend {}", name, name);
			std::list<std::string> result{ prolog, end };
			return result;
		};

	const auto MAKE_VAR = [](std::string name, std::string type, std::string val) -> std::string
		{
			return name + " " + type + " " + val;
		};

	const auto MAKE_PARAM = [](std::string name, std::string type) -> std::string
		{
			std::string result = std::format("{}:{}, ", name, type);
			return result;
		};

	const auto MAKE_NAME = [](LEXER::Entry entry) -> std::string
		{
			return entry.name + "__" + entry.scope;
		};

	std::string MAKE_VAL(std::string val)
	{
		std::string result = val;

		if (result[result.length() - 1] == '\"')
		{
			result += ", 0";
		}
		return result;
	};

	const auto MAKE_LOCALS = [](std::string locals) -> std::list<std::string>
		{
			return { "local " + locals + "\n" };
		};

	const auto POP = [](auto val, bool isDouble = false) -> std::string
		{
			if (!isDouble)
			{
				return "pop " + val + "\n";
			}
			else
			{
				return "fstp " + val + "\n";
			}
		};

	const auto PUSH = [](auto val, bool isDouble = false) -> std::string
		{
			if (!isDouble)
			{
				return "push " + val + "\n";
			}
			else
			{
				return std::format(" lea eax, {}\n push sdword ptr [eax+4]\npush sdword ptr [eax]\n", val);
			}
		};
}