#pragma once

#include <Engine.h>

//TestLayers
#include "TestLayers/TransformTestLayer.h"
#include "TestLayers/GameObjectTestLayer.h"
#include "TestLayers/PhysicsTestLayer.h"
#include "TestLayers/RenderingTestLayer.h"
#include "TestLayers/SceneTestLayer.h"
#include "TestLayers/ScriptingTestLayer.h"

class DebugLayer final : public engine::Layer
{
private:
    engine::Application& application;
    std::vector<std::shared_ptr<SceneBaseLayer>> debugLayers;
    std::shared_ptr<SceneBaseLayer> currentLayer = nullptr;

public:
    DebugLayer(engine::Application& app)
        : Layer{ "Debug Layer" }
        , application{ app }
    {
        LOG_TRACE("SUCCESSFULLY LOADED DEBUG LAYER");
    }

    void OnAttach() final override
    {
        // Purposeful breaking if uncommented out and choosing to run it.
        debugLayers.emplace_back(std::make_shared<GameObjectTestLayer>());
        debugLayers.emplace_back(std::make_shared<TransformTestLayer>());
        debugLayers.emplace_back(std::make_shared<PhysicsTestLayer>());
        debugLayers.emplace_back(std::make_shared<SceneTestLayer>("D:/GameEngine/bin/Debug-OpenGL-windows-x86_64/Sandbox/New folder/Scene.scene"));
        debugLayers.emplace_back(std::make_shared<RenderingTestLayer>());
        debugLayers.emplace_back(std::make_shared<ScriptingTestLayer>());
        
        //if (debugLayers.size() > 0)
        //{
        //    // Immediately set the first layer to be loaded.
        //    currentLayer = debugLayers.front();
        //    application.PushOverlay(currentLayer);
        //}
    }

    void OnImGuiRender() final override
    {
        ImGui::Begin("Debug Layers", nullptr, ImGuiWindowFlags_NoDocking);
        for (auto& debugLayer : debugLayers)
            if (ImGui::Button(debugLayer->GetName().c_str()))
            {
                if (currentLayer != nullptr)
                application.PopOverlay(currentLayer);

                currentLayer = debugLayer;
                application.PushOverlay(currentLayer);
            }
        ImGui::End();
    }
};
