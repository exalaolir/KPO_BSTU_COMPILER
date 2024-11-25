#pragma once

#define SUCCESS_LOG(sourse, mes) LOGGER::GetLogger().Log(LOGGER::Logger::Level::SUCESS_LEVEL, (sourse), (mes));
#define INFO_LOG(sourse, mes) LOGGER::GetLogger().Log(LOGGER::Logger::Level::INFO_LEVEL, (sourse), (mes));
#define WARN_LOG(sourse, mes) LOGGER::GetLogger().Log(LOGGER::Logger::Level::WARN_LEVEL, (sourse), (mes));
#define ERROR_LOG(sourse, mes) LOGGER::GetLogger().Log(LOGGER::Logger::Level::ERROR_LEVEL, (sourse), (mes));
#define FATAL_LOG(sourse, mes) LOGGER::GetLogger().Log(LOGGER::Logger::Level::FATAL_ERROR, (sourse), (mes));

namespace LOGGER
{
    static struct FileLog
	{
	public:
		FileLog() 
		{
			
		}

		~FileLog();

		void Write(const std::string& data);
		std::string path;
		std::ofstream logFile;
	};

	static struct Logger
	{
	public:
		enum Level
		{
			SUCESS_LEVEL = 0,
			INFO_LEVEL,
			WARN_LEVEL,
			ERROR_LEVEL,
			FATAL_ERROR
		};

		static inline std::array<std::string, FATAL_ERROR + 1u> messageLevel =
		{
			"SUCESS:", "INFO:", "WARNING:", "ERROR:", "FATAL ERROR:"
		};

		Logger(const std::string& filePath)
		{
			fileLogger.path = filePath;
		}

		void Log(Level level, const std::string sourse, const std::string message);

		static inline std::string path = "log.txt.log";
		static inline FileLog fileLogger;
	private:
		static inline std::array<int, FATAL_ERROR + 1u> colors =
		{
			FOREGROUND_GREEN | FOREGROUND_INTENSITY,
			FOREGROUND_BLUE | FOREGROUND_INTENSITY,
			FOREGROUND_RED | FOREGROUND_GREEN,
			FOREGROUND_RED | FOREGROUND_INTENSITY,
			FOREGROUND_RED | FOREGROUND_INTENSITY
		};
	
	};

	static inline Logger GetLogger()
	{
		LOGGER::Logger logger(LOGGER::Logger::path);
		return logger;
	}
}

