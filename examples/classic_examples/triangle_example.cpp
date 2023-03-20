#include <iostream>
#include <yadl/yadl.hpp>
#include <cmath>


int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;
    constexpr float PI = 3.14159;

    using namespace yadl;
    Canvas canvas(300, 300);
    Animation animation(canvas.GetWidth(), canvas.GetHeight());

    canvas.Clear(Color::Dark);
    Context ctx(canvas);

    const float cx = canvas.GetCenterX();
    const float cy = canvas.GetCenterY();

    float x0 = canvas.GetCenterX();
    float y0 = canvas.GetCenterY() - canvas.GetHeight() / 4;
    float x1 = canvas.GetCenterX() - canvas.GetWidth() / 4;
    float y1 = canvas.GetCenterY() + canvas.GetHeight() / 4;
    float x2 = canvas.GetCenterX() + canvas.GetWidth() / 4;
    float y2 = canvas.GetCenterY() + canvas.GetHeight() / 4;

    tris::FloatTriangle triangle(x0, y0, x1, y1, x2, y2);

    const int frames = 300;
    const float angle = 2 * PI / frames;

    for (int i = 0; i < frames; i++)
    {
        canvas.Clear(Color::Dark);
        triangle.Rotate(cx, cy, angle);

        tris::DrawTriangle(ctx, triangle.ToIntTriangle() , Color::Red, Color::Green, Color::Blue);        

        animation.AddFrame(canvas);
    }

    io::SaveAsGIF("triangle.gif", animation, 2);

    return 0;
}
