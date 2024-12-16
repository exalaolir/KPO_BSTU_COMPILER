#include "pch.h"
#include "InputValidator.h"
#include "Automat.h"

//using namespace INPUTVAL;
std::array<string, 4> INPUTVAL::validateParams(int& countOfArgs, char* args[])
{
	std::vector<string> params;
	std::array<string, 4> paths{ INPUT_PATH, OUTPUT_PATH, LOG_PATH, PROTOCOL_PATH };

	params.reserve(countOfArgs - 1);

	if (countOfArgs <= 1 || countOfArgs > 5)
	{
		ERROR_LOG("params:", "Неверное число параметров компилятора");
		throw "Exception";
	}

	for (size_t i = 1; i < countOfArgs; i++)
	{
		params.push_back(std::string() + args[i]);
	}

	std::sort(params.begin(), params.end());

	Regex::Regex regex;

	if (regex.Match(params[LOG_FILE], "(-log:[ -z]+.txt.log)"))
	{
		paths[LOG_FILE] = params[LOG_FILE].substr(5);
		LOGGER::Logger::path = paths[LOG_FILE];
	}
	else
	{
		WARN_LOG("-log:param", "Невозможно использовать указанный .log файл. Будет создан файл log.txt.log");
		INFO_LOG("-log:param", "Используйте шаблон -log:<name>.txt.log");
	}

	if (regex.Match(params[IN_FILE], "(-in:[ -z]+.txt)"))
	{
		paths[IN_FILE] = params[IN_FILE].substr(4);
	}
	else
	{
		ERROR_LOG("-in:param", "Невозможно использовать указанный -in файл.");
		INFO_LOG("-in:param", "Используйте шаблон -in:<name>.txt");
		throw "Exception";
	}

	if (regex.Match(params[OUT_FILE], "(-out:[ -z]+.asm)"))
	{
		paths[OUT_FILE] = params[OUT_FILE].substr(5);
	}
	else
	{
		WARN_LOG("-out:param", "Невозможно использовать указанный .out файл. Будет создан файл out.asm");
		INFO_LOG("-out:param", "Используйте шаблон -out:<name>.asm");
	}

	if (regex.Match(params[PROTOCOL_FILE], "(-prot:[ -z]+.txt)"))
	{
		paths[PROTOCOL_FILE] = params[PROTOCOL_FILE].substr(6);
	}
	else
	{
		WARN_LOG("-prot:param", "Невозможно использовать указанный .out файл. Будет создан файл prot.txt");
		INFO_LOG("-prot:param", "Используйте шаблон -prot:<name>.txt");
	}

	auto substr = paths[OUT_FILE];
	substr.erase(substr.length() - 4, 4);
	std::fstream bat;
	bat.open("C:\\labs\\Course\\Compiler\\Release\\CompilationOfGenCode.bat", std::ios::out);
	bat << std::format("@ECHO OFF\ntimeout 1\ncd C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\Build\ncall vcvarsall.bat x86\ncd C:\\labs\\Course\\Compiler\\Compiler\nml /c /coff {}.asm\nlink /OPT:NOREF /LTCG /SUBSYSTEM:CONSOLE kernel32.lib msvcrt.lib ucrt.lib vcruntime.lib msvcprt.lib standartLib.lib {}.obj\n call {}.exe\ntimeout 5\npause\nexit",substr, substr, substr);
	bat.close();
	return paths;
}
