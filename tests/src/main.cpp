#include <iostream>

#include <yadl/canvas.hpp>
#include <yadl/color.hpp>


int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;
    using namespace yadl;

    Canvas canvas(50, 50);
    canvas.Clear(Color::White);

    auto subCanvas = canvas.SubCanvas(10, 10, 10, 10);
    subCanvas.Clear(Color::Blue);

    auto subsubCanvas = subCanvas.SubCanvas(2, 2, 5, 5);
    subsubCanvas.Clear(Color::Green);

    subCanvas.Save("sub.png", FileFormat::PNG);
    canvas.Save("test.png", FileFormat::PNG);

    subCanvas.Save("sub.ppm", FileFormat::PPM);
    canvas.Save("test.ppm", FileFormat::PPM);



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
