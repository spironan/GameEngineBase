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
    std::vector<SceneBaseLayer*> debugLayers;
    SceneBaseLayer* currentLayer = nullptr;

public:
    DebugLayer(engine::Application& app)
        : Layer{ "Debug Layer" }
        , application{ app }
    {
        LOG_TRACE("SUCCESSFULLY LOADED DEBUG LAYER");
    }

    virtual ~DebugLayer() final
    {
        for (auto& debugLayer : debugLayers)
            if (debugLayer != currentLayer)
                delete debugLayer;
    }

    void OnAttach() final override
    {
        // Purposeful breaking if uncommented out and choosing to run it.
        //debugLayers.emplace_back(new GameObjectTestLayer());

        debugLayers.emplace_back(new TransformTestLayer());
        debugLayers.emplace_back(new PhysicsTestLayer());
        debugLayers.emplace_back(new SceneTestLayer("D:/GameEngine/bin/Debug-OpenGL-windows-x86_64/Sandbox/New folder/Scene.scene"));
        debugLayers.emplace_back(new RenderingTestLayer());
        debugLayers.emplace_back(new ScriptingTestLayer());
        
        //currentLayer = debugLayers.front();
        //application.PushOverlay(currentLayer);
    }

    void OnUpdate(engine::Timestep dt) final override
    {
        if(currentLayer)
            engine::SceneManager::SetActiveScene(currentLayer->GetID());
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
                application.PushOverlay(debugLayer);
            }
        ImGui::End();
    }
};
