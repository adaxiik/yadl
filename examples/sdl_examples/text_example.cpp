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

    virtual void OnRender(double deltaTime)
    {
        (void)deltaTime;

        m_canvas.Clear(Color::Dark);
        Pixel color = Pixel(static_cast<uint8_t>(m_color[0] * 255),
                            static_cast<uint8_t>(m_color[1] * 255),
                            static_cast<uint8_t>(m_color[2] * 255),
                            static_cast<uint8_t>(m_color[3] * 255));

        Text::Get().SetCanvas(m_canvas).SetColor(color).SetScale(m_fontScale);
        Text::Get().SetPosition(20,50).DrawText(m_text);
        
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
    Font font("assets/RobotoCondensed-Regular.ttf");
    Text::Get().SetFont(font);

    TextExample example;
    example.Start();
    return 0;
}


