#include "Controller.h"

#include "CoreLayers/SimulationSceneLayer.h"
#include "CoreLayers/GameSceneLayer.h"

Controller::Controller(engine::Application& app)
    : m_application{ app }
    , m_gameScene { std::make_shared<GameSceneLayer>() }
    , m_simulateScene { std::make_shared<SimulationSceneLayer>() }
{
    m_currentScene = m_gameScene;
    m_application.PushOverlay(m_currentScene);
}

void Controller::Simulate()
{
    if (m_currentScene == m_simulateScene) return;

    m_application.PopOverlay(m_currentScene);
    m_currentScene = m_simulateScene;
    m_application.PushOverlay(m_currentScene);
}

void Controller::Pause()
{

}

void Controller::Stop()
{
    m_application.PopOverlay(m_currentScene);
    m_currentScene = m_gameScene;
    m_application.PushOverlay(m_currentScene);
}
