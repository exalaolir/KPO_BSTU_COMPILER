#pragma once

#define INPUT_PATH "in.txt"
#define OUTPUT_PATH "out.txt.out"
#define LOG_PATH "log.txt.out"

namespace INPUTVAL
{
	enum Files
	{
		IN_FILE = 0,
		LOG_FILE,
		OUT_FILE
	};
	std::array<string, 3> validateParams(int& countOfArgs, char* args[]);
}

