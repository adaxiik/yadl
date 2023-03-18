#include "shape.hpp"

namespace yadl
{
    void shape::DrawFilledRectangle(Context& ctx, int32_t width, int32_t height)
    {
        auto& state = ctx.GetState();

        for (int32_t y = state.positionY; y < state.positionY + height; y++)
            for (int32_t x = state.positionX; x < state.positionX + width; x++)
                state.action(state.canvas.RefPixel(x, y));
        
    }

    void shape::DrawFilledCircle(Context& ctx, int32_t radius)
    {
        auto& state = ctx.GetState();

        for (int32_t y = state.positionY - radius; y < state.positionY + radius; y++)
            for (int32_t x = state.positionX - radius; x < state.positionX + radius; x++)
                if ((x - state.positionX) * (x - state.positionX) + (y - state.positionY) * (y - state.positionY) <= radius * radius)
                    state.action(state.canvas.RefPixel(x, y));
    }

    void shape::DrawLine(Context& ctx, int32_t x1, int32_t y1)
    {
        // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
        // stupid and naive, but gets the job done
        
        auto& state = ctx.GetState();

        int32_t x0 = state.positionX;
        int32_t y0 = state.positionY;
        int32_t dx = std::abs(x1 - x0);
        int32_t dy = -std::abs(y1 - y0);
        int32_t sx = x0 < x1 ? 1 : -1;
        int32_t sy = y0 < y1 ? 1 : -1;
        int32_t err = dx + dy;

        std::function<void(int32_t, int32_t)> draw;
        if (state.thickness == 1)
        {
            draw = [&](int32_t x0, int32_t y0)
            {
                state.action(state.canvas.RefPixel(x0, y0));
            };
        }
        else
        {
            draw = [&](int32_t x0, int32_t y0)
            {
                for (int32_t y = y0 - state.thickness; y < y0 + state.thickness; y++)
                    for (int32_t x = x0 - state.thickness; x < x0 + state.thickness; x++)
                        if ((x - x0) * (x - x0) + (y - y0) * (y - y0) <= state.thickness * state.thickness)
                            state.action(state.canvas.RefPixel(x, y));
            };
        }

        while (true)
        {
            draw(x0, y0);
            if (x0 == x1 && y0 == y1)
                break;
            int32_t e2 = 2 * err;
            if (e2 >= dy)
            {
                err += dy;
                x0 += sx;
            }
            if (e2 <= dx)
            {
                err += dx;
                y0 += sy;
            }
        }
    }

    void shape::DrawRectangle(Context& ctx, int32_t width, int32_t height)
    {
        auto& state = ctx.GetState();

        if(state.thickness == 1)
        {
            for (int32_t y = state.positionY; y < state.positionY + height; y++)
            {
                state.action(state.canvas.RefPixel(state.positionX, y));
                state.action(state.canvas.RefPixel(state.positionX + width, y));
            }
            for (int32_t x = state.positionX; x < state.positionX + width; x++)
            {
                state.action(state.canvas.RefPixel(x, state.positionY));
                state.action(state.canvas.RefPixel(x, state.positionY + height));
            }
            return;
        }

        int32_t originalPositionX = state.positionX;
        int32_t originalPositionY = state.positionY;

        DrawLine(ctx, originalPositionX + width, originalPositionY);
        DrawLine(ctx, originalPositionX, originalPositionY + height);

        ctx.SetPosition(originalPositionX + width, originalPositionY + height);

        DrawLine(ctx, originalPositionX + width, originalPositionY);
        DrawLine(ctx, originalPositionX, originalPositionY + height);

        ctx.SetPosition(originalPositionX, originalPositionY);
    }

    void shape::DrawCircle(Context& ctx, int32_t radius)
    {
        auto& state = ctx.GetState();

       // https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
        std::function<void(int32_t, int32_t)> draw;
        if (state.thickness == 1)
        {
            draw = [&](int32_t x0, int32_t y0)
            {
                state.action(state.canvas.RefPixel(x0, y0));
            };
        }
        else
        {
            draw = [&](int32_t x0, int32_t y0)
            {
                for (int32_t y = y0 - state.thickness; y < y0 + state.thickness; y++)
                    for (int32_t x = x0 - state.thickness; x < x0 + state.thickness; x++)
                        if ((x - x0) * (x - x0) + (y - y0) * (y - y0) <= state.thickness * state.thickness)
                            state.action(state.canvas.RefPixel(x, y));
            };
        }

         int32_t x = 0;
        int32_t y = radius;
        int32_t d = 3 - 2 * radius;

        auto drawCirclePart = [&](int32_t x, int32_t y)
        {
            draw(state.positionX + x, state.positionY + y);
            draw(state.positionX - x, state.positionY + y);
            draw(state.positionX + x, state.positionY - y);
            draw(state.positionX - x, state.positionY - y);
            draw(state.positionX + y, state.positionY + x);
            draw(state.positionX - y, state.positionY + x);
            draw(state.positionX + y, state.positionY - x);
            draw(state.positionX - y, state.positionY - x);
        };

        drawCirclePart(x, y);

        while (y >= x)
        {
            x++;

            if (d > 0)
            {
                y--;
                d = d + 4 * (x - y) + 10;
            }
            else
                d = d + 4 * x + 6;

            drawCirclePart(x, y);
        }
    }


