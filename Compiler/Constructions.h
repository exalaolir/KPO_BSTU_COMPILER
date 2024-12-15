#pragma once
#include "pch.h"
#include "Lexer.h"

namespace GEN
{
	static const std::string BEGIN_OF_FILE = ".686p\n.xmm\n.MODEL FLAT, STDCALL\nincludelib kernel32.lib\nincludelib msvcrt.lib\nincludelib ucrt.lib\nincludelib vcruntime.lib\nincludelib msvcprt.lib\nincludelib standartLib.lib\nPrint PROTO arg:ptr byte\nPrintln PROTO arg:ptr byte\nIToString PROTO arg:sdword\nUIToString PROTO arg:dword\nFToString PROTO arg:real8\nBoolToString PROTO arg:sdword\nConcat PROTO arg1:ptr byte, arg2:ptr byte\nAbsb PROTO arg:sword\nCharToString PROTO arg:sdword\nExitProcess PROTO:DWORD\n.STACK 4096\n";

	static const bool IS_DOUBLE = true;

	static const std::string DATA = ".DATA";
	static const std::string CODE = ".CODE\n";
	static const std::string LOCAL = "local";
	static const std::string END_MAIN = "end main";
	static const std::string PROC = "option prologue:PrologueDef\noption epilogue:EpilogueDef\n";

	static const std::string DIV_WITH_PERCENTS = "xor EDX,EDX\npop EBX\ncmp EBX, 0\nje null_exception\npop EAX\nidiv EBX\npush EDX\n";
	static const std::string DIV = "xor EDX,EDX\npop EBX\ncmp EBX, 0\nje null_exception\npop EAX\ndiv EBX\npush EAX\n";
	static const std::string DIV_WITH_PERCENTS_U = "xor EDX,EDX\npop EBX\ncmp EBX, 0\nje null_exception\npop EAX\ndiv EBX\npush EDX\n";
	static const std::string DIV_U = "xor EDX,EDX\npop EBX\ncmp EBX, 0\nje null_exception\npop EAX\ndiv EBX\npush EAX\n";
	static const std::string ADD = "pop EBX\npop EAX\nadd EAX, EBX\npush EAX\n";
	static const std::string SUB = "pop EBX\npop EAX\nsub EAX, EBX\npush EAX\n";
	static const std::string IMUL = "pop EBX\npop EAX\nimul EAX, EBX\npush EAX\n";
	static const std::string SHIFT_LEFT = "pop eax\npop ecx\nshl eax, cl\npush eax";
	static const std::string SHIFT_RIGHT = "pop eax\npop ecx\nshr eax, cl\npush eax";
	static const std::string MUL = "pop EBX\npop EAX\nmul EAX, EBX\npush EAX\n";

	static const std::string F_ADD = "fadd\n";
	static const std::string F_SUB = "fsub\n";
	static const std::string F_MUL = "fmul\n";
	static const std::string F_DIV = "fdiv\n";

	static const std::string PUSH_REAL_PARAM = "fstp real_buff\nlea eax, real_buff\npush sdword ptr [eax+4]\npush sdword ptr [eax]\n";

	static const std::string LESS = "pop ebx\npop eax\ncmp eax, ebx\njl ";
	static const std::string MORE = "pop ebx\npop eax\ncmp eax, ebx\njg ";
	static const std::string LESS_OR_EQAL = "pop ebx\npop eax\ncmp eax, ebx\njle ";
	static const std::string MORE_OR_EQAL = "pop ebx\npop eax\ncmp eax, ebx\njge ";
	static const std::string EQAL = "pop eax\npop ebx\ncmp eax, ebx\nje ";
	static const std::string NO_EQAL = "pop eax\npop ebx\ncmp eax, ebx\njne ";

	static const std::string SSE_MOVEMENT = "";

