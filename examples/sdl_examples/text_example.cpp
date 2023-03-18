#include "sdl_core/sdl_core.hpp"
#include <imgui.h>
#include <imgui_stdlib.h>
#include <cmath>

#include <yadl/pixel.hpp>
#include <yadl/text.hpp>
#include <string>

using namespace yadl;
class TextExample : public SDLCore
{
private:
    std::string m_text{"I hate text\nrendering :c"};
    float m_fontScale = 1.0f;
    float m_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    virtual void OnStart()
    {
        Font font("assets/RobotoCondensed-Regular.ttf");
        m_ctx.SetFont(font);
    }

    virtual void OnRender(double deltaTime)
    {
        (void)deltaTime;

        m_canvas.Clear(Color::Dark);
        Pixel color = Pixel(static_cast<uint8_t>(m_color[0] * 255),
                            static_cast<uint8_t>(m_color[1] * 255),
                            static_cast<uint8_t>(m_color[2] * 255),
                            static_cast<uint8_t>(m_color[3] * 255));

        m_ctx.SetColor(color).SetFontScale(m_fontScale).SetPosition(20, 10);
        text::DrawText(m_ctx, m_text);
        
    }
    virtual void OnImGuiRender(double deltaTime)
    {
        ImGui::Begin("Text Example");
        ImGui::Text("FPS: %.2f", 1.0 / deltaTime);
        ImGui::SliderFloat("Font Scale", &m_fontScale, 0.1f, 2.0f);
        ImGui::ColorEdit4("Color", m_color);
        ImGui::InputTextMultiline("Text", &m_text);

        ImGui::End();
    }
public:
    TextExample() : SDLCore(){}
    virtual ~TextExample() = default;
};


int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    TextExample example;
    example.Start();
    return 0;
}


