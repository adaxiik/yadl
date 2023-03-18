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
        Context(Canvas canvas) 
        {
            m_currentState.color = Pixel{0, 0, 0, 255};
            m_currentState.thickness = 1;
            m_currentState.positionX = 0;
            m_currentState.positionY = 0;
            m_currentState.action = m_set;
            m_currentState.canvas = canvas;
            m_currentState.fontScale = 1.0f;
        }
        Context(const Context &other) = default;
        Context &operator=(const Context &other) = default;
        Context(Context &&other) noexcept = default;
        Context &operator=(Context &&other) noexcept = default;
        ~Context() = default;

        Context &SaveState()
        {
            m_stateStack.push(m_currentState);
            return *this;
        }

        Context &RestoreState()
        {
            if (!m_stateStack.empty())
            {
                m_currentState = m_stateStack.top();
                m_stateStack.pop();
            }
            return *this;
        }

        Guard CreateGuard()
        {
            m_stateStack.push(m_currentState);
            return Guard([this]() { RestoreState();});
        }

        Context &SetColor(Pixel color)
        {
            m_currentState.color = color;
            return *this;
        }

        Context &SetThickness(int32_t thickness)
        {
            m_currentState.thickness = thickness;
            return *this;
        }

        Context &SetPosition(int32_t x, int32_t y)
        {
            m_currentState.positionX = x;
            m_currentState.positionY = y;
            return *this;
        }

        Context &SetModeBlend()
        {
            m_currentState.action = m_blend;
            return *this;
        }

        Context &SetModeAdd()
        {
            m_currentState.action = m_add;
            return *this;
        }

        Context &SetModeSub()
        {
            m_currentState.action = m_subtract;
            return *this;
        }

        Context &SetModeSet()
        {
            m_currentState.action = m_set;
            return *this;
        }

        Context &SetCanvas(Canvas& canvas)
        {
            m_currentState.canvas = canvas;
            return *this;
        }

        Context &SetFont(Font& font)
        {
            m_currentState.font = font;
            return *this;
        }

        Context &SetFontScale(float scale)
        {
            m_currentState.fontScale = scale;
            return *this;
        }

        State &GetState()
        {
            return m_currentState;
        }

        inline int32_t GetCurrentTextHeight() const
        {
            return 42 * m_currentState.fontScale; // magic constant :) 
            // TODO: replace 42, with less magic constant 
            // font is loaded with 42 as height
            // @see font_internal.cpp
        }
        
    };
}