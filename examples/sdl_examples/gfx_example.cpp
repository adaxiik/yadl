#include "sdl_core/sdl_core.hpp"
#include <imgui.h>
#include <imgui_stdlib.h>
#include <cmath>
#include <string>

#include <yadl/pixel.hpp>
#include <yadl/text.hpp>
#include <yadl/io.hpp>
#include <yadl/gfx.hpp>

using namespace yadl;
class GfxExample : public SDLCore
{
private:
    std::string m_input{"assets/cat.png"};
    bool m_rerender = true;
    Canvas m_loadedOriginal;
    const char* m_effects[4] = {"None", "Blur", "GrayScale", "Invert"};
    int m_selected = 0;
    int m_blurRadius = 1;

    virtual void OnRender(double deltaTime)
    {
        (void)deltaTime;
        
        if(!m_rerender)
            return;

        auto tmp = m_loadedOriginal.DeepCopy();
        m_ctx.SetCanvas(tmp);

        switch(m_selected)
        {
            case 0:
                break;
            case 1:
                gfx::Blur(m_ctx, m_blurRadius);
                break;
            case 2:
                gfx::Grayscale(m_ctx);
                break;
            case 3:
                gfx::Invert(m_ctx);
                break;
        }


        m_canvas.Paste(tmp, 0, 0);

        m_rerender = false;
    }

    virtual void OnImGuiRender(double deltaTime)
    {
        ImGui::Begin("Gfx Example");
        ImGui::Text("FPS: %.2f", 1.0 / deltaTime);
        ImGui::InputText("Input image", &m_input);
        if(ImGui::Button("Load image"))
        {
            m_rerender = true;
            std::optional<Canvas> img = io::Load(m_input);
            if(!img.has_value())
            {
                m_rerender = false;
                std::cout << "Failed to load image: " << m_input << std::endl;
                return;
            }
            m_loadedOriginal = img.value().Resize(m_canvas.GetWidth(), m_canvas.GetHeight());
        }
        ImGui::Separator();

        if(ImGui::ListBox("Effects", &m_selected, m_effects, 4))
            m_rerender = true;


        if(m_selected == 1) // Blur
            if(ImGui::SliderInt("Blur Radius", &m_blurRadius, 1, 3))
                m_rerender = true;

        ImGui::End();
    }
public:
    GfxExample() : SDLCore(){}
    virtual ~GfxExample() = default;
};


int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    GfxExample example;
    example.Start();
    return 0;
}


