#include <iostream>
#include <cmath>

#include <yadl/canvas.hpp>
#include <yadl/shape.hpp>
#include <yadl/debug.hpp>
#include <yadl/text.hpp>
#include <yadl/font.hpp>

void example_subcanvas(const std::string &outputFilename)
{
    YADL_FUNCTION_PERF(std::cout);

    using namespace yadl;

    Canvas canvas(50, 50);
    canvas.Clear(Color::Magenta);
    canvas.SetPixel(0, 5, Color::Red);

    auto subCanvas = canvas.SubCanvas(10, 10, 10, 10);
    subCanvas.Clear(Color::Blue);

    auto subsubCanvas = subCanvas.SubCanvas(2, 2, 5, 5);
    subsubCanvas.Clear(Color::Green);

    canvas.Save(outputFilename, FileFormat::PNG);
}

void example_subcanvas2(const std::string &outputFilename)
{
    YADL_FUNCTION_PERF(std::cout);

    using namespace yadl;
    constexpr int width = 512;
    constexpr int height = 512;
    Canvas canvas(width, height);
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            canvas.SetPixel(x, y, Pixel::RGBA(static_cast<float>(x) / width * 255, static_cast<float>(y) / height * 255, 0, 255));

    auto subCanvas = canvas.SubCanvas(width / 4, height / 4, width / 2, height / 2);
    for (int y = 0; y < subCanvas.GetHeight(); y++)
        for (int x = 0; x < subCanvas.GetWidth(); x++)
            subCanvas.SetPixel(x, y, Pixel::RGBA(0, static_cast<float>(x) / subCanvas.GetWidth() * 255, static_cast<float>(y) / subCanvas.GetHeight() * 255, 255));

    canvas.Save(outputFilename, FileFormat::PNG);
}

void example_load(const std::string &outputFilename, const std::string &inputFilename)
{
    YADL_FUNCTION_PERF(std::cout);

    using namespace yadl;

    Canvas canvas(inputFilename);

    auto subCanvas = canvas.SubCanvas(canvas.GetWidth() / 4, canvas.GetHeight() / 4, canvas.GetWidth() / 2, canvas.GetHeight() / 2);
    subCanvas.Clear(Color::Blue);

    canvas.Save(outputFilename, FileFormat::PNG);
}

void example_deepcopy(const std::string &outputOriginal, const std::string &outputSubCanvas)
{
    YADL_FUNCTION_PERF(std::cout);

    using namespace yadl;

    Canvas canvas(50, 50);
    canvas.Clear(Color::Magenta).SubCanvas(10, 10, 30, 30).Clear(Color::Red);


    auto deepCopy = canvas.DeepCopy();
    deepCopy.SubCanvas(15, 15, 10, 10).Clear(Color::Blue);

    canvas.Save(outputOriginal, FileFormat::PNG);
    deepCopy.Save(outputSubCanvas, FileFormat::PNG);
}

void example_resize(const std::string &outputWH, const std::string &outputFactor, const std::string &inputFilename)
{
    YADL_FUNCTION_PERF(std::cout);

    using namespace yadl;

    Canvas canvas(inputFilename);
    canvas.Resize(200, 200).Save(outputWH, FileFormat::PNG);

    canvas.Resize(2.f).Save(outputFactor, FileFormat::PNG);

}

void example_shape_actions(const std::string &outputBlend, const std::string &outputAdd, const std::string &outputSub, const std::string &outputSet)
{
    YADL_FUNCTION_PERF(std::cout);

    using namespace yadl;

    Canvas original(50, 50);
    original.Clear(Pixel(255, 0, 0, 128));

    Shape::Get().SetPosition(0,0).SetColor(Pixel(0, 255, 0, 128));

    auto blend = original.DeepCopy();
    Shape::Get().SetCanvas(blend).SetAction(Shape::Action::Blend);
    Shape::Get().DrawFilledRectangle(25, 25);
    blend.Save(outputBlend, FileFormat::PNG);

    auto add = original.DeepCopy();
    Shape::Get().SetCanvas(add).SetAction(Shape::Action::Add);
    Shape::Get().DrawFilledRectangle(25, 25);
    add.Save(outputAdd, FileFormat::PNG);
    
    auto set = original.DeepCopy();
    Shape::Get().SetCanvas(set).SetAction(Shape::Action::Set);
    Shape::Get().DrawFilledRectangle(25, 25);
    set.Save(outputSet, FileFormat::PNG);

    auto sub = original.DeepCopy();
    Shape::Get().SetCanvas(sub).SetAction(Shape::Action::Sub).SetColor(Pixel(50, 0, 255, 0));
    Shape::Get().DrawFilledRectangle(25, 25);
    sub.Save(outputSub, FileFormat::PNG);
    Shape::Get().SetAction(Shape::Action::Set);
    
}

void example_thickness(const std::string &outputFilename)
{
    YADL_FUNCTION_PERF(std::cout);
    using namespace yadl;

    Canvas canvas(200, 200);
    canvas.Clear(Color::Dark);

    Shape::Get().SetCanvas(canvas).SetColor(Color::Red).SetThickness(1);
    Shape::Get().SetPosition(canvas.GetCenterX(), canvas.GetCenterY());
    Shape::Get().SetThickness(2).SetColor(Color::Black);
    
    for (int i = 0; i < 9; i++)
        Shape::Get().DrawCircle(10 + i * 10);

    Shape::Get().SetPosition(60,140).SetColor(Color::Red).DrawFilledCircle(15);


    Shape::Get().SetPosition(canvas.GetCenterX(), canvas.GetCenterY());
    Shape::Get().SetColor(Color::Blue).DrawLine(0,0);

    Shape::Get().SetThickness(5).DrawLine(canvas.GetWidth(), 0);

    Shape::Get().SetThickness(3).SetColor(Color::Green).SetPosition(20,20).DrawRectangle(160,160);
    Shape::Get().SetThickness(1).SetColor(Color::Yellow).SetPosition(40,40).DrawRectangle(120,120);


    canvas.Save(outputFilename, FileFormat::PNG);
}

void example_text(const std::string &outputFilename)
{
    YADL_FUNCTION_PERF(std::cout);
    using namespace yadl;

    Canvas canvas(300, 100);
    canvas.Clear(Color::Dark);

    Font font("assets/RobotoCondensed-Regular.ttf");
    Text::Get().SetCanvas(canvas).SetFont(font).SetColor(Color::White).SetPosition(20, 42);
    Text::Get().DrawText("I hate text\nrendering :c");

    canvas.Save(outputFilename, FileFormat::PNG);
}

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;
    YADL_PERF_START(ALL_EXAMPLES);
    example_subcanvas("subcanvas.png");
    example_subcanvas2("subcanvas2.png");
    example_load("load.png", "assets/cat.png");
    example_deepcopy("deepcopy_original.png", "deepcopy_subcanvas.png");
    example_resize("resizeWH.png", "resizeFactor.png", "assets/cat.png");
    example_shape_actions("action_blend.png", "action_add.png", "action_sub.png", "action_set.png");
    example_thickness("thickness.png");
    example_text("text.png");
    YADL_PERF_END(ALL_EXAMPLES);
    
    // TODOOO:
    // Canvas tex("texture.png", ImageFormat::PNG);
    // tex.Resize(50, 50);
    // textures::DrawTexture(canvas, tex, 0, 0);

    // gfx::ToGrayscale(canvas);
    // gfx::Invert(canvas);
    // gfx::Blur(canvas, 5);

    // canvas.Save("test.jpg", ImageFormat::JPG);

    return 0;
}
