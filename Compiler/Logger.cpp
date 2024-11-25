#include "pch.h"
#include "Logger.h"

void LOGGER::Logger::Log(Level level, const std::string sourse, const std::string message)
{
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);

	std::string logMessage;

	std::string hours = std::to_string(newtime.tm_hour);
	if (hours.length() == 1) hours = "0" + hours;

	std::string minutes = std::to_string(newtime.tm_min);
	if (minutes.length() == 1) minutes = "0" + minutes;

	std::string seconds = std::to_string(newtime.tm_sec);
	if (seconds.length() == 1) seconds = "0" + seconds;

	logMessage = std::format("[{}:{}:{}] {} Источник: {}, Сообщение: {}\n",
							 hours, minutes, seconds,
							 this->messageLevel[level], sourse, message);


	std::cout << std::format("[{}:{}:{}] ",
							 hours, minutes, seconds);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, this->colors[level]);

	std::cout << this->messageLevel[level];

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	std::cout << std::format(" Источник: {}, Сообщение: {}\n", sourse, message);
	
	this->fileLogger.Write(logMessage);
}

LOGGER::FileLog::~FileLog()
{
	if (logFile.is_open())
	{
		logFile.close();
	}
}

void LOGGER::FileLog::Write(const std::string& data)
{

	if (!this->logFile.is_open())
	{
		logFile.open(this->path);
	}
	if (!this->logFile.is_open())
	{
		std::string info = __FILE__ + std::string() + " : " + std::to_string(__LINE__);
		std::cout << "Ошибка пути лог файла " << info << std::endl;
	}

	this->logFile << data;
}

