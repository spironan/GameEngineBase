/************************************************************************************//*!
\file           Assert.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 10, 2021
\brief          Asserts that are used in the engine.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/Core/Log.h"

/****************************************************************************//*!
 @brief     Windows only, visual studios only debug definition.
*//*****************************************************************************/
#ifdef ENGINE_DEBUG

    /****************************************************************************//*!
     @brief     Implements the functionality when to test asserts 
                when engine is in debug mode.
     
     @note      Asserts are completely stripped upon not in debug
                whereas verify still calls the original function (x).
                Use at own discretion.
    *//*****************************************************************************/
    #define ENGINE_ASSERT(x)                        { if(!(x)) {LOG_ENGINE_ERROR("Assertion Failed!"); __debugbreak();}}
    #define ENGINE_ASSERT_MSG(x, ...)               { if(!(x)) {LOG_ENGINE_ERROR("Assertion Failed : {0}", __VA_ARGS__); __debugbreak();}}
    #define ENGINE_ASSERT_CUSTOM_MSG(x, msg, ...)   { if(!(x)) {LOG_ENGINE_ERROR("Assertion Failed! " + std::string(msg), __VA_ARGS__); __debugbreak();}}

    #define ENGINE_VERIFY(x)                        { if(!(x)) {LOG_ENGINE_ERROR("Verification Failed!"); __debugbreak();} }
    #define ENGINE_VERIFY_MSG(x, ...)               { if(!(x)) {LOG_ENGINE_ERROR("Verification Failed : {0}", __VA_ARGS__); __debugbreak();} }
    #define ENGINE_VERIFY_CUSTOM_MSG(x, msg, ...)   { if(!(x)) {LOG_ENGINE_ERROR("Verification Failed! " + std::string(msg), __VA_ARGS__); __debugbreak();} }
    
#else
    #define ENGINE_ASSERT(x)                        
    #define ENGINE_ASSERT_MSG(x, ...)               
    #define ENGINE_ASSERT_CUSTOM_MSG(x, msg, ...)   
    #define ENGINE_VERIFY(x)                        x
    #define ENGINE_VERIFY_MSG(x, ...)               x
    #define ENGINE_VERIFY_CUSTOM_MSG(x, msg, ...)   x
#endif