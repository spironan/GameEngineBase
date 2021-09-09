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
#if !defined(SPDLOG_ACTIVE_LEVEL) 
#define SPDLOG_ACTIVE_LEVEL 0 
#endif 
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

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
         @brief     Releasing of logging library. Performed in main
        *//*********************************************************************************/
        static void Shutdown();

        /********************************************************************************//*!
         @brief     Retrieve the core logger used to store logging information of
                    Game Engine
        *//*********************************************************************************/
        static std::shared_ptr<spdlog::logger>& GetCoreLogger()      { return s_coreLogger; }

        /********************************************************************************//*!
         @brief     Retrieve the client logger used to store logging information of
                    Client Application
        *//*********************************************************************************/
        static std::shared_ptr<spdlog::logger>& GetClientLogger()    { return s_clientLogger; }


    protected:
        // do not create an instance of this class
        Log()   = delete;
        ~Log()  = delete;

    private:
        static std::shared_ptr<spdlog::logger> s_coreLogger;
        static std::shared_ptr<spdlog::logger> s_clientLogger;
    };

    /*-----------------------------------------------------------------------------*/
    /* Debug Functions for C#                                                      */
    /*-----------------------------------------------------------------------------*/
    extern "C"
    {
        /*********************************************************************************//*!
        \brief      This function's main purpose is to allow C# scripts to log traces to the engine.

        \param      msg
                the string to output to the engine console
        *//**********************************************************************************/
        __declspec(dllexport) void Log(const char* msg);

        /*********************************************************************************//*!
        \brief      This function's main purpose is to allow C# scripts to log info to the engine.

        \param      msg
                the string to output to the engine console
        *//**********************************************************************************/
        __declspec(dllexport) void LogInfo(const char* msg);

        /*********************************************************************************//*!
        \brief      This function's main purpose is to allow C# scripts to log warnings to the engine.

        \param      msg
                the string to output to the engine console
        *//**********************************************************************************/
        __declspec(dllexport) void LogWarning(const char* msg);

        /*********************************************************************************//*!
        \brief      This function's main purpose is to allow C# scripts to log errors to the engine.

        \param      msg
                the string to output to the engine console
        *//**********************************************************************************/
        __declspec(dllexport) void LogError(const char* msg);

        /*********************************************************************************//*!
        \brief      This function's main purpose is to allow C# scripts to log critical errors to the engine.

        \param      msg
                the string to output to the engine console
        *//**********************************************************************************/
        __declspec(dllexport) void LogCritical(const char* msg);
    }
}

/****************************************************************************//*!
 @brief     Disables all logging information if not in debug mode
*//*****************************************************************************/
#ifdef ENGINE_DEBUG
    // Engine Log macros
    #define LOG_ENGINE_TRACE(...)       SPDLOG_LOGGER_TRACE(::engine::Log::GetCoreLogger(),__VA_ARGS__)
    #define LOG_ENGINE_INFO(...)        SPDLOG_LOGGER_INFO(::engine::Log::GetCoreLogger(),__VA_ARGS__)
    #define LOG_ENGINE_WARN(...)        SPDLOG_LOGGER_WARN(::engine::Log::GetCoreLogger(),__VA_ARGS__)
    #define LOG_ENGINE_ERROR(...)       SPDLOG_LOGGER_ERROR(::engine::Log::GetCoreLogger(),__VA_ARGS__)
    #define LOG_ENGINE_CRITICAL(...)    SPDLOG_LOGGER_CRITICAL(::engine::Log::GetCoreLogger(),__VA_ARGS__)

    // Client Log macros
    #define LOG_TRACE(...)              SPDLOG_LOGGER_TRACE(::engine::Log::GetClientLogger(),__VA_ARGS__)
    #define LOG_INFO(...)               SPDLOG_LOGGER_INFO(::engine::Log::GetClientLogger(),__VA_ARGS__)
    #define LOG_WARN(...)               SPDLOG_LOGGER_WARN(::engine::Log::GetClientLogger(),__VA_ARGS__)
    #define LOG_ERROR(...)              SPDLOG_LOGGER_ERROR(::engine::Log::GetClientLogger(),__VA_ARGS__)
    #define LOG_CRITICAL(...)           SPDLOG_LOGGER_CRITICAL(::engine::Log::GetClientLogger(),__VA_ARGS__)

    //#define LOG_OUTPUT_STREAM(oss)      ::engine::Log::SetOutputStream(oss);

#elif ENGINE_RELEASE
    // Engine Log macros
    #define LOG_ENGINE_TRACE(...)       
    #define LOG_ENGINE_INFO(...)        
    #define LOG_ENGINE_WARN(...)        SPDLOG_LOGGER_WARN(::engine::Log::GetCoreLogger(),__VA_ARGS__)
    #define LOG_ENGINE_ERROR(...)       SPDLOG_LOGGER_ERROR(::engine::Log::GetCoreLogger(),__VA_ARGS__)
    #define LOG_ENGINE_CRITICAL(...)    SPDLOG_LOGGER_CRITICAL(::engine::Log::GetCoreLogger(),__VA_ARGS__)

    // Client Log macros
    #define LOG_TRACE(...)              SPDLOG_LOGGER_TRACE(::engine::Log::GetClientLogger(),__VA_ARGS__)
    #define LOG_INFO(...)               SPDLOG_LOGGER_INFO(::engine::Log::GetClientLogger(),__VA_ARGS__)
    #define LOG_WARN(...)               SPDLOG_LOGGER_WARN(::engine::Log::GetClientLogger(),__VA_ARGS__)
    #define LOG_ERROR(...)              SPDLOG_LOGGER_ERROR(::engine::Log::GetClientLogger(),__VA_ARGS__)
    #define LOG_CRITICAL(...)           SPDLOG_LOGGER_CRITICAL(::engine::Log::GetClientLogger(),__VA_ARGS__)

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