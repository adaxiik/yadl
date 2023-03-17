#include "sdl_core/sdl_core.hpp"
#include <imgui.h>
#include <cmath>

#include <yadl/pixel.hpp>
#include <yadl/shape.hpp>


using namespace yadl;
class CircleExample : public SDLCore
{
private:
    int32_t m_radius{100};
    int32_t m_centerDistanceX{70};
    int32_t m_centerDistanceY{70};
    float m_color[4]{1.0f, 0.0f, 0.0f, 1.0f};
    bool m_filled{true};

    virtual void OnRender(double deltaTime)
    {
        static double time = 0;
        time += deltaTime;

        m_canvas.Clear(Color::Dark);
        Pixel color(static_cast<uint8_t>(m_color[0] * 255), static_cast<uint8_t>(m_color[1] * 255), static_cast<uint8_t>(m_color[2] * 255), static_cast<uint8_t>(m_color[3] * 255));
        
        m_ctx.SetColor(color);

        int32_t centerX = m_canvas.GetCenterX();
        int32_t centerY = m_canvas.GetCenterY();

        int32_t x = centerX + std::cos(time) * m_centerDistanceX;
        int32_t y = centerY + std::sin(time) * m_centerDistanceY;

        m_ctx.SetPosition(x, y);
        if (m_filled)
            Shape::DrawFilledCircle(m_ctx, m_radius);
        else
            Shape::DrawCircle(m_ctx, m_radius);

    }
    virtual void OnImGuiRender(double deltaTime)
    {
        ImGui::Begin("Circle Example");
        ImGui::Text("FPS: %.2f", 1.0 / deltaTime);
        ImGui::SliderInt("Radius", &m_radius, 1, 125);
        ImGui::SliderInt("Center Distance X", &m_centerDistanceX, 1, 100);
        ImGui::SliderInt("Center Distance Y", &m_centerDistanceY, 1, 100);
        ImGui::ColorEdit4("Color", m_color);
        ImGui::Checkbox("Filled", &m_filled);
        ImGui::End();
    }
public:
    CircleExample() : SDLCore(){}
    virtual ~CircleExample() = default;
};


int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;
    CircleExample example;
    example.Start();
    return 0;
}


