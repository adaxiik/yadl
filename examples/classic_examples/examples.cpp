#include <iostream>
#include <cmath>

#include <yadl/canvas.hpp>
#include <yadl/shape.hpp>
#include <yadl/debug.hpp>
#include <yadl/text.hpp>
#include <yadl/font.hpp>
#include <yadl/io.hpp>
#include <yadl/context.hpp>

void asserts()
{
    static_assert(sizeof(yadl::Pixel) == 4, "Pixel size is not 4 bytes");
    static_assert(yadl::Pixel(1,2,3,4) == yadl::Pixel::RGBA(1,2,3,4), "Pixel comparison failed");
    static_assert(yadl::Pixel(0x04030201) == yadl::Pixel::RGBA(1,2,3,4), "Pixel comparison failed");
    static_assert(yadl::Pixel(0,2,3,4) != yadl::Pixel::RGBA(1,2,3,4), "Pixel comparison failed");
}

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

    io::SaveAsPNG(outputFilename, canvas);
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

    // canvas.Save(outputFilename, FileFormat::PNG);
    io::SaveAsPNG(outputFilename, canvas);
}

void example_load(const std::string &outputFilename, const std::string &inputFilename)
{
    YADL_FUNCTION_PERF(std::cout);

    using namespace yadl;

    Canvas canvas(inputFilename);

    auto subCanvas = canvas.SubCanvas(canvas.GetWidth() / 4, canvas.GetHeight() / 4, canvas.GetWidth() / 2, canvas.GetHeight() / 2);
    subCanvas.Clear(Color::Blue);

    // canvas.Save(outputFilename, FileFormat::PNG);
    io::SaveAsPNG(outputFilename, canvas);
}

void example_deepcopy(const std::string &outputOriginal, const std::string &outputSubCanvas)
{
    YADL_FUNCTION_PERF(std::cout);

    using namespace yadl;

    Canvas canvas(50, 50);
    canvas.Clear(Color::Magenta).SubCanvas(10, 10, 30, 30).Clear(Color::Red);


    auto deepCopy = canvas.DeepCopy();
    deepCopy.SubCanvas(15, 15, 10, 10).Clear(Color::Blue);

    // canvas.Save(outputOriginal, FileFormat::PNG);
    // deepCopy.Save(outputSubCanvas, FileFormat::PNG);
    io::SaveAsPNG(outputOriginal, canvas);
    io::SaveAsPNG(outputSubCanvas, deepCopy);
}

void example_resize(const std::string &outputWH, const std::string &outputFactor, const std::string &inputFilename)
{
    YADL_FUNCTION_PERF(std::cout);

    using namespace yadl;

    Canvas canvas(inputFilename);

    auto absolute = canvas.Resize(200, 200);
    io::SaveAsPNG(outputWH, absolute);

    auto relative = canvas.Resize(2.f);
    io::SaveAsPNG(outputFactor, relative);

}

void example_shape_actions(const std::string &outputBlend, const std::string &outputAdd, const std::string &outputSub, const std::string &outputSet)
{
    YADL_FUNCTION_PERF(std::cout);

    using namespace yadl;
    
    Canvas original(50, 50);
    original.Clear(Pixel(255, 0, 0, 128));
   
    // Shape::Get().SetPosition(0,0).SetColor(Pixel(0, 255, 0, 128));
    Context ctx(original);
    
    ctx.SetPosition(0,0).SetColor(Pixel(0, 255, 0, 128));
    
    auto blend = original.DeepCopy();
    ctx.SetCanvas(blend).SetModeBlend();
    Shape::DrawFilledRectangle(ctx, 25, 25);
    io::SaveAsPNG(outputBlend, blend);
    
    auto add = original.DeepCopy();
    ctx.SetCanvas(add).SetModeAdd();
    Shape::DrawFilledRectangle(ctx, 25, 25);
    io::SaveAsPNG(outputAdd, add);
    
    auto set = original.DeepCopy();
    ctx.SetCanvas(set).SetModeSet();
    Shape::DrawFilledRectangle(ctx, 25, 25);
    io::SaveAsPNG(outputSet, set);
    
    auto sub = original.DeepCopy();
    ctx.SetCanvas(sub).SetModeSub();
    Shape::DrawFilledRectangle(ctx, 25, 25);
    io::SaveAsPNG(outputSub, sub);
    
}

void example_thickness(const std::string &outputFilename)
{
    YADL_FUNCTION_PERF(std::cout);
    using namespace yadl;

    Canvas canvas(200, 200);
    canvas.Clear(Color::Dark);

    Context ctx(canvas);
    ctx.SetPosition(canvas.GetCenterX(), canvas.GetCenterY());
    ctx.SetThickness(2).SetColor(Color::Black);

    for (int i = 0; i < 9; i++)
        Shape::DrawCircle(ctx, 10 + i * 10);

    ctx.SetPosition(60,140).SetColor(Color::Red);
    Shape::DrawFilledCircle(ctx, 15);

    
    ctx.SetPosition(canvas.GetCenterX(), canvas.GetCenterY()).SetColor(Color::Blue);
    Shape::DrawLine(ctx, 0,0);

    ctx.SetThickness(5);
    Shape::DrawLine(ctx, canvas.GetWidth(), 0);

    ctx.SetThickness(3).SetColor(Color::Green).SetPosition(20,20);
    Shape::DrawRectangle(ctx, 160,160);
    ctx.SetThickness(1).SetColor(Color::Yellow).SetPosition(40,40);
    Shape::DrawRectangle(ctx, 120,120);

    io::SaveAsPNG(outputFilename, canvas);
}

void example_text(const std::string &outputFilename)
{
    YADL_FUNCTION_PERF(std::cout);
    using namespace yadl;

    Canvas canvas(300, 100);
    canvas.Clear(Color::Dark);

    Context ctx(canvas);
    Font font("assets/RobotoCondensed-Regular.ttf");

    ctx.SetFont(font).SetColor(Color::White).SetPosition(20, 42);
    Text::DrawText(ctx, "I hate text\nrendering :c");

    io::SaveAsPNG(outputFilename, canvas);
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

    // Canvas canvas(...);
    // Context ctx(canvas);
    // ctx.set_color(RED);
    // ctx.move_to(100, 100);
    // ctx.line_to(120, 120);
    // ctx.line_to(140, 100);
    // {
    //     auto guard = ctx.save();
    //     ctx.set_color(BLUE); // nyní je barva modrá
    //     ctx.rectangle(...);
    // } // guard skončil, zavolá se ctx.restore(), barva se vrátí na červenou

    
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
