#include "sdl_core/sdl_core.hpp"
#include <imgui.h>
#include <imgui_stdlib.h>
#include <cmath>

#include <yadl/pixel.hpp>
#include <yadl/shape.hpp>
#include <yadl/io.hpp>
#include <string>

using namespace yadl;
class PaintExample : public SDLCore
{
private:
    float m_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    int m_mouseX {0};
    int m_mouseY {0};
    bool m_mouseDown {false};
    int m_brushSize {10};
    std::string m_saveAs {"painting.png"};


    virtual void OnStart()
    {
        m_canvas.Clear(Color::Dark);
    }

    virtual void OnRender(double deltaTime)
    {
        (void)deltaTime;

        int canvasX = (m_mouseX / static_cast<float>(m_width)) * m_canvasWidth;
        int canvasY = (m_mouseY / static_cast<float>(m_height)) * m_canvasHeight;


        Pixel color{static_cast<uint8_t>(m_color[0] * 255),
                    static_cast<uint8_t>(m_color[1] * 255),
                    static_cast<uint8_t>(m_color[2] * 255),
                    static_cast<uint8_t>(m_color[3] * 255)};

        m_ctx.SetColor(color).SetPosition(canvasX, canvasY);

        if (m_mouseDown)
            Shape::DrawFilledCircleAA(m_ctx, m_brushSize);

        
    }

    virtual void OnImGuiRender(double deltaTime)
    {
        ImGui::Begin("Paint Example");
        ImGui::Text("FPS: %.2f", 1.0 / deltaTime);   
        ImGui::Text("Mouse Position: (%d, %d)", m_mouseX, m_mouseY);
        ImGui::ColorEdit4("Color", m_color);
        ImGui::SliderInt("Brush Size", &m_brushSize, 1, 50);
        if(ImGui::Button("Clear"))
            m_canvas.Clear(Color::Dark);

        ImGui::Separator();
        ImGui::InputText("Save As", &m_saveAs);
        if (ImGui::Button("Save"))
            io::SaveAsPNG(m_saveAs, m_canvas);


        ImGui::End();
    }

    virtual void OnUpdate(double deltaTime)
    {
        (void)deltaTime;
        m_mouseDown = SDL_GetMouseState(&m_mouseX, &m_mouseY) & SDL_BUTTON(SDL_BUTTON_RIGHT);
    }
public:
    PaintExample(int32_t width, int32_t height) : SDLCore(width, height) {}
    virtual ~PaintExample() = default;
};


int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    PaintExample example(1280, 720);
    example.Start();
    return 0;
}


