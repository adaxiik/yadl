#include <iostream>

#include <yadl/canvas.hpp>
#include <yadl/color.hpp>

bool example_subcanvas(const std::string &outputFilename)
{
    using namespace yadl;

    Canvas canvas(50, 50);
    canvas.Clear(Color::Magenta);
    canvas.SetPixel(0, 5, Color::Red);

    auto subCanvas = canvas.SubCanvas(10, 10, 10, 10);
    subCanvas.Clear(Color::Blue);

    auto subsubCanvas = subCanvas.SubCanvas(2, 2, 5, 5);
    subsubCanvas.Clear(Color::Green);

    return canvas.Save(outputFilename, FileFormat::PNG);
}

bool example_subcanvas2(const std::string &outputFilename)
{
    using namespace yadl;
    constexpr int width = 1000;
    constexpr int height = 1000;
    Canvas canvas(width, height);
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            canvas.SetPixel(x, y, Pixel::RGBA(static_cast<float>(x) / width * 255, static_cast<float>(y) / height * 255, 0, 255));

    auto subCanvas = canvas.SubCanvas(width / 4, height / 4, width / 2, height / 2);
    for (int y = 0; y < subCanvas.GetHeight(); y++)
        for (int x = 0; x < subCanvas.GetWidth(); x++)
            subCanvas.SetPixel(x, y, Pixel::RGBA(0, static_cast<float>(x) / subCanvas.GetWidth() * 255, static_cast<float>(y) / subCanvas.GetHeight() * 255, 255));

    return canvas.Save(outputFilename, FileFormat::PNG);
}

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;
    example_subcanvas("subcanvas.png");
    example_subcanvas2("subcanvas2.png");

    // // canvas.Clear({255, 255, 255, 255}); // White

    // canvas.SetPixel(0, 0, Color::Red);
    // Color color = canvas.GetPixel(0, 0);

    // // shapes::Circle(canvas, 100, 100, 50, color);
    // // shapes::FilledRectangle(canvas, 0, 0, 100, 100, {0, 255, 0, 255});
    // // shapes::Line(canvas, 50, 50, 100, 100, {0, 0, 255, 255});

    // Shapes::Get().SetCanvas(canvas);

    // Shapes::Get()
    //     .SetColor(color)
    //     .SetPosition(100, 100)
    //     .DrawCircle(50);

    // Shapes::Get()
    //     .SetPosition(0, 0)
    //     .SetColor({0, 255, 0, 255})
    //     .DrawFilledRectangle(100, 100);

    // Shapes::Get()
    //     .SetColor({0, 0, 255, 255})
    //     .SetPosition(50, 50)
    //     .DrawLine(100, 100);

    // Font font = Font("test.ttf", 12);
    // // text::DrawText(canvas, font, "Hello World!", 0, 0, Color::Black);
    // Text::Get()
    //     .SetCanvas(canvas)
    //     .SetFont(font)
    //     .SetColor(Color::Black)
    //     .SetPosition(0, 0)
    //     .DrawText("Hello World!");

    // Canvas tex("texture.png", ImageFormat::PNG);
    // tex.Resize(50, 50);
    // textures::DrawTexture(canvas, tex, 0, 0);

    // gfx::ToGrayscale(canvas);
    // gfx::Invert(canvas);
    // gfx::Blur(canvas, 5);

    // canvas.Save("test.jpg", ImageFormat::JPG);

    return 0;
}
