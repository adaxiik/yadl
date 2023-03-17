#pragma once
#include <cstdint>
#include <functional>
class Guard
{
private:
    std::function<void()> m_onExit;
public:
    Guard(std::function<void()> onExit) : m_onExit(onExit) {}
    Guard(const Guard& other) = delete;
    Guard& operator=(const Guard& other) = delete;
    Guard(Guard&& other) noexcept = default;
    Guard& operator=(Guard&& other) noexcept = default;

    ~Guard()
    {
        m_onExit();
    }
};

