#pragma once

#define INPUT_PATH "in.txt"
#define OUTPUT_PATH "out.asm"
#define LOG_PATH "log.txt.log"
#define PROTOCOL_PATH "protocol.txt"

namespace INPUTVAL
{
	enum Files
	{
		IN_FILE = 0,
		LOG_FILE,
		OUT_FILE,
		PROTOCOL_FILE
	};
	std::array<string, 4> validateParams(int& countOfArgs, char* args[]);
}

