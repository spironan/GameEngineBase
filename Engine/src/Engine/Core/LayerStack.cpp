/************************************************************************************//*!
\file           LayerStack.cpp
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 24, 2021
\brief          Layerstack holds a vector of layers and provides the interface to 
                interact and control the individual layers. 
                Read layers.h for more explaination on what a layer is.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "LayerStack.h"

namespace engine
{
    LayerStack::~LayerStack()
    {
        for (Layer* layer : m_layers)
        {
            layer->OnDetach();
            delete layer;
        }
    }

    void LayerStack::PushLayer(Layer* layer)
    {
        m_layers.emplace(m_layers.begin() + m_layerInsertIndex++, layer);
    }

    void LayerStack::PushOverlay(Layer* overlay)
    {
        m_layers.emplace_back(overlay);
    }

    void LayerStack::PopLayer(Layer* layer)
    {
        auto it = std::find(m_layers.begin(), m_layers.begin() + m_layerInsertIndex, layer);
        if (it != m_layers.begin() + m_layerInsertIndex)
        {
            layer->OnDetach();
            m_layers.erase(it);
            --m_layerInsertIndex;
        }
    }

    void LayerStack::PopOverlay(Layer* overlay)
    {
        auto it = std::find(m_layers.begin() + m_layerInsertIndex, m_layers.end(), overlay);
        if (it != m_layers.end())
        {
            overlay->OnDetach();
            m_layers.erase(it);
        }
    }

}