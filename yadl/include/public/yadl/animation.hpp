#pragma once
#include <vector>
#include "canvas.hpp"

namespace yadl
{
    /**
     * @brief Animation class
     * @details This class is used to store a series of frames that can be used to create an animation.
     * 
     */
    class Animation
    {
    private:
        std::vector<Canvas> m_frames;
        int32_t m_width, m_height;
    public:
        /**
         * @brief Construct a new Animation object
         * @details All added frames will be resized to the specified width and height.
         * @param width 
         * @param height 
         */
        Animation(int32_t width, int32_t height) : m_width(width), m_height(height) {}
        ~Animation() = default;

        /**
         * @brief Add a frame to the animation
         * @details This will create own deep copy of the frame.
         * 
         * @param frame 
         * @return Animation& 
         */
        Animation& AddFrame(const Canvas& frame);

        /**
         * @brief Remove a frame from the animation
         * 
         * @note if the index is out of range, this function will do nothing.
         * 
         * @param index 
         * @return Animation& 
         */
        Animation& RemoveFrame(size_t index);

        /**
         * @brief Get the Frame 
         * 
         * @note if the index is out of range, it will cause undefined behavior.
         * 
         * @param index 
         * @return const Canvas& 
         */
        const Canvas& GetFrame(size_t index) const;

        /**
         * @brief Get the Frame Count 
         * 
         * @return size_t 
         */
        size_t GetFrameCount() const;

        /**
         * @brief Get the Frame
         * @note It will internally call GetFrame(int index) const, so if the index is out of range, it will cause undefined behavior.
         * 
         * @param index 
         * @return const Canvas& 
         */
        const Canvas& operator[](size_t index) const { return GetFrame(index); }

        /**
         * @brief Get the Width 
         * 
         * @return int32_t 
         */
        int32_t GetWidth() const { return m_width; }

        /**
         * @brief Get the Height 
         * 
         * @return int32_t 
         */
        int32_t GetHeight() const { return m_height; }

    };

}
