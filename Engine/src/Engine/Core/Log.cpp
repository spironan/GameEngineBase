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

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Engine 
{
    //static defines
    std::shared_ptr<spdlog::logger> Log::s_Corelogger;
    std::shared_ptr<spdlog::logger> Log::s_Clientlogger;

    void Log::Init()
    {
        // %^	== Color
        // [%T] == Timestamp
        // %n	== Name of Logger
        // %v%$ == Log Message
        spdlog::set_pattern("%^ [%T] %n: %v%$");
        // Creates a multi-threaded, Colored std out console
        s_Corelogger = spdlog::stdout_color_mt("ENGINE");
        // Set logging level to trace : lowest level therefore traces everything
        s_Corelogger->set_level(spdlog::level::trace);

        // Creates a multi-threaded, Colored std out console
        s_Clientlogger = spdlog::stdout_color_mt("CLIENT");
        // Set logging level to trace : lowest level therefore traces everything
        s_Clientlogger->set_level(spdlog::level::trace);
    }


}