	static const std::string LESS_SSE = "fstp real_buff\nmovsd xmm1, real_buff\nfstp real_buff\nmovsd xmm0, real_buff\ncmpsd xmm0, xmm1, 1\nmovd eax, xmm0\nand eax, eax\njnz ";
	static const std::string MORE_SSE = "fstp real_buff\nmovsd xmm1, real_buff\nfstp real_buff\nmovsd xmm0, real_buff\ncmpsd xmm0, xmm1, 6\nmovd eax, xmm0\nand eax, eax\njnz ";
	static const std::string LESS_OR_EQAL_SSE = "fstp real_buff\nmovsd xmm1, real_buff\nfstp real_buff\nmovsd xmm0, real_buff\ncmpsd xmm0, xmm1, 2\nmovd eax, xmm0\nand eax, eax\njnz ";
	static const std::string MORE_OR_EQAL_SSE = "fstp real_buff\nmovsd xmm1, real_buff\nfstp real_buff\nmovsd xmm0, real_buff\ncmpsd xmm0, xmm1, 5\nmovd eax, xmm0\nand eax, eax\njnz ";
	static const std::string EQAL_SSE = "fstp real_buff\nmovsd xmm1, real_buff\nfstp real_buff\nmovsd xmm0, real_buff\ncmpsd xmm0, xmm1, 0\nmovd eax, xmm0\nand eax, eax\njnz ";
	static const std::string NO_EQAL_SSE = "fstp real_buff\nmovsd xmm1, real_buff\nfstp real_buff\nmovsd xmm0, real_buff\ncmpsd xmm0, xmm1, 4\nmovd eax, xmm0\nand eax, eax\njnz ";

	static const std::string NULL_EXCEPTION = "null_exception:\nlea eax, null_err\npush eax\ncall Println\npop eax\nINVOKE ExitProcess, -1\n";

	static const std::string EQAL_STR = "pop esi\npop edi\nmov ecx, 256\nrepe cmpsb\njz ";
	static const std::string NO_EQAL_STR = "pop esi\npop edi\nmov ecx, 256\nrepe cmpsb\njnz ";
	
	const std::unordered_map<std::string, std::string> operatorsBool
	{
		{"<", LESS},
		{">", MORE},
		{"<=", LESS_OR_EQAL},
		{">=", MORE_OR_EQAL},
		{":", EQAL},
		{":!", NO_EQAL}
	};

	const std::unordered_map<std::string, std::string> operatorsSse
	{
		{"<", LESS_SSE},
		{">", MORE_SSE},
		{"<=", LESS_OR_EQAL_SSE},
		{">=", MORE_OR_EQAL_SSE},
		{":", EQAL_SSE},
		{":!", NO_EQAL_SSE}
	};

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

	const std::unordered_map<std::string, std::string> operatorsUInt
	{
		{"+", ADD},
		{"-", SUB},
		{"*", MUL},
		{"/", DIV_U},
		{"%", DIV_WITH_PERCENTS_U},
		{"<<", SHIFT_LEFT},
		{">>", SHIFT_RIGHT},
	};

	const std::unordered_map<std::string, std::string> operatorsStr
	{
		{":", EQAL_STR},
		{":!", NO_EQAL_STR}
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
			auto end = std::format("\nINVOKE ExitProcess, eax\n{}\n{} endp\nend {}", NULL_EXCEPTION, name, name, name);
			std::list<std::string> result{ prolog, end };
			return result;
		};

	const auto MAKE_VAR = [](std::string name, std::string type, std::string val) -> std::string
		{
			return name + " " + type + " " + val;
		};

	const auto MAKE_PARAM = [](std::string name, std::string type, Keywords eType = None, bool isLockalStr = false) -> std::string
		{
			if (eType == String)
			{
				name += "_S";
				type = "ptr byte";
			}
			if(isLockalStr) name +="[256]";
			std::string result = std::format("{}:{}, ", name, type);
			return result;
		};

