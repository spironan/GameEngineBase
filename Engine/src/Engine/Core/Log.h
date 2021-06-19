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

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#include <ostream>
#include <iostream>

namespace engine
{
    /********************************************************************************//*!
     @brief     Core Logging library using external library spdlog.
    *//*********************************************************************************/
    class Log final
    {
    public:
        /********************************************************************************//*!
         @brief     Initialization of logging library. Performed in main
        *//*********************************************************************************/
        static void Init();

        /********************************************************************************//*!
         @brief     Retrieve the ostream output that is stored for logging.
        *//*********************************************************************************/
        static std::ostringstream& GetOstreamOutput();

        /********************************************************************************//*!
         @brief     Retrieve the core logger used to store logging information of
                    Game Engine
        *//*********************************************************************************/
        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()      { return s_coreLogger; }

        /********************************************************************************//*!
         @brief     Retrieve the client logger used to store logging information of
                    Client Application
        *//*********************************************************************************/
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger()    { return s_clientLogger; }

    protected:
        // do not create an instance of this class
        Log()   = delete;
        ~Log()  = delete;

    private:
        static std::shared_ptr<spdlog::logger> s_coreLogger;
        static std::shared_ptr<spdlog::logger> s_clientLogger;
        static std::ostringstream oss;
    };

}


/****************************************************************************//*!
 @brief     Disables all logging information if not in debug mode
*//*****************************************************************************/
#ifdef ENGINE_DEBUG
    // Engine Log macros
    #define LOG_ENGINE_TRACE(...)       ::engine::Log::GetCoreLogger()->trace(__VA_ARGS__)
    #define LOG_ENGINE_INFO(...)        ::engine::Log::GetCoreLogger()->info(__VA_ARGS__)
    #define LOG_ENGINE_WARN(...)        ::engine::Log::GetCoreLogger()->warn(__VA_ARGS__)
    #define LOG_ENGINE_ERROR(...)       ::engine::Log::GetCoreLogger()->error(__VA_ARGS__)
    #define LOG_ENGINE_CRITICAL(...)    ::engine::Log::GetCoreLogger()->critical(__VA_ARGS__)

    // Client Log macros
    #define LOG_TRACE(...)              ::engine::Log::GetClientLogger()->trace(__VA_ARGS__)
    #define LOG_INFO(...)               ::engine::Log::GetClientLogger()->info(__VA_ARGS__)
    #define LOG_WARN(...)               ::engine::Log::GetClientLogger()->warn(__VA_ARGS__)
    #define LOG_ERROR(...)              ::engine::Log::GetClientLogger()->error(__VA_ARGS__)
    #define LOG_CRITICAL(...)           ::engine::Log::GetClientLogger()->critical(__VA_ARGS__)

    //#define LOG_OUTPUT_STREAM(oss)      ::engine::Log::SetOutputStream(oss);
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