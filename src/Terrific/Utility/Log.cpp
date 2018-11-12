#include "Log.h"

#include <iostream>

namespace Terrific {
	namespace Utility {
		std::shared_ptr<spdlog::logger> Log::_coreLogger;
		std::shared_ptr<spdlog::logger> Log::_clientLogger;

		int Log::init() {
      //			spdlog::set_pattern("%^[%L] %^[%T] %n: %v%$");
      //spdlog::set_pattern("%^%L: %^[%T] %n: %v%$");

      // gnu
      spdlog::set_pattern("%^%v%$");

      //      watcom
      //..\src\ctrl\lister.c(109): Error! E1009: Expecting ';' but found '{'
      //..\src\ctrl\lister.c(120): Warning! W201: Unreachable code
      //spdlog::set_pattern("%^%v:[%l] %$");

      try {
        _coreLogger = spdlog::stdout_color_mt("TERRIFIC");
      } catch (const spdlog::spdlog_ex &ex) {
        std::cout << "Terrific::Utility::Log::init() TERRIFIC logger failed to initialize: " << ex.what() << "\n"; 
        return EXIT_FAILURE;
      }

      try {
        //TODO: If verbose:
                _clientLogger = spdlog::stdout_color_mt("APP");
                //        _clientLogger = spdlog::basic_logger_mt("APP", "logs/app.log");
      } catch (const spdlog::spdlog_ex &ex) {
        std::cout << "Terrific::Utility::Log::init() APP logger failed to initialize: " << ex.what() << "\n"; 
      }

      _clientLogger->set_pattern("%^%L: %^[%T] %n:\t%v%$");

      _coreLogger->set_level(spdlog::level::trace);
			_clientLogger->set_level(spdlog::level::trace);

			TERRIFIC_CORE_INFO("Logger initialized");
			TERRIFIC_INFO("Logger initialized");
#if 0
      TERRIFIC_CORE_WARNING("Logger Warning");

      TERRIFIC_CORE_TRACE("Trace");
			TERRIFIC_CORE_ERROR("Logger Error");
      TERRIFIC_CORE_CRITICAL("Logger Critical");
#endif

      return EXIT_SUCCESS;
		}
	}
}
