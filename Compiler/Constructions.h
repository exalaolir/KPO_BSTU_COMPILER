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
		 .STACK 4096\n";

	static const std::string DATA = ".DATA";
	static const std::string LOCAL = "local";
	static const std::string END_MAIN = "end main";

	const auto MAKE_FUN = [](std::string& name, std::string params, auto& bytes) -> std::string
	{
			std::string result = std::format(
				"option prologue:PrologueDef\n\
				 option epilogue:EpilogueDef\n\
				 {} proc {}\n\
				 ret\n\
				 {} endp\n", params, name);
			auto end = name + " endp\n";
			bytes = end.size();
			return result;
	};

	const auto MAKE_VAR = [](std::string name, std::string type, std::string val) -> std::string 
	{
			return name + " " + type + " " + val;
	};

	const auto MAKE_NAME = [](LEXER::Entry& entry) -> std::string 
	{
			return entry.name + "__" + entry.scope;
	};

	const auto MAKE_VAL = [](auto& val) -> std::string
	{
			std::string result = std::to_string(val);

			if (std::is_same_v<val, std::string>)
			{
				result += ", 0";
			}
			return result;
	};
}