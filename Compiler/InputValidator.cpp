#include "pch.h"
#include "InputValidator.h"
#include "Automat.h"

std::array<string, 3> INPUTVAL::validateParams(int& countOfArgs, char* args[])
{
	std::vector<string> params;
	std::array<string, 3> paths{ INPUT_PATH, OUTPUT_PATH, LOG_PATH };

	params.reserve(countOfArgs - 1);

	if (countOfArgs <= 1 || countOfArgs > 4)
	{
		ERROR_LOG("params:", "�������� ����� ���������� �����������");
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
		WARN_LOG("-log:param", "���������� ������������ ��������� .log ����. ����� ������ ���� log.txt.log");
		INFO_LOG("-log:param", "����������� ������ -log:<name>.txt.log");
	}

	if (regex.Match(params[IN_FILE], "(-in:[ -z]+.txt)"))
	{
		paths[IN_FILE] = params[IN_FILE].substr(4);
	}
	else
	{
		ERROR_LOG("-in:param", "���������� ������������ ��������� -in ����.");
		INFO_LOG("-in:param", "����������� ������ -in:<name>.txt");
		throw "Exception";
	}

	if (regex.Match(params[OUT_FILE], "(-out:[ -z]+.txt.out)"))
	{
		paths[OUT_FILE] = params[OUT_FILE].substr(5);
	}
	else
	{
		WARN_LOG("-out:param", "���������� ������������ ��������� .out ����. ����� ������ ���� out.txt.out");
		INFO_LOG("-out:param", "����������� ������ -out:<name>.txt.out");
	}

	return paths;
}
