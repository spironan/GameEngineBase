#pragma once

namespace engine
{
    class GraphicsContext
    {
    public :
        virtual ~GraphicsContext() = default;

        virtual void Init() = 0;
        virtual void OnUpdateBegin() = 0;
        virtual void SwapBuffers() = 0;

        virtual void InitImGui() = 0;
        virtual void OnImGuiBegin() = 0;
        virtual void OnImGuiEnd() = 0;
        virtual void OnImGuiShutdown() = 0;

        virtual void SetWindowResized() = 0;
        virtual bool SetVSync(bool enable) = 0;
    };
}