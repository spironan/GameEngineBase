#pragma once

#include "Engine/Core/Layer.h"
#include "Engine/Events/Event.h"

namespace engine
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& e) override;

        void Begin();
        void End();

        void BlockEvents(bool block) { m_blockEvents = block; }
    private:
        bool m_blockEvents;
    };
}