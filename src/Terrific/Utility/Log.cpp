#include "Log.h"

#include <iostream>

namespace Terrific {
	namespace Utility {
		std::shared_ptr<spdlog::logger> Log::_coreLogger;
		std::shared_ptr<spdlog::logger> Log::_clientLogger;

		int Log::init() {
			spdlog::set_pattern("%^[%T] %n: %v%$");

			try {
				_coreLogger = spdlog::stdout_color_mt("TERRIFIC");
			} catch (const spdlog::spdlog_ex &ex) {
				std::cout << "Terrific::Utility::Log::init() TERRIFIC logger failed to initialize: " << ex.what() << "\n"; 
				return EXIT_FAILURE;
			} 

			try {
				_clientLogger = spdlog::stdout_color_mt("APP");
			} catch (const spdlog::spdlog_ex &ex) {
				std::cout << "Terrific::Utility::Log::init() APP logger failed to initialize: " << ex.what() << "\n"; 
			} 

			_coreLogger->set_level(spdlog::level::trace);
			_clientLogger->set_level(spdlog::level::trace);

			//_coreLogger->info("Logger initialized");
			//_clientLogger->info("Logger initialized");
			TERRIFIC_CORE_INFO("Logger initialized");
			TERRIFIC_INFO("Logger initialized");

			return EXIT_SUCCESS;
		}
	}
}