	const auto MAKE_NAME = [](LEXER::Entry entry) -> std::string
		{
			if (Functions.contains(entry.name) && entry.type == Fun)
			{
				return entry.name;
			}
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

	const auto POP = [](auto val, bool isDouble = false, bool isReturn = false, bool isString = false, size_t index = 0, Keywords typeFun = None) -> std::string
		{
			if (isString)
			{
				if (!isReturn)
					return std::format("pop esi\nlea edi, {}\n copy_loop_{}:\nlodsb\nstosb\ntest al, al\njnz copy_loop_{}\n", val, index, index);
				else
					return "\npop eax\nret\n";
			}
			if (!isDouble)
			{
				if(!isReturn)
					return "pop " + val + "\n";
				else
					if(typeFun == Main)
						return "\npop eax\n";
					else return "\npop eax\nret\n";
			}
			else
			{
				if (!isReturn)
					return "fstp " + val + "\n";
				else
					return "fstp real_buff\nfld qword ptr [real_buff]\nret\n";
			}
		};

	const auto PUSH = [](auto val, bool isDouble = false, bool isString = false) -> std::string
		{
			if(isString) return std::format("lea eax, {}\npush eax\n", val);
			if (!isDouble)
			{
				return "push " + val + "\n";
			}
			else
			{
				return "fld " + val + "\n";
			}
		};

	const auto CALL = [](auto val, bool isDouble = false) -> std::string
		{
			if (!isDouble)
			{
				return "call " + val + "\n" + "push eax\n";
			}
			else
			{
				return "call " + val + "\n" + "fstp real_buff\nfld real_buff\n";
			}
		};

	const auto MAKE_BOOKMARK = [](Keywords type, size_t id) -> std::string
		{
			if (type == If)
			{
				return "If_l" + std::to_string(id);
			}
			else if (type == Else)
			{
				return "Else" + std::to_string(id);
			}
		};

	const auto MAKE_BOOKMARK_WHILE = [](Keywords type, size_t id) -> std::string
		{
			if (type == If)
			{
				return "While_l" + std::to_string(id);
			}
			else if (type == Else)
			{
				return "Else_While" + std::to_string(id);
			}
		};

	const auto MAKE_ENDIF = [](Keywords type, size_t id) -> std::string
		{
			if (type == If)
			{
				return "Endif" + std::to_string(id) + ":\n";
			}
		};

	const auto MAKE_WHILE = [](size_t id) -> std::string
		{
				return "While_start" + std::to_string(id) + ":\n";	
		};

	const auto MAKE_END_WHILE = [](size_t id) -> std::string
		{
				return "jmp While_start" + std::to_string(id) + "\n"
					+ "Else_While" + std::to_string(id) + ":" + "\n";
			
		};

	const auto MAKE_END_MARK = [](Keywords type, size_t id) -> std::string
		{
			if (type == If)
			{
				return "jmp Endif" + std::to_string(id) + "\n"
					+ "Else" + std::to_string(id) + ":" + "\n";
			}
		};

	const auto MAKE_IF = [](std::string op, auto bookmark, auto elseBookmark, bool isDouble = false, bool isString = false) -> std::string
		{
			if (isString)
			{
				return operatorsStr.at(op) + bookmark + "\n" + "jmp " + elseBookmark + "\n" + bookmark + ":" + "\n";
			}
			if (!isDouble)
			{
				return operatorsBool.at(op) + bookmark + "\n" + "jmp " + elseBookmark + "\n" + bookmark + ":" + "\n";
			}
			else
			{
				return operatorsSse.at(op) + bookmark + "\n" + "jmp " + elseBookmark + "\n" + bookmark + ":" + "\n";
			}
		};

	const auto MAKE_COPY = [](std::string sourse, std::string destination, size_t index, bool isParam = false) -> std::string
		{
			if (isParam)
			{
				return std::format("mov esi, {}\nlea edi, {}\n copy_loop_{}:\nlodsb\nstosb\ntest al, al\njnz copy_loop_{}\n", sourse, destination, index, index);
			}
		};
}