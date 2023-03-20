#include <yadl/yadl.hpp>

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;
    using namespace yadl;
    
    Canvas canvas(500, 500);
    canvas.Clear(Color::Dark);
    Context ctx(canvas);
    ctx.SetColor(Color::Red).SetPosition(canvas.GetCenterX(), canvas.GetCenterY());
    
    int radius = 150;
    shape::DrawFilledCircleAA(ctx, radius); // AA stands for antialiased

    io::SaveAsPNG("circle.png", canvas);
    return 0;
}