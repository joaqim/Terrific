#pragma once

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>


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

#define TERRIFIC_CORE_TRACE(...) ::Terrific::Utility::Log::getCoreLogger()->trace(__VA_ARGS__)
#define TERRIFIC_CORE_INFO(...)  ::Terrific::Utility::Log::getCoreLogger()->info(__VA_ARGS__)
#define TERRIFIC_CORE_WARN(...)  ::Terrific::Utility::Log::getCoreLogger()->warn(__VA_ARGS__)
#define TERRIFIC_CORE_ERROR(...) ::Terrific::Utility::Log::getCoreLogger()->error(__VA_ARGS__)
#define TERRIFIC_CORE_FATAL(...) ::Terrific::Utility::Log::getCoreLogger()->fatal(__VA_ARGS__)

#define TERRIFIC_TRACE(...) ::Terrific::Utility::Log::getClientLogger()->trace(__VA_ARGS__)
#define TERRIFIC_INFO(...)  ::Terrific::Utility::Log::getClientLogger()->info(__VA_ARGS__)
#define TERRIFIC_WARN(...)  ::Terrific::Utility::Log::getClientLogger()->warn(__VA_ARGS__)
#define TERRIFIC_ERROR(...) ::Terrific::Utility::Log::getClientLogger()->error(__VA_ARGS__)
#define TERRIFIC_FATAL(...) ::Terrific::Utility::Log::getClientLogger()->fatal(__VA_ARGS__)
