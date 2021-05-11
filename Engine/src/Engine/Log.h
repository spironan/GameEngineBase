/************************************************************************************//*!
\file           log.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 10, 2021
\brief          Core Logging library using external library spdlog.

\note           using spdlog causes some warnings : look into fixing those if possible.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Engine
{
    class Log
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()      { return s_Corelogger; }

        inline static std::shared_ptr<spdlog::logger>& GetClientLogger()    { return s_Clientlogger; }

    protected:
        // do not create an instance of this class
        Log()   = default;
        ~Log()  = default;

    private:
        static std::shared_ptr<spdlog::logger> s_Corelogger;
        static std::shared_ptr<spdlog::logger> s_Clientlogger;
    };

}


// disable all logging if not in debug
#ifdef ENGINE_DEBUG
    // Engine Log macros
    #define LOG_ENGINE_TRACE(...)       ::Engine::Log::GetCoreLogger()->trace(__VA_ARGS__)
    #define LOG_ENGINE_INFO(...)        ::Engine::Log::GetCoreLogger()->info(__VA_ARGS__)
    #define LOG_ENGINE_WARN(...)        ::Engine::Log::GetCoreLogger()->warn(__VA_ARGS__)
    #define LOG_ENGINE_ERROR(...)       ::Engine::Log::GetCoreLogger()->error(__VA_ARGS__)
    #define LOG_ENGINE_CRITICAL(...)    ::Engine::Log::GetCoreLogger()->critical(__VA_ARGS__)

    // Client Log macros
    #define LOG_TRACE(...)              ::Engine::Log::GetClientLogger()->trace(__VA_ARGS__)
    #define LOG_INFO(...)               ::Engine::Log::GetClientLogger()->info(__VA_ARGS__)
    #define LOG_WARN(...)               ::Engine::Log::GetClientLogger()->warn(__VA_ARGS__)
    #define LOG_ERROR(...)              ::Engine::Log::GetClientLogger()->error(__VA_ARGS__)
    #define LOG_CRITICAL(...)           ::Engine::Log::GetClientLogger()->critical(__VA_ARGS__)

#else
    #define LOG_ENGINE_TRACE(...)
    #define LOG_ENGINE_INFO(...) 
    #define LOG_ENGINE_WARN(...) 
    #define LOG_ENGINE_ERROR(...)
    #define LOG_ENGINE_CRITICAL(...)

    #define LOG_TRACE(...)
    #define LOG_INFO(...) 
    #define LOG_WARN(...) 
    #define LOG_ERROR(...)
    #define LOG_CRITICAL(...)
#endif