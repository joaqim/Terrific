#pragma once

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h> // support for stdout colored logging
#include "spdlog/sinks/basic_file_sink.h"    // support for basic file logging

namespace Terrific {
	namespace Utility {
		class Log {
			public:
				static int init();

				inline static std::shared_ptr<spdlog::logger> &getCoreLogger() {return _coreLogger; }
				inline static std::shared_ptr<spdlog::logger> &getClientLogger() {return _clientLogger; }
			private:
				static std::shared_ptr<spdlog::logger> _coreLogger;
				static std::shared_ptr<spdlog::logger> _clientLogger;
		};
	}
}

#define TERRIFIC_(logger,name,...)                                      \
    if (::Terrific::Utility::Log::get##logger##Logger()->should_log(spdlog::level::name)) { \
      ::Terrific::Utility::Log::get##logger##Logger()->name("{}:{}: in function ::{}(): {}", __FILE__,  __LINE__, __FUNCTION__,fmt::format(__VA_ARGS__)); \
    }

#define TERRIFIC_INFO_(logger,name,...)                                 \
  if (::Terrific::Utility::Log::get##logger##Logger()->should_log(spdlog::level::name)) { \
    ::Terrific::Utility::Log::get##logger##Logger()->name("{}", fmt::format(__VA_ARGS__)); \
  }


#define TERRIFIC_INFO(...) TERRIFIC_INFO_(Client,info,__VA_ARGS__)
#define TERRIFIC_WARNING(...) TERRIFIC_(Client,warn,__VA_ARGS__)
#define TERRIFIC_ERROR(...) TERRIFIC_(Client,trace,__VA_ARGS__)
#define TERRIFIC_TRACE(...) TERRIFIC_(Client,trace,__VA_ARGS__)
#define TERRIFIC_CRITICAL(...) TERRIFIC_(Client,critical,__VA_ARGS__)

#define TERRIFIC_CORE_INFO(...) TERRIFIC_INFO_(Core,info,__VA_ARGS__)
#define TERRIFIC_CORE_WARNING(...) TERRIFIC_INFO_(Core,warn,__VA_ARGS__)
#define TERRIFIC_CORE_ERROR(...) TERRIFIC_(Core,trace,__VA_ARGS__)
#define TERRIFIC_CORE_TRACE(...) TERRIFIC_(Core,trace,__VA_ARGS__)
#define TERRIFIC_CORE_CRITICAL(...) TERRIFIC_(Core,critical,__VA_ARGS__)

#if 0
#define TERRIFIC_CORE_TRACE(...) ::Terrific::Utility::Log::getCoreLogger()->trace(__VA_ARGS__)
#define TERRIFIC_CORE_INFO(...)  ::Terrific::Utility::Log::getCoreLogger()->info(__VA_ARGS__)
#define TERRIFIC_CORE_WARN(...)  ::Terrific::Utility::Log::getCoreLogger()->warn(__VA_ARGS__)
#define TERRIFIC_CORE_ERROR(...) ::Terrific::Utility::Log::getCoreLogger()->error(__VA_ARGS__)
#define TERRIFIC_CORE_CRITICAL(...) ::Terrific::Utility::Log::getCoreLogger()->critical(__VA_ARGS__)

//#define TERRIFIC_TRACE(...) ::Terrific::Utility::Log::getClientLogger()->trace(__VA_ARGS__)
#define TERRIFIC_INFO(...)  ::Terrific::Utility::Log::getClientLogger()->info(__VA_ARGS__)
#define TERRIFIC_WARN(...)  ::Terrific::Utility::Log::getClientLogger()->warn(__VA_ARGS__)
#define TERRIFIC_ERROR(...) ::Terrific::Utility::Log::getClientLogger()->error(__VA_ARGS__)
#define TERRIFIC_CRITICAL(...) ::Terrific::Utility::Log::getClientLogger()->critical(__VA_ARGS__)
#endif
