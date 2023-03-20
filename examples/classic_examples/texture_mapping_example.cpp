#include <yadl/yadl.hpp>

int main(int argc, char const *argv[])
{
    using namespace yadl;
    (void)argc;
    (void)argv;

    Canvas canvas(500, 500);
    canvas.Clear(Color::Dark);
    Context ctx(canvas);
    ctx.SetModeBlend(); // our cpp texture has transparency

    Canvas texture("assets/cpp.png"); // not recommended to do that, can throw an exception

    int w = canvas.GetWidth();
    int h = canvas.GetHeight();

    // 2 triangles relative to the canvas size
    tris::FloatTriangle left_bottom(w / 2 - w / 4, h / 2 - h / 4
                          , w / 2 - w / 4, h / 2 + h / 4
                          , w / 2 + w / 4, h / 2 + h / 4);

    tris::FloatTriangle right_top(w / 2 - w / 4, h / 2 - h / 4
                           , w / 2 + w / 4, h / 2 - h / 4
                           , w / 2 + w / 4, h / 2 + h / 4);

    // texture coordinates 
    tris::FloatTriangle uv_lb(0, 0, 0, 1, 1, 1);
    tris::FloatTriangle uv_rt(0, 0, 1, 0, 1, 1);

    Animation animation(canvas.GetWidth(), canvas.GetHeight());
    const int frames = 100;
    constexpr float PI = 3.14159;
    const float angle = 2 * PI / frames;

    const int cx = canvas.GetCenterX();
    const int cy = canvas.GetCenterY();

    for (int i = 0; i < frames; i++)
    {
        canvas.Clear(Color::Dark);
        left_bottom.Rotate(cx, cy, angle);
        right_top.Rotate(cx, cy, angle);

        tris::DrawTexturedTriangle(ctx, left_bottom.ToIntTriangle(), uv_lb, texture);
        tris::DrawTexturedTriangle(ctx, right_top.ToIntTriangle(), uv_rt, texture);

        animation.AddFrame(canvas);
    }

    io::SaveAsGIF("textured_triangle.gif", animation, 2);

    return 0;
}
