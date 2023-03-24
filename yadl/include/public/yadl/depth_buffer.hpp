#pragma once

#include "canvas.hpp"

namespace yadl
{
    class DepthBuffer
    {
    private:
        Canvas m_buffer;


    public:
        DepthBuffer(Canvas& canvas)
            : m_buffer(canvas)
        {
        }

        DepthBuffer(int32_t width, int32_t height)
            : m_buffer(width, height)
        {
        }
        DepthBuffer() = default;
        ~DepthBuffer() = default;

        Canvas& GetBuffer() { return m_buffer; }
        DepthBuffer& Clear();

        float GetDepth(int x, int y) const;
        void SetDepth(int x, int y, float depth);
    };
}
