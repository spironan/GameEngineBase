/************************************************************************************//*!
\file           Layer.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 24, 2021
\brief          Describes a single layer in the application. Think of each layer like a
                general scene where u inherit and write behaviour. 
                The idea behind having layers
                is to allow control of rendering and updating as one can selectively
                turn on and off a layer.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Core/Events/Event.h"

namespace engine
{
    /********************************************************************************//*!
     @brief     Describes a single layer in the application. Think of each layer like a
                general scene where u inherit and write behaviour. 
                The idea behind having layers
                is to allow control of rendering and updating as one can selectively
                turn on and off a layer.
    *//*********************************************************************************/
    class Layer
    {
    public:
        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        /*-----------------------------------------------------------------------------*/
        /* Getters                                                                     */
        /*-----------------------------------------------------------------------------*/
        const std::string& GetName() const { return m_debugName; }

        /*-----------------------------------------------------------------------------*/
        /* Functions                                                                   */
        /*-----------------------------------------------------------------------------*/
        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep dt) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

    private:
        std::string m_debugName;
    };
}