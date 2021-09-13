#pragma once

#include <engine.h>

#include "UtilityLayers/SceneBaseLayer.h"

class Controller
{
private:
    //Temporary
    // filepath to scene
    std::filesystem::path m_filepath; 

public:
    Controller(engine::Application& app);
    ~Controller() = default;

    void Simulate();
    void Stop();

    void Pause();
    void UnPause();

    void Next();

    

    //app access
    engine::Application& m_application;

    std::shared_ptr<SceneBaseLayer> m_currentScene = nullptr;
    std::shared_ptr<SceneBaseLayer> m_gameScene = nullptr;
    std::shared_ptr<SceneBaseLayer> m_simulateScene = nullptr;
};
