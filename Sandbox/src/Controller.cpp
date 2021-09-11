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
    std::reinterpret_pointer_cast<SimulationSceneLayer>(m_simulateScene)->PauseSimulation();
}

void Controller::UnPause()
{
    std::reinterpret_pointer_cast<SimulationSceneLayer>(m_simulateScene)->UnpauseSimulation();
}

void Controller::Next()
{
    std::reinterpret_pointer_cast<SimulationSceneLayer>(m_simulateScene)->ProcessFrame(1);
}

void Controller::Stop()
{
    if (m_currentScene == m_gameScene) return;

    m_application.PopOverlay(m_currentScene);
    m_currentScene = m_gameScene;
    m_application.PushOverlay(m_currentScene);
}
