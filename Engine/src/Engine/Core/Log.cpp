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
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/sinks/dup_filter_sink.h>
#include "LogCallbackSink.h"
#pragma warning(pop)

namespace engine
{
    //static defines
    std::shared_ptr<spdlog::logger> Log::s_coreLogger;
    std::shared_ptr<spdlog::logger> Log::s_clientLogger;
    std::ostringstream Log::oss;
    void Log::Init()
    {
        // %^	== Color
        // [%T] == Timestamp
        // %n	== Name of Logger
        // %v%$ == Log Message
        spdlog::set_pattern("%^ [%T] %n: %v%$");
        
        // Creates a multi-threaded, Colored std out console
        s_coreLogger = spdlog::stdout_color_mt("ENGINE");
        // Set logging level to trace : lowest level therefore traces everything
        s_coreLogger->set_level(spdlog::level::trace);

        // Creates a multi-threaded, Colored std out console
        s_clientLogger = spdlog::stdout_color_mt("CLIENT");
        // Set logging level to trace : lowest level therefore traces everything
        s_clientLogger->set_level(spdlog::level::trace);

        /*auto dup_filter_sink = std::make_shared<spdlog::sinks::dup_filter_sink_mt>(std::chrono::seconds(1));
        s_coreLogger->sinks().push_back(dup_filter_sink);
        s_clientLogger->sinks().push_back(dup_filter_sink);*/

        // this can be broken down so that it takes in user defined oss.
        // create the ostream sink
        auto ostream_sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(oss);
		auto callback_sink = std::make_shared<CallbackSink_mt>();
        // link them to both loggers
        s_coreLogger->sinks().push_back(ostream_sink);
        s_clientLogger->sinks().push_back(ostream_sink);
		s_coreLogger->sinks().push_back(callback_sink);
		s_clientLogger->sinks().push_back(callback_sink);
        //spdlog::flush_every(std::chrono::seconds(3));
    }

    std::ostringstream& Log::GetOstreamOutput()
    {
        return oss;
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