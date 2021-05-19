#pragma once

namespace engine
{
    class Timestep
    {
    public:
        Timestep(double time = 0.f)
            : m_time(time)
        {
        }

        operator double() const { return m_time; }

        double GetSeconds() const { return m_time * 0.001; }
        double GetMilliSeconds() const { return m_time; }

    private:
        // time in milliseconds.
        double m_time;
    };
}
