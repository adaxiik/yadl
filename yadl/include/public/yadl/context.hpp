#pragma once
#include <memory>
#include <cstdint>
#include <stack>
#include <functional>
#include <utility>

#include "pixel.hpp"
#include "canvas.hpp"
#include "guard.hpp"
#include "font.hpp"
#include "depth_buffer.hpp"

namespace yadl
{
    class Context
    {
    private:
        using ShapeActionFn = std::function<void(Pixel &)>;
    public:
        struct State
        {
            Pixel color{0, 0, 0, 255};
            int32_t thickness{1};
            int32_t positionX{0};
            int32_t positionY{0};
            ShapeActionFn action;
            Canvas canvas;
            DepthBuffer depthBuffer;
            Font font;
            float fontScale{1.0f};
        };
    private:
        State m_currentState;
        std::stack<State> m_stateStack;

        ShapeActionFn  m_blend = [&](Pixel &pixel)    { pixel = pixel.Blend(m_currentState.color); };
        ShapeActionFn  m_add = [&](Pixel &pixel)      { pixel += m_currentState.color; };
        ShapeActionFn  m_subtract = [&](Pixel &pixel) { pixel -= m_currentState.color; };
        ShapeActionFn  m_set = [&](Pixel &pixel)      { pixel = m_currentState.color; };

        Context() = delete;
    public:
        // This crashes the program for some reason:
        // Context(Canvas canvas) : m_currentState{State{Pixel{0, 0, 0, 255}, 1, 0, 0, m_set, canvas}} {}

        /**
         * @brief Construct a new Context 
         * @note This will create a context with a default state.
         * @warning If you want to use text rendering, you must set the font before using it!
         * 
         * @sa SetCanvas
         * 
         * @param canvas 
         */
        Context(Canvas canvas) 
        {
            m_currentState.color = Pixel{0, 0, 0, 255};
            m_currentState.thickness = 1;
            m_currentState.positionX = 0;
            m_currentState.positionY = 0;
            m_currentState.action = m_set;
            m_currentState.canvas = canvas;
            m_currentState.depthBuffer = DepthBuffer(canvas.GetWidth(), canvas.GetHeight());
            m_currentState.fontScale = 1.0f;
        }

        Context(Canvas canvas, DepthBuffer depthBuffer) : Context(canvas)
        {
            m_currentState.depthBuffer = depthBuffer;
        }

        Context(const Context &other) = default;
        Context &operator=(const Context &other) = default;
        Context(Context &&other) noexcept = default;
        Context &operator=(Context &&other) noexcept = default;
        ~Context() = default;
        
        /**
         * @brief Save the current state of the context
         * @sa RestoreState
         * 
         * @return Context& 
         */
        Context &SaveState()
        {
            m_stateStack.push(m_currentState);
            return *this;
        }

        /**
         * @brief Restore the state of the context
         * @sa SaveState
         * 
         * @return Context& 
         */
        Context &RestoreState()
        {
            if (!m_stateStack.empty())
            {
                m_currentState = m_stateStack.top();
                m_stateStack.pop();
            }
            return *this;
        }

        /**
         * @brief Create a Guard object
         * @details This will save the current state of the context and restore it when the guard goes out of scope.
         * @sa SaveState
         * @sa RestoreState
         * @sa Guard
         * 
         * @return Guard 
         */
        Guard CreateGuard()
        {
            m_stateStack.push(m_currentState);
            return Guard([this]() { RestoreState();});
        }

        /**
         * @brief Set the Color
         * 
         * @param color 
         * @return Context& 
         */
        Context &SetColor(Pixel color)
        {
            m_currentState.color = color;
            return *this;
        }

        /**
         * @brief Set the Thickness 
         * 
         * @param thickness 
         * @return Context& 
         */
        Context &SetThickness(int32_t thickness)
        {
            m_currentState.thickness = thickness;
            return *this;
        }

        /**
         * @brief Set the Position 
         * 
         * @param x 
         * @param y 
         * @return Context& 
         */
        Context &SetPosition(int32_t x, int32_t y)
        {
            m_currentState.positionX = x;
            m_currentState.positionY = y;
            return *this;
        }

        /**
         * @brief Set the Mode Blend
         * @details This will set the mode to blend.
         * 
         * @sa SetModeAdd
         * @sa SetModeSub
         * @sa SetModeSet
         * 
         * @return Context& 
         */
        Context &SetModeBlend()
        {
            m_currentState.action = m_blend;
            return *this;
        }

        /**
         * @brief Set the Mode Add
         * @details This will set the mode to add.
         * 
         * @sa SetModeBlend
         * @sa SetModeSub
         * @sa SetModeSet
         * 
         * @return Context& 
         */
        Context &SetModeAdd()
        {
            m_currentState.action = m_add;
            return *this;
        }

        /**
         * @brief Set the Mode Sub
         * @details This will set the mode to subtract.
         * 
         * @sa SetModeBlend
         * @sa SetModeAdd
         * @sa SetModeSet
         * 
         * @return Context& 
         */
        Context &SetModeSub()
        {
            m_currentState.action = m_subtract;
            return *this;
        }

        /**
         * @brief Set the Mode Set
         * @details This will set the mode to set.
         * 
         * @sa SetModeBlend
         * @sa SetModeAdd
         * @sa SetModeSub
         * 
         * @return Context& 
         */
        Context &SetModeSet()
        {
            m_currentState.action = m_set;
            return *this;
        }

        /**
         * @brief Set the Canvas
         * 
         * @param canvas 
         * @return Context& 
         */
        Context &SetCanvas(Canvas& canvas)
        {
            m_currentState.canvas = canvas;
            return *this;
        }

        /**
         * @brief Set the Font
         * 
         * @param font 
         * @return Context& 
         */
        Context &SetFont(Font& font)
        {
            m_currentState.font = font;
            return *this;
        }

        /**
         * @brief Set the Font Scale
         * 
         * @param scale 
         * @return Context& 
         */
        Context &SetFontScale(float scale)
        {
            m_currentState.fontScale = scale;
            return *this;
        }

        /**
         * @brief Get the current state of the context
         * 
         * @return Pixel 
         */
        State &GetState()
        {
            return m_currentState;
        }

        /**
         * @brief Get the Current Text Height
         * 
         * @return int32_t 
         */
        inline int32_t GetCurrentTextHeight() const
        {
            return 42 * m_currentState.fontScale; // magic constant :) 
            // TODO: replace 42, with less magic constant 
            // font is loaded with 42 as height
            // @see font_internal.cpp
        }
        
    };
}