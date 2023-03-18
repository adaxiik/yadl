#include "animation.hpp"

namespace yadl
{
    Animation& Animation::AddFrame(const Canvas& frame)
    {
        m_frames.push_back(frame.Resize(m_width, m_height));
        return *this;
    }

    const Canvas& Animation::GetFrame(int index) const
    {
        return m_frames[index];
    }

    size_t Animation::GetFrameCount() const
    {
        return m_frames.size();
    }

    Animation& Animation::RemoveFrame(int index)
    {
        m_frames.erase(m_frames.begin() + index);
        return *this;
    }

}