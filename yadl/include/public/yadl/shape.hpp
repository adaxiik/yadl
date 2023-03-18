#include <iostream>
#include <memory>
#include <functional>

#include "canvas.hpp"
#include "pixel.hpp"
#include "context.hpp"

namespace yadl
{
    class Shape
    {
    private:
        Shape() = default;
        Shape(const Shape &) = delete;
        Shape &operator=(const Shape &) = delete;
        Shape(Shape &&) = delete;
        Shape &operator=(Shape &&) = delete;
        ~Shape() = default;
    public:
        static void DrawFilledRectangle(Context& ctx, int32_t width, int32_t height);
        static void DrawRectangle(Context& ctx, int32_t width, int32_t height);

        static void DrawFilledCircle(Context& ctx, int32_t radius);
        static void DrawCircle(Context& ctx, int32_t radius);
        static void DrawLine(Context& ctx, int32_t x, int32_t y);

        static void DrawFilledCircleAA(Context& ctx, int32_t radius);
        static void DrawCircleAA(Context& ctx, int32_t radius);
        static void DrawLineAA(Context& ctx, int32_t x, int32_t y);
    };

}