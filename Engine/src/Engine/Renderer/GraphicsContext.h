#pragma once

namespace engine
{
    class GraphicsContext
    {
    public :
        virtual ~GraphicsContext() = default;

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        virtual void InitImGui() = 0;
    };
}