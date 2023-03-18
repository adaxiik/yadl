#pragma once
#include <vector>
#include "canvas.hpp"

namespace yadl
{
    class Animation
    {
    private:
        std::vector<Canvas> m_frames;
        int32_t m_width, m_height;
    public:
        Animation(int32_t width, int32_t height) : m_width(width), m_height(height) {}
        ~Animation() = default;

        Animation& AddFrame(const Canvas& frame);
        Animation& RemoveFrame(int index);
        const Canvas& GetFrame(int index) const;
        size_t GetFrameCount() const;
        const Canvas& operator[](int index) const { return GetFrame(index); }
        int32_t GetWidth() const { return m_width; }
        int32_t GetHeight() const { return m_height; }

    };

}