    void shape::DrawFilledCircleAA(Context& ctx, int32_t radius)
    {
        auto& state = ctx.GetState();
        // I had to improvise a bit here c:
        auto originalAction = state.action;
        Pixel originalColor = state.color;
        ctx.SetModeBlend();

        constexpr int32_t AA_RES = 2;
        constexpr int32_t AA_RES1 = AA_RES + 1;

        for (int32_t y = state.positionY - radius; y < state.positionY + radius; ++y)
        {
            for (int32_t x = state.positionX - radius; x < state.positionX + radius; ++x)
            {
                int32_t count{0};
                for (int32_t i = 0; i < AA_RES; i++)
                {
                    for (int32_t j = 0; j < AA_RES; j++)
                    {
                        // kindly borrowed from another drawing library c:
                        
                        int32_t dx = (x * AA_RES1 * 2 + 2 + i*2 - AA_RES1 * state.positionX*2 - AA_RES1);
                        int32_t dy = (y * AA_RES1 * 2 + 2 + j*2 - AA_RES1 * state.positionY*2 - AA_RES1);
                        
                        if (dx * dx + dy * dy <= radius * radius * AA_RES1 * AA_RES1 * 4)
                            count++;
                    }
                }
                uint32_t alpha = originalColor.a * count / AA_RES / AA_RES;
                Pixel p{state.color.r, state.color.g, state.color.b, static_cast<uint8_t>(alpha)};
                state.color = p; // might be actually faster and easier to access the canvas directly 
                state.action(state.canvas.RefPixel(x, y));
            }
        }
        state.color = originalColor;
        state.action = originalAction;
    }

    void shape::DrawCircleAA(Context& ctx, int32_t radius)
    {
        auto& state = ctx.GetState();

        // https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
        std::function<void(int32_t, int32_t)> draw;
        if (state.thickness == 1)
        {
            draw = [&](int32_t x0, int32_t y0)
            {
                state.action(state.canvas.RefPixel(x0, y0));
            };
        }
        else
        {
            draw = [&](int32_t x0, int32_t y0)
            {
                int32_t originalPositionX = state.positionX;
                int32_t originalPositionY = state.positionY;
                state.positionX = x0;
                state.positionY = y0;
                DrawFilledCircleAA(ctx, state.thickness);
                state.positionX = originalPositionX;
                state.positionY = originalPositionY;
            };
        }

        int32_t x = 0;
        int32_t y = radius;
        int32_t d = 3 - 2 * radius;

        auto drawCirclePart = [&](int32_t x, int32_t y)
        {
            draw(state.positionX + x, state.positionY + y);
            draw(state.positionX - x, state.positionY + y);
            draw(state.positionX + x, state.positionY - y);
            draw(state.positionX - x, state.positionY - y);
            draw(state.positionX + y, state.positionY + x);
            draw(state.positionX - y, state.positionY + x);
            draw(state.positionX + y, state.positionY - x);
            draw(state.positionX - y, state.positionY - x);
        };

        drawCirclePart(x, y);

        while (y >= x)
        {
            x++;

            if (d > 0)
            {
                y--;
                d = d + 4 * (x - y) + 10;
            }
            else
                d = d + 4 * x + 6;

            drawCirclePart(x, y);
        }
    }


    void shape::DrawLineAA(Context& ctx, int32_t x1, int32_t y1)
    {
        // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
        // stupid and naive, but gets the job done
        
        auto& state = ctx.GetState();

        int32_t x0 = state.positionX;
        int32_t y0 = state.positionY;
        int32_t dx = std::abs(x1 - x0);
        int32_t dy = -std::abs(y1 - y0);
        int32_t sx = x0 < x1 ? 1 : -1;
        int32_t sy = y0 < y1 ? 1 : -1;
        int32_t err = dx + dy;

        std::function<void(int32_t, int32_t)> draw;
        if (state.thickness == 1)
        {
            draw = [&](int32_t x0, int32_t y0)
            {
                state.action(state.canvas.RefPixel(x0, y0));
            };
        }
        else
        {
            draw = [&](int32_t x0, int32_t y0)
            {
                int32_t originalPositionX = state.positionX;
                int32_t originalPositionY = state.positionY;
                state.positionX = x0;
                state.positionY = y0;
                DrawFilledCircleAA(ctx, state.thickness);
                state.positionX = originalPositionX;
                state.positionY = originalPositionY;
            };
        }

        while (true)
        {
            draw(x0, y0);
            if (x0 == x1 && y0 == y1)
                break;
            int32_t e2 = 2 * err;
            if (e2 >= dy)
            {
                err += dy;
                x0 += sx;
            }
            if (e2 <= dx)
            {
                err += dx;
                y0 += sy;
            }
        }
    }

}