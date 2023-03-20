#include <iostream>
#include <yadl/yadl.hpp>
#include <cmath>

static inline void RotatePoint(float &x, float &y, float cx, float cy, float angle)
{
    const float s = std::sin(angle);
    const float c = std::cos(angle);

    // translate point back to origin
    x -= cx;
    y -= cy;

    // rotate point
    const float xnew = x * c - y * s;
    const float ynew = x * s + y * c;

    // translate point back
    x = xnew + cx;
    y = ynew + cy;
}

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


    const int frames = 300;
    const float angle = 2 * PI / frames;

    for (int i = 0; i < frames; i++)
    {
        RotatePoint(x0, y0, cx, cy, angle);
        RotatePoint(x1, y1, cx, cy, angle);
        RotatePoint(x2, y2, cx, cy, angle);

        canvas.Clear(Color::Dark);
        int32_t x0i = static_cast<int32_t>(x0);
        int32_t y0i = static_cast<int32_t>(y0);
        int32_t x1i = static_cast<int32_t>(x1);
        int32_t y1i = static_cast<int32_t>(y1);
        int32_t x2i = static_cast<int32_t>(x2);
        int32_t y2i = static_cast<int32_t>(y2);

        tris::DrawTriangle(ctx, x0i, y0i, x1i, y1i, x2i, y2i, Color::Red, Color::Green, Color::Blue);        

        animation.AddFrame(canvas);
    }

    io::SaveAsGIF("triangle.gif", animation, 2);

    return 0;
}
