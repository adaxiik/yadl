#pragma once
#include <string>

#include "context.hpp"
namespace yadl
{
    class Text
    {
    private:
        Text() = delete;
        Text(const Text &) = delete;
        Text &operator=(const Text &) = delete;
        Text(Text &&) = delete;
        Text &operator=(Text &&) = delete;
        ~Text() = delete;
    public:
        static void DrawText(Context& ctx, const std::string& text);
    };
}