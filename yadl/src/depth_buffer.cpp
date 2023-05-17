#include "depth_buffer.hpp"
#include <cstring>
#include "pixel.hpp"

namespace yadl
{
    DepthBuffer& DepthBuffer::Clear()
    {
        float clearValue = std::numeric_limits<float>::min();
        uint32_t clearValueInt;
        std::memcpy(&clearValueInt, &clearValue, sizeof(float));
        Pixel clearPixel(clearValueInt);
        m_buffer.Clear(clearPixel);
        return *this;
    }

    float DepthBuffer::GetDepth(int x, int y) const
    {
        uint32_t pixel = m_buffer.GetPixel(x, y).ToInt32();
        float depth;
        std::memcpy(&depth, &pixel, sizeof(float));
        return depth;
    }

    void DepthBuffer::SetDepth(int x, int y, float depth)
    {
        uint32_t depthInt;
        std::memcpy(&depthInt, &depth, sizeof(float));
        Pixel depthPixel(depthInt);
        m_buffer.SetPixel(x, y, depthPixel);
    }
}