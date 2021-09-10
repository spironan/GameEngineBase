#pragma once

#include <engine.h>

#include "UtilityLayers/SceneBaseLayer.h"

class Controller
{
public:
    Controller(engine::Application& app);
    ~Controller() = default;

    void Simulate();
    void Pause();
    void Stop();

    //app access
    engine::Application& m_application;

    std::shared_ptr<SceneBaseLayer> m_currentScene = nullptr;
    std::shared_ptr<SceneBaseLayer> m_simulateScene = nullptr;
    std::shared_ptr<SceneBaseLayer> m_gameScene = nullptr;
};
