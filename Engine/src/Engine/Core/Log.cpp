/************************************************************************************//*!
\file           log.cpp
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 10, 2021
\brief          Core Logging library using external library spdlog.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "Log.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#pragma warning(pop)

#include "LogCallbackSink.h"

#include <filesystem>

namespace engine
{
    //static defines
    std::shared_ptr<spdlog::logger> Log::s_coreLogger;
    std::shared_ptr<spdlog::logger> Log::s_clientLogger;
    
    void Log::Init()
    {
        // Create logging directory if it doesn't exist.
        std::string logsDirectory = "logs";
        if (!std::filesystem::exists(logsDirectory))
            std::filesystem::create_directories(logsDirectory);

        std::vector<spdlog::sink_ptr> coreSinks =
        {
            // Creates a multi-threaded, Colored std out console
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
            std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/CoreEngine.log", true),
            std::make_shared<CallbackSink_mt>()
        };

        std::vector<spdlog::sink_ptr> clientSinks =
        {
            // Creates a multi-threaded, Colored std out console
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
            std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/Client.log", true),
            std::make_shared<CallbackSink_mt>()
        };

        // %^	== Color
        // [%T] == Timestamp
        // [%l] == Message Level
        // %n	== Name of Logger
        // %v%$ == Log Message
        coreSinks[0]->set_pattern("%^ [%T] %n: %v%$");
        coreSinks[1]->set_pattern("[%T] [%l] %n: %v");

        clientSinks[0]->set_pattern("%^ [%T] %n: %v%$");
        clientSinks[1]->set_pattern("[%T] [%l] %n: %v");

        // Create core logger
        s_coreLogger = std::make_shared<spdlog::logger>("ENGINE", coreSinks.begin(), coreSinks.end());
        // Set logging level to trace : lowest level therefore traces everything
        s_coreLogger->set_level(spdlog::level::trace);

        // Create client logger
        s_clientLogger = std::make_shared<spdlog::logger>("CLIENT", clientSinks.begin(), clientSinks.end());
        // Set logging level to trace : lowest level therefore traces everything
        s_clientLogger->set_level(spdlog::level::trace);
    }

    void Log::Shutdown()
    {
        s_coreLogger.reset();
        s_clientLogger.reset();
        spdlog::drop_all();
    }

    /*-----------------------------------------------------------------------------*/
    /* Debug Functions for C#                                                      */
    /*-----------------------------------------------------------------------------*/

    void Log(const char* msg)
    {
        LOG_TRACE(msg);
    }

    void LogInfo(const char* msg)
    {
        LOG_INFO(msg);
    }

    void LogWarning(const char* msg)
    {
        LOG_WARN(msg);
    }

    void LogError(const char* msg)
    {
        LOG_ERROR(msg);
    }

    void LogCritical(const char* msg)
    {
        LOG_CRITICAL(msg);
    }
